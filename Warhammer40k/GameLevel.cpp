#include "GameLevel.h"

#include "Ogre.h"

#include "GameEngine.h"
#include "Actors.h"
#include "Player.h"
#include "TableTop.h"

using namespace Ogre;

void GameLevel::LoadLevel(GameEngine& gameEngineP)
{
    /* Player need to be initialize first for the main camera */
    // ===== Player ==== 
    Player* player = new Player(gameEngineP);
    gameEngineP.AddActor(player);

    TableTop* tabletop = new TableTop(gameEngineP);
    gameEngineP.AddActor(tabletop);

    SceneManager& sceneManager = *gameEngineP.GetSceneManager();

    // ===== LIGHT ==== 
   /* Create the light */
    Light* light = sceneManager.createLight("MainLight");

    /* Create a SceneNode for the light, and attach the new light */
    SceneNode* lightNode = sceneManager.getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setPosition(20, 80, 50);

    LoadTuto(sceneManager);

    /* SkyBox */
    sceneManager.setSkyBox(true, "Examples/CloudyNoonSkyBox");                        // SkyBox
    //sceneManager.setSkyDome(true, "Examples/CloudySky", 5, 8);                                       // SkyDome

    /* Fog */
    Ogre::Viewport* mainViewport = sceneManager.getCamera("mainCamera")->getViewport();

    // Set the background color for the main viewport
    Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);
    mainViewport->setBackgroundColour(fadeColour);
    sceneManager.setFog(Ogre::FOG_LINEAR, fadeColour, 0, 2000, 10000); // Linear
    //sceneManagerP.setFog(Ogre::FOG_EXP, fadeColour, 0.002);                                            // Exponential
    //sceneManagerP.setFog(Ogre::FOG_EXP2, fadeColour, 0.002);                                           // Exponential
}

void GameLevel::LoadTuto(Ogre::SceneManager& sceneManagerP)
{
    // ===== OGRES ==== 
    /* Create the Ogres */
    Entity* ogreEntity = sceneManagerP.createEntity("ogrehead.mesh");
    Entity* ogreEntity2 = sceneManagerP.createEntity("ogrehead.mesh");
    Entity* ogreEntity3 = sceneManagerP.createEntity("ogrehead.mesh");
    Entity* ogreEntity4 = sceneManagerP.createEntity("ogrehead.mesh");

    /* Create a scene node for the ogres */
    SceneNode* ogreNode = sceneManagerP.getRootSceneNode()->createChildSceneNode();
    SceneNode* ogreNode2 = sceneManagerP.getRootSceneNode()->createChildSceneNode(Vector3(84, 48, 0));
    SceneNode* ogreNode3 = sceneManagerP.getRootSceneNode()->createChildSceneNode(Vector3(0, 104, 0));
    SceneNode* ogreNode4 = sceneManagerP.getRootSceneNode()->createChildSceneNode(Vector3(-84, 48, 0));
    ogreNode->attachObject(ogreEntity);
    ogreNode2->attachObject(ogreEntity2);
    ogreNode3->attachObject(ogreEntity3);
    ogreNode4->attachObject(ogreEntity4);

    ogreNode3->setScale(2, 1.2, 1);
    ogreNode2->yaw(Degree(90));
    ogreNode4->yaw(Degree(-90));
}
