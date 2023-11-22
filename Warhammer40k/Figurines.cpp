#include "Figurines.h"
#include "QueryFlags.h"
#include "PathFindingComponent.h"

#include <OgreMath.h>

#include <iostream>


Figurines::Figurines(GameEngine& gameEngineP, std::string entityNameP, std::string nodeNameP)
{
    m_Entity = gameEngineP.GetSceneManager()->createEntity(entityNameP, "LowPolyMarine.mesh");
    m_Entity->setCastShadows(true);
    m_Entity->setQueryFlags(QueryFlags::FIGURINE_MASK);

    m_Node = gameEngineP.GetSceneManager()->getRootSceneNode()->createChildSceneNode(nodeNameP);
    m_Node->attachObject(m_Entity);
    m_Node->setScale(m_UniformScale, m_UniformScale, m_UniformScale);
    m_Node->setOrientation(Ogre::Quaternion::IDENTITY);
    m_Node->yaw(Ogre::Degree(90));

    pathfinding = new PathFindingComponent(gameEngineP);
    AddComponent(pathfinding);
}

Figurines::~Figurines()
{

}

void Figurines::Update(float deltaTime)
{   
    if (m_IsSelected)
    {
        animationTime += deltaTime;
        // Calculate scaling factor using a sin function
        float scale = m_UniformScale + sin(animationTime * scaleSpeed) * scaleFactor;

        // Calculate flattening factor using a sin function
        float flatten = m_UniformScale + cos(animationTime * flattenSpeed) * flattenFactor;

        // Apply scale and flatten to the entity
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
    if (m_IndexPosition >= pathfinding->lookPoints.size())
     {
         m_Path.clear();
         m_IndexPosition = 1;
         m_IsMoving = false;
         return;
     }

    Vector2 pos2D = Vector2(GetPosition().x, GetPosition().z);
    if (m_Path[m_IndexPosition - 1]->HasCrossedLine(pos2D))
    {
        std::cout << "CROSSED THE LINE\n";
        m_IndexPosition++;
    }

    Quaternion currentRotation = m_Node->getOrientation();

    Vector3 targetPos = pathfinding->lookPoints[m_IndexPosition];
    Vector3 direction = (targetPos - GetPosition()).normalisedCopy();

    // Create a quaternion that rotates along the Y-axis to point in the direction
    Quaternion targetRotationYaw = Ogre::Vector3::UNIT_Z.getRotationTo(direction);

    // Extract the yaw angle from the full rotation
    Radian yaw = targetRotationYaw.getYaw();

    // Create a new quaternion with only the yaw component
    Quaternion targetRotationYawOnly(yaw, Vector3::UNIT_Y);

    // Use Slerp to smoothly interpolate the current rotation to the new yaw-only rotation
    Quaternion myRotation = Quaternion::Slerp(deltaTime * 5, currentRotation, targetRotationYawOnly, true);

    // Set the new rotation
    m_Node->setOrientation(myRotation);

    // Translate the entity forward in its local space
    Vector3 forward = myRotation * Ogre::Vector3::UNIT_Z;
    Vector3 translation = forward * deltaTime * 10;
    m_Node->translate(translation);






    //Vector3 targetPosition = pathfinding->lookPoints[m_IndexPosition];
    //targetPosition.y = m_Offset.y;
    //Vector3 currentPosition = m_Node->getPosition();

    //// Calculate the direction vector
    //Vector3 direction = targetPosition - currentPosition;

    //// Calculate the distance to the target
    //float distanceToTarget = direction.length();

    //// Normalize the direction vector
    //direction.normalise();

    //// Calculate the movement for this frame
    //Vector3 movement = direction * deltaTime * m_MovementSpeed;

    //// Check if the movement overshoots the target
    //if (movement.length() > distanceToTarget)
    //{
    //    movement = direction * distanceToTarget;
    //    m_IndexPosition++;
    //}

    //// Calculate the new position
    //Vector3 newPos = currentPosition + movement;

    //SetPosition(newPos);
}

void Figurines::SetPosition(Vector3 positionP)
{
    positionP.y = m_Offset.y;

    m_Node->setPosition(positionP);
}

void Figurines::OnSelected(bool isSelected)
{
    m_IsSelected = isSelected;

    if (!isSelected)
    {
        m_Node->setScale(Vector3(m_UniformScale, m_UniformScale, m_UniformScale));
    }
}

void Figurines::MoveTo(Vector3 targetPositionP)
{
    bool pathFound = pathfinding->FindPath(GetPosition(), targetPositionP, m_MovementActionDistance);

    if (pathFound)
    {
        m_Path = pathfinding->GetTurnPath();

        m_IsMoving = true;
    }
}

void Figurines::LookAt(const Ogre::Vector3& targetPosition)
{
    // Calculate the direction vector from the node's position to the target
    Vector3 direction = (targetPosition - m_Node->getPosition()).normalisedCopy();

    // Create a rotation quaternion based on the direction vector
    Quaternion rotation = Vector3::NEGATIVE_UNIT_Z.getRotationTo(direction);

    // Set the rotation of the node
    m_Node->setOrientation(rotation);
}
