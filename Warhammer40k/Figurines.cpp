#include "Figurines.h"
#include "QueryFlags.h"
#include "OgreMath.h"

#include "PathFindingComponent.h"
#include "Grid.h"

#include <iostream>

constexpr float POSITION_SNAP_THRESHOLD = 0.5f;

Figurines::Figurines(GameEngine &gameEngineP, const std::string& entityNameP, const std::string& nodeNameP, int ownerP) :
    mGameEngine(gameEngineP),
    mCurrentHealthPoint(FigurineStats::maxHealthPoints),
    mCurrentMovementAction(FigurineStats::maxMovementAction),
    mCurrentActionPoint(FigurineStats::maxActionPoints)
{
    mEntity = mGameEngine.GetSceneManager()->createEntity(entityNameP, "LowPolyMarine.mesh");
    mEntity->setCastShadows(true);
    mEntity->setQueryFlags(QueryFlags::FIGURINE_MASK);

    mNode = mGameEngine.GetSceneManager()->getRootSceneNode()->createChildSceneNode(nodeNameP);
    mNode->attachObject(mEntity);
    mNode->setScale(mUniformScale, mUniformScale, mUniformScale);

    mPathfinding = new PathFindingComponent(mGameEngine);
    AddComponent(mPathfinding);

    mOwnerID = ownerP;
}

Figurines::~Figurines()
{
    delete mPathfinding;
    mPathfinding = nullptr;
}

void Figurines::Update(float deltaTimeP)
{   
    if (mIsSelected)
    {
        UpdateSelectedAnimation(deltaTimeP);
    }
    else
    {
        mSelectedAnim_Time = 0; // Reset the animation
    }

    if (mIsMoving)
    {
        UpdatePositions(deltaTimeP);
    }
}

void Figurines::UpdateSelectedAnimation(float deltaTimeP)
{
    mSelectedAnim_Time += deltaTimeP;

    float verticalScale = mUniformScale + sin(mSelectedAnim_Time * ANIM_SCALE_SPEED) * ANIM_SCALE_FACTOR;
    float horizontalScale = mUniformScale + cos(mSelectedAnim_Time * ANIM_FLATTEN_SPEED) * ANIM_FLATTEN_FACTOR;

    Ogre::Vector3 newScale(horizontalScale, verticalScale, horizontalScale);
    mNode->setScale(newScale);
}

void Figurines::UpdatePositions(float deltaTimeP)
{
    if (!mShouldMoveStraight)
    {
        /* If the figurine passes the last lookPoint, we stop it. */
        if (mIndexPosition >= mPathfinding->lookPoints.size())
        {
            mIndexPosition = 1;
            mIsMoving = false;

            /* 
             * If the figurine is still selected after it's movement, we trigger the OnSelected function,
             * It will show and update its GridMovement
             */
            if (mIsSelected)
            {
                OnSelected(true);
            }

            return;
        }

        /* If the figurine passes the current lookPoint target, it goes to the next */
        Vector2 pos2D = Vector2(GetPosition().x, GetPosition().z);
        if (mPath[mIndexPosition - 1]->HasCrossedLine(pos2D))
        {
            mIndexPosition++;
        }

        Vector3 targetPos = mPathfinding->lookPoints[mIndexPosition];
        LookAt(targetPos, deltaTimeP);
    }
    else
    {
        /* Snap the figurine to the target position if close enough */
        if (GetPosition().distance(mStraightTargetPosition) <= POSITION_SNAP_THRESHOLD)
        {
            SetPosition(mStraightTargetPosition);
            mIsMoving = false;
        }
    }

    /* Translate the entity forward in its local space */
    Vector3 forward = mNode->getOrientation() * Ogre::Vector3::UNIT_Z;
    Vector3 translation = forward * deltaTimeP * ANIM_MOVEMENT_SPEED;
    mNode->translate(translation);
}

void Figurines::SetPosition(Vector3 positionP)
{
    positionP.y = mPositionOffset.y;

    mNode->_setDerivedPosition(positionP);
}

