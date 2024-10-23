#include "Actors.h"
#include "Components.h"

void Actors::Update(float deltaTimeP)
{
	for (auto& components : myComponents)
	{
		components->Update(deltaTimeP);
	}
}