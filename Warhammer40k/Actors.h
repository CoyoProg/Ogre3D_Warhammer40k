#pragma once
#include "GameEngine.h"
#include <memory>

class Components;

/*
 * @brief The Actors class represents game actors with entities and components.
 * It provides methods to update the actor's state and retrieve its entity, scene node, and other properties.
 */
class Actors
{
public:
	virtual void Update(float deltaTimeP);

	SceneNode* GetSceneNode() { return mNode; }
	const Entity* GetEntity() const { return mEntity; }
	const Ogre::ManualObject* GetObject() const { return mManualObject; }

	void SetMaterial(const std::string& materialNameP) { mEntity->setMaterialName(materialNameP); }
	virtual void OnEndTurnEvent() {};

protected:
	void AddComponent(Components* componentP) { mComponents.emplace_back(componentP); }

	Entity *mEntity = nullptr;
	Ogre::ManualObject *mManualObject = nullptr;
	SceneNode *mNode = nullptr;
	std::vector<Components*> mComponents;
};