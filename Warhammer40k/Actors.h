#pragma once
#include "GameEngine.h"

class Components;

class Actors
{
public:
	virtual void Update(float deltaTime);

	const Entity* GetEntity() const { return m_Entity; }
	SceneNode* GetSceneNode() { return m_Node; }

protected:
	void AddComponent(Components* componentP) { myComponents.emplace_back(componentP); }
	//std::vector<Components*> GetComponents() { return myComponents; }

	Entity* m_Entity{ nullptr };
	SceneNode* m_Node{ nullptr };
	std::vector<Components*> myComponents;
};