void Figurines::SetYawRotation(const Degree &rotationP)
{
    Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
    orientation.FromAngleAxis(rotationP, Ogre::Vector3::UNIT_Y);

    mNode->_setDerivedOrientation(orientation);
}

void Figurines::OnSelected(bool isSelectedP)
{
    mIsSelected = isSelectedP;

    if (!isSelectedP)
    {
        /* Reset the scale of the figurine to its initial state */
        mNode->setScale(Vector3(mUniformScale, mUniformScale, mUniformScale));
        mPathfinding->HideMovementGrid(true);

        return;
    }

    mPathfinding->GetMovementGrid(GetPosition(), mCurrentMovementAction, TILE_MOVEMENT_SELECTED);
}

void Figurines::OnMouseOver(bool isEnemyP)
{
    int mTileType = isEnemyP ? TILE_MOVEMENT_ENEMY : TILE_MOVEMENT_MOUSEOVER;

    /* Show the corresponding movement action Grid */
    mPathfinding->GetMovementGrid(GetPosition(), mCurrentMovementAction, mTileType);
}

void Figurines::OnMouseOut()
{
    if (!mIsSelected)
    {
        mPathfinding->HideMovementGrid(false);
    }
}

void Figurines::OnEndTurnEvent()
{
    mCurrentMovementAction = FigurineStats::maxMovementAction;
    mCurrentActionPoint = FigurineStats::maxActionPoints;
}

void Figurines::MoveTo(Tile *targetTileP)
{
    Vector3 targetPositionP;

    mPath.clear();

    // OVERKILL ??
    // int expectedPathSize = static_cast<int>(mCurrentMovementAction / tileSize);
    // mPath.reserve(expectedPathSize);  // Reserve based on an estimated size

    mPathfinding->RetracePath(mGameEngine.GetGrid().GetTile(GetPosition()), targetTileP);
    mPath = mPathfinding->GetTurnPath();

    mCurrentMovementAction -= mPathfinding->totalCost;
    mPathfinding->HideMovementGrid(true);

    /* Orient the figurine to the first point on the path */
    Vector3 targetPos = mPathfinding->lookPoints[1];
    targetPos.y = 0;
    Vector3 currentPos = GetPosition();
    currentPos.y = 0;

    Vector3 direction = (targetPos - currentPos).normalisedCopy();

    Quaternion targetRotationYaw = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
    Radian yaw = targetRotationYaw.getYaw();
    Quaternion targetRotationYawOnly(yaw, Vector3::UNIT_Y);

    mNode->_setDerivedOrientation(targetRotationYawOnly);
    mShouldMoveStraight = false;
    mIsMoving = true;
}

void Figurines::Attack(Figurines *targetP)
{
    targetP->GetHit(FigurineStats::attackDamage);
    mCurrentActionPoint--;
}

void Figurines::GetHit(int damageAmountP)
{
    mCurrentHealthPoint -= damageAmountP;

    if (mCurrentHealthPoint <= 0)
    {
        mIsDead = true;

        mGameEngine.GetSceneManager()->destroyEntity(mEntity);
        mGameEngine.GetSceneManager()->destroySceneNode(mNode);

        mGameEngine.RemoveActor(this);
    }
}

void Figurines::LookAt(const Ogre::Vector3 &targetPositionP, float deltaTimeP)
{
    Quaternion currentRotation = mNode->_getDerivedOrientation();
    Vector3 direction = (targetPositionP - GetPosition()).normalisedCopy();

    Quaternion targetRotationYaw = Ogre::Vector3::UNIT_Z.getRotationTo(direction);

    // Extract the yaw angle from the full rotation
    Radian yaw = targetRotationYaw.getYaw();
    Quaternion targetRotationYawOnly(yaw, Vector3::UNIT_Y);

    Quaternion myRotation = Quaternion::Slerp(deltaTimeP * mTurnSpeed, currentRotation, targetRotationYawOnly, true);

    mNode->_setDerivedOrientation(myRotation);
}
