#pragma once
#include "GameEngine.h"

class Components;

/*
 * The Actors class represents game actors with entities and components.
 * It provides methods to update the actor's state, retrieve its entity and scene node amon others.
 */
class Actors
{
public:
	virtual void Update(float deltaTimeP);

	// TODO: REPLACE GETTERS WITH TEMPLATE ?
	SceneNode* GetSceneNode() { return mNode; }
	const Entity* GetEntity() const { return mEntity; }
	const Ogre::ManualObject* GetObject() const { return mManualObject; }

	void SetMaterial(std::string materialNameP) { mEntity->setMaterialName(materialNameP); }
	virtual void OnEndTurnEvent() {};

protected:
	void AddComponent(Components* componentP) { myComponents.emplace_back(componentP); }

	Entity *mEntity = nullptr;
	Ogre::ManualObject *mManualObject = nullptr;
	SceneNode *mNode = nullptr;
	std::vector<Components*> myComponents;
};

