#pragma once
#include "GameEngine.h"

class Components;

class Actors
{
public:
	virtual void Update(float deltaTimeP);

	const Entity* GetEntity() const { return mEntity; }
	const Ogre::ManualObject* GetObject() const { return mManualObject; }

	SceneNode* GetSceneNode() { return mNode; }

	void SetMaterial(std::string materialNameP) { mEntity->setMaterialName(materialNameP); }
	virtual void OnEndTurnEvent();

protected:
	void AddComponent(Components* componentP) { myComponents.emplace_back(componentP); }

	Entity *mEntity = nullptr;
	Ogre::ManualObject *mManualObject = nullptr;
	SceneNode *mNode = nullptr;
	std::vector<Components*> myComponents;
};

