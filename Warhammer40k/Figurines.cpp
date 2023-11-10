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
    m_Node->setScale(uniformScale, uniformScale, uniformScale);
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
        float scale = uniformScale + sin(animationTime * scaleSpeed) * scaleFactor;

        // Calculate flattening factor using a sin function
        float flatten = uniformScale + cos(animationTime * flattenSpeed) * flattenFactor;

        // Apply scale and flatten to the entity
        Ogre::Vector3 newScale(flatten, scale, flatten);
        m_Node->setScale(newScale);
    }
    else
        animationTime = 0;
}

void Figurines::SetPosition(Vector3 positionP)
{
    m_Node->setPosition(positionP);
}

void Figurines::OnSelected(bool isSelected)
{
    m_IsSelected = isSelected;

    if (!isSelected)
    {
        m_Node->setScale(Vector3(uniformScale, uniformScale, uniformScale));
    }
}
