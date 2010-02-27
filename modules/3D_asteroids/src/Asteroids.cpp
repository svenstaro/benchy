#include <iostream>

#include "Asteroids.hpp"

void Asteroids::setupScene() {
	// generic scene stuff
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
	mCamera = mSceneMgr->createCamera("Camera");
	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	mSceneMgr->setShadowColour( Ogre::ColourValue(0.5, 0.5, 0.5) );
	mSceneMgr->setSkyBox(true, "BenchyMaterials/Sun");
	Ogre::Entity* ent;

	// TODO: make functional
	//Ogre::CompositorManager::getSingleton().addCompositor(mViewport, "Bloom3");
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled(mViewport, "Bloom3", true); 

	// set up camera
    mCamera->setPosition(0, 50, 0);
    mCamera->lookAt(5000, 0, 0);

	// create base plane
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground",
           Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
           15000, 15000, 20, 20, true, 1, 5, 5 ,Ogre::Vector3::UNIT_Z);
	ent = mSceneMgr->createEntity("GroundEntity", "ground");
	Ogre::SceneNode* groundnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ground");
	groundnode->setPosition(0, -50, 0);
	groundnode->attachObject(ent);

	// create a sun
	Ogre::SceneNode* sunnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Sun");
	Ogre::BillboardSet* sunset= mSceneMgr->createBillboardSet("Sun");
	sunset->setMaterialName("BenchyMaterials/Sun");
	Ogre::Billboard* sunboard = sunset->createBillboard(0, 0, 0);
	sunnode->setPosition(5000, 200, 0);
	sunnode->setScale(20, 20, 20);
	sunnode->attachObject(sunset);

	// create sun light
	Ogre::Light* light = mSceneMgr->createLight("SunLight");
	light->setType(Ogre::Light::LT_POINT);
	light->setDiffuseColour(0.1, 0.1, 1.0);
	light->setSpecularColour(0.1, 0.1, 1.0);
	light->setAttenuation(100000, 1, 0.000005, 0);
	light->setCastShadows(true);
	sunnode->attachObject(light);

	// create a planet
	ent = mSceneMgr->createEntity("Planet", "planet/planet.mesh");
	ent->setMaterialName("BenchyMaterials/Planet");
	Ogre::SceneNode* planetnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Planet");
	planetnode->setPosition(1000, 0, 500);
	planetnode->setScale(10, 10, 10);
	planetnode->attachObject(ent);

	// create atmosphere
	Ogre::SceneNode* atmospherenode = planetnode->createChildSceneNode("PlanetAtmosphere");
	Ogre::BillboardSet* atmosphereset= mSceneMgr->createBillboardSet("PlanetAtmosphere");
	atmosphereset->setMaterialName("BenchyMaterials/PlanetAtmosphere");
	Ogre::Billboard* atmosphereboard = atmosphereset->createBillboard(0, 0, 0);
	atmospherenode->setScale(0.65, 0.65, 0.65);
	atmospherenode->attachObject(atmosphereset);
}

void Asteroids::stepScene() {
	float last_fps = mRenderWin->getLastFPS();

	// get frame time using 1/fps
	float last_frame_time = 1.0f/last_fps;

	// the first second or so, FPS will be 0 and we'd get an error
	if(last_fps == 0)
		return;

	//Ogre::Node* ground = mSceneMgr->getRootSceneNode()->getChild("ground");
	//ground->pitch(Ogre::Degree(10.0f*last_frame_time));
}

void Asteroids::run() {
	//mRenderWin->resetStatistics();
	Ogre::uint32 frame_no = 0;
	bool running = true;

	while(running)
	{
		// let's render these manually for better control
		mRoot->renderOneFrame();

		++frame_no;
		updateStats(frame_no);

		mKeyboard->capture();
		if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
			running = false;
	
		// TODO: remove this
		debugStuff();

		stepScene();

		// pump those messages or the window goes starving!
	    Ogre::WindowEventUtilities::messagePump();
	}
}
