#include "Figurines.h"
#include "QueryFlags.h"
#include "PathFindingComponent.h"

#include <OgreMath.h>
#include <iostream>

Figurines::Figurines(GameEngine& gameEngineP, std::string entityNameP, std::string nodeNameP, int owner) :
    m_GameEngine(gameEngineP)
{
    m_Entity = m_GameEngine.GetSceneManager()->createEntity(entityNameP, "LowPolyMarine.mesh");
    m_Entity->setCastShadows(true);
    m_Entity->setQueryFlags(QueryFlags::FIGURINE_MASK);

    m_Node = m_GameEngine.GetSceneManager()->getRootSceneNode()->createChildSceneNode(nodeNameP);


    m_Node->attachObject(m_Entity);
    m_Node->setScale(m_UniformScale, m_UniformScale, m_UniformScale);

    pathfinding = new PathFindingComponent(m_GameEngine);
    AddComponent(pathfinding);

    ownerID = owner;
}

Figurines::~Figurines()
{
}

void Figurines::Update(float deltaTime)
{   
    if (m_IsSelected)
    {
        animationTime += deltaTime;

        float scale = m_UniformScale + sin(animationTime * scaleSpeed) * scaleFactor;
        float flatten = m_UniformScale + cos(animationTime * flattenSpeed) * flattenFactor;

        Ogre::Vector3 newScale(flatten, scale, flatten);
        m_Node->setScale(newScale);
    }
    else
        animationTime = 0;

    if (m_IsMoving)
        UpdatePositions(deltaTime);
}

void Figurines::UpdatePositions(float deltaTime)
{
    if (!m_MoveStraight)
    {
        if (m_IndexPosition >= pathfinding->lookPoints.size())
        {
            m_IndexPosition = 1;
            m_IsMoving = false;
            return;
        }

        Vector2 pos2D = Vector2(GetPosition().x, GetPosition().z);
        if (m_Path[m_IndexPosition - 1]->HasCrossedLine(pos2D))
        {
            m_IndexPosition++;
        }

        Vector3 targetPos = pathfinding->lookPoints[m_IndexPosition];

        LookAt(targetPos, deltaTime, turnSpeed);
    }
    else
    {
        if (GetPosition().distance(straightTargetPosition) <= 0.5f)
        {
            SetPosition(straightTargetPosition);
            m_IsMoving = false;
        }
    }

    // Translate the entity forward in its local space
    Vector3 forward = m_Node->getOrientation() * Ogre::Vector3::UNIT_Z;
    Vector3 translation = forward * deltaTime * m_MovementSpeed;
    m_Node->translate(translation);
}

void Figurines::SetPosition(Vector3 positionP)
{
    positionP.y = m_Offset.y;

    m_Node->_setDerivedPosition(positionP);
}

void Figurines::SetYawRotation(Degree rotationP)
{
    Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
    orientation.FromAngleAxis(rotationP, Ogre::Vector3::UNIT_Y);
    m_Node->_setDerivedOrientation(orientation);
}

void Figurines::OnSelected(bool isSelected)
{
    m_IsSelected = isSelected;

    if (!isSelected)
    {
        m_Node->setScale(Vector3(m_UniformScale, m_UniformScale, m_UniformScale));
    }
}

void Figurines::OnEndTurnEvent()
{
    m_CurrentMovementAction = m_MaxMovementAction;
    m_CurrentActionPoint = m_MaxActionPoint;
}

void Figurines::MoveTo(Vector3 targetPositionP)
{
    m_Path.clear();

    bool pathFound = pathfinding->FindPath(GetPosition(), targetPositionP, m_CurrentMovementAction);

    if (pathFound)
    {
        m_CurrentMovementAction -= pathfinding->totalCost;

        m_Path = pathfinding->GetTurnPath();

        /* Orient the figurine to the first point on the path */
        Vector3 targetPos = pathfinding->lookPoints[1];
        targetPos.y = 0;
        Vector3 currentPos = GetPosition();
        currentPos.y = 0;

        Vector3 direction = (targetPos - currentPos).normalisedCopy();

        Quaternion targetRotationYaw = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
        Radian yaw = targetRotationYaw.getYaw();
        Quaternion targetRotationYawOnly(yaw, Vector3::UNIT_Y);

        m_Node->_setDerivedOrientation(targetRotationYawOnly);
        m_MoveStraight = false;
        m_IsMoving = true;
    }
}

void Figurines::MoveStraight(Vector3 targetPositionP)
{
    straightTargetPosition = targetPositionP;

    /* Orient the figurine to the first point on the path */
    Vector3 targetPos = targetPositionP;
    targetPos.y = 0;
    Vector3 currentPos = GetPosition();
    currentPos.y = 0;

    float distanceFromSelected = (currentPos - targetPos).length();
    m_CurrentMovementAction -= distanceFromSelected;

    Vector3 direction = (targetPos - currentPos).normalisedCopy();

    Quaternion targetRotationYaw = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
    Radian yaw = targetRotationYaw.getYaw();
    Quaternion targetRotationYawOnly(yaw, Vector3::UNIT_Y);

    m_Node->_setDerivedOrientation(targetRotationYawOnly);
    m_MoveStraight = true;
    m_IsMoving = true;
}

void Figurines::Attack(Figurines* targetP)
{
    targetP->GetHit(1);
    m_CurrentActionPoint--;
}

void Figurines::GetHit(int damageAmountP)
{
    m_CurrentHealthPoint -= damageAmountP;

    if (m_CurrentHealthPoint <= 0)
    {
        m_IsDead = true;

        m_GameEngine.GetSceneManager()->destroyEntity(m_Entity);
        m_GameEngine.GetSceneManager()->destroySceneNode(m_Node);

        m_GameEngine.RemoveActor(this);
        std::cout << "IS DEAD";
    }
}

void Figurines::LookAt(const Ogre::Vector3& targetPosition, float deltaTime, int turnSpeed)
{
    Quaternion currentRotation = m_Node->_getDerivedOrientation();
    Vector3 direction = (targetPosition - GetPosition()).normalisedCopy();

    Quaternion targetRotationYaw = Ogre::Vector3::UNIT_Z.getRotationTo(direction);

    // Extract the yaw angle from the full rotation
    Radian yaw = targetRotationYaw.getYaw();
    Quaternion targetRotationYawOnly(yaw, Vector3::UNIT_Y);

    Quaternion myRotation = Quaternion::Slerp(deltaTime * turnSpeed, currentRotation, targetRotationYawOnly, true);

    m_Node->_setDerivedOrientation(myRotation);
}
