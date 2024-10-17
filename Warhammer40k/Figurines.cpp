#include "Figurines.h"
#include "QueryFlags.h"
#include "PathFindingComponent.h"

#include <OgreMath.h>
#include <iostream>


#include "Grid.h"

Figurines::Figurines(GameEngine &gameEngineP, std::string entityNameP, std::string nodeNameP, int ownerP) :
    mGameEngine(gameEngineP),
    mCurrentHealthPoint( mMaxHealthPoint ),
    mCurrentMovementAction( mMaxMovementAction ),
    mCurrentActionPoint( mMaxActionPoint )
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
}

void Figurines::Update(float deltaTimeP)
{   
    if (mIsSelected)
    {
        mAnimationTime += deltaTimeP;

        float scale = mUniformScale + sin(mAnimationTime * mScaleSpeed) * mScaleFactor;
        float flatten = mUniformScale + cos(mAnimationTime * mFlattenSpeed) * mFlattenFactor;

        Ogre::Vector3 newScale(flatten, scale, flatten);
        mNode->setScale(newScale);
    }
    else
        mAnimationTime = 0;

    if (mIsMoving)
        UpdatePositions(deltaTimeP);
}

void Figurines::UpdatePositions(float deltaTimeP)
{
    if (!mMoveStraight)
    {
        if (mIndexPosition >= mPathfinding->lookPoints.size())
        {
            mIndexPosition = 1;
            mIsMoving = false;
            return;
        }

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
        if (GetPosition().distance(mStraightTargetPosition) <= 0.5f)
        {
            SetPosition(mStraightTargetPosition);
            mIsMoving = false;
        }
    }

    // Translate the entity forward in its local space
    Vector3 forward = mNode->getOrientation() * Ogre::Vector3::UNIT_Z;
    Vector3 translation = forward * deltaTimeP * mMovementSpeed;
    mNode->translate(translation);
}

void Figurines::SetPosition(Vector3 positionP)
{
    positionP.y = mOffset.y;

    mNode->_setDerivedPosition(positionP);
}

void Figurines::SetYawRotation(Degree rotationP)
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
        mNode->setScale(Vector3(mUniformScale, mUniformScale, mUniformScale));
        mPathfinding->HideMovementGrid(true);

        return;
    }

    OnMouseOver(mIsEnemy);
}

void Figurines::OnMouseOver(bool isEnemyP)
{
    mIsEnemy = isEnemyP;
    int mTileType = TILE_MOVEMENT_SELECTED;

    if (mIsEnemy)
        mTileType = TILE_MOVEMENT_ENEMY;
    else if (!mIsSelected)
        mTileType = TILE_MOVEMENT_MOUSEOVER;

    // Show movement action Grid
    mPathfinding->GetMovementGrid(GetPosition(), mCurrentMovementAction, mTileType);
}

void Figurines::OnMouseOut()
{
    // Hide Movement Action Grid
    if(!mIsSelected)
        mPathfinding->HideMovementGrid(false);
}

void Figurines::OnEndTurnEvent()
{
    mCurrentMovementAction = mMaxMovementAction;
    mCurrentActionPoint = mMaxActionPoint;
}

void Figurines::MoveTo(Tile *targetTileP)
{
    Vector3 targetPositionP;

    mPath.clear();
    mPathfinding->RetracePath(mGameEngine.GetGrid().GetTile(GetPosition()), targetTileP);

    mCurrentMovementAction -= mPathfinding->totalCost;

    mPath = mPathfinding->GetTurnPath();

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
    mMoveStraight = false;
    mIsMoving = true;
}

void Figurines::Attack(Figurines *targetP)
{
    targetP->GetHit(1);
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
        std::cout << "IS DEAD";
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
