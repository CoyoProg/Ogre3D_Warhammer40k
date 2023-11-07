#include "TableTop.h"

TableTop::TableTop(GameEngine& gameEngineP)
{
    SceneManager& sceneManager = *gameEngineP.GetSceneManager();

    /* Import Custom mesh */
    Ogre::Entity* tabletopEntity = sceneManager.createEntity("TableTopEntity", "Plateau.mesh");

    //tabletopEntity->getSubEntity(0)->setMaterialName("Material");
    Ogre::SceneNode* tabletopNode = sceneManager.getRootSceneNode()->createChildSceneNode("TableTopNode", Vector3(0, 0, 0));
    tabletopNode->attachObject(tabletopEntity);
    tabletopNode->setScale(10, 2.5, 10);
}
