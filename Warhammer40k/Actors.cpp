#include "Actors.h"
#include "Components.h"

void Actors::Update(float deltaTimeP)
{
	for (auto& components : mComponents)
	{
		components->Update(deltaTimeP);
	}
}