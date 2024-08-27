#pragma once
#include "GameEngine.h"

class Components;

class Actors
{
public:
	virtual void Update(float deltaTime);

	const Entity* GetEntity() const { return m_Entity; }
	const Ogre::ManualObject* GetObject() const { return m_ManualObject; }

	SceneNode* GetSceneNode() { return m_Node; }

	void SetMaterial(std::string materialName) { m_Entity->setMaterialName(materialName); }
	virtual void OnEndTurnEvent();

protected:
	void AddComponent(Components* componentP) { myComponents.emplace_back(componentP); }

	Entity* m_Entity{ nullptr };
	Ogre::ManualObject* m_ManualObject{ nullptr };
	SceneNode* m_Node{ nullptr };
	std::vector<Components*> myComponents;
};

