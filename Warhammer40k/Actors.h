#pragma once
#include "GameEngine.h"

class Components;

class Actors
{
public:
	virtual void Update(float deltaTime);

protected:
	void AddComponent(Components* componentP) { myComponents.emplace_back(componentP); }
	//std::vector<Components*> GetComponents() { return myComponents; }

private:
	std::vector<Components*> myComponents;
};

