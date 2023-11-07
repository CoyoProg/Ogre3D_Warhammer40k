#include "Actors.h"

#include "Components.h"

void Actors::Update(float deltaTime)
{
	for (auto& components : myComponents)
	{
		components->Update(deltaTime);
	}
}
