#include "TableTop.h"

TableTop::TableTop(GameEngine& gameEngineP)
{
    SceneManager& sceneManager = *gameEngineP.GetSceneManager();

    /* Import Custom mesh */
    Entity* tabletopEntity = sceneManager.createEntity("TableTopEntity", "Plateau.mesh");
    tabletopEntity->setMaterialName("Plateau.bois");

    /* Table Top */
    tabletopNode = sceneManager.getRootSceneNode()->createChildSceneNode("TableTopNode");
    tabletopNode->attachObject(tabletopEntity);
    tabletopNode->setPosition(Vector3(0, 0, 0));
    tabletopNode->yaw(Degree(90));
    tabletopNode->setScale(10, 2.5, 10);

    /* Blue carpet */
    Entity* planeEntity = sceneManager.createEntity("Plane", SceneManager::PT_PLANE);
    planeEntity->setMaterialName("Plateau.tapis");
    SceneNode* planeNode = sceneManager.getRootSceneNode()->createChildSceneNode();
    planeNode->attachObject(planeEntity);
    planeNode->setPosition(Vector3(0, -12, -100));
    planeNode->pitch(Degree(-90));
    planeNode->setScale(Vector3(6, 4, 1));
}
