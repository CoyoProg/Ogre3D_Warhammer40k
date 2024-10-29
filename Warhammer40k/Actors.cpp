#include "Actors.h"
#include "Components.h"

Actors::~Actors()
{
}

void Actors::Update(float deltaTimeP)
{
	for (auto& components : mComponents)
	{
		components->Update(deltaTimeP);
	}
}