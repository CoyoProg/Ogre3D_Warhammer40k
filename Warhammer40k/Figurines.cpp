#include "Figurines.h"
#include "QueryFlags.h"
#include <iostream>

Figurines::Figurines(SceneManager& sceneManagerP, std::string entityNameP, std::string nodeNameP)
{
    m_Entity = sceneManagerP.createEntity(entityNameP, "LowPolyMarine.mesh");
    m_Entity->setCastShadows(true);
    m_Entity->setQueryFlags(QueryFlags::FIGURINE_MASK);

    m_Node = sceneManagerP.getRootSceneNode()->createChildSceneNode(nodeNameP);
    m_Node->attachObject(m_Entity);
    m_Node->setScale(m_UniformScale, m_UniformScale, m_UniformScale);
    m_Node->yaw(Degree(-90));
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
    if (m_IndexPosition >= m_NextPositions.size())
    {
        m_NextPositions.clear();
        m_IndexPosition = 0;
        m_IsMoving = false;
        return;
    }

    Vector3 targetPosition = m_NextPositions[m_IndexPosition];
    targetPosition.y = m_Offset.y;
    Vector3 currentPosition = m_Node->getPosition();

    // Calculate the direction vector
    Vector3 direction = targetPosition - currentPosition;

    // Calculate the distance to the target
    float distanceToTarget = direction.length();

    // Normalize the direction vector
    direction.normalise();

    // Calculate the movement for this frame
    Vector3 movement = direction * deltaTime * m_MovementSpeed;

    // Check if the movement overshoots the target
    if (movement.length() > distanceToTarget)
    {
        movement = direction * distanceToTarget;
        m_IndexPosition++;
    }

    // Calculate the new position
    Vector3 newPos = currentPosition + movement;

    SetPosition(newPos);
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
