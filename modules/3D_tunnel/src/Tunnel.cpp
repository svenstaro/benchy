#include <iostream>

#include "Tunnel.hpp"

void Tunnel::setupScene() {
	// generic scene stuff
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
	mCamera = mSceneMgr->createCamera("Camera");
	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	mSceneMgr->setShadowColour( Ogre::ColourValue(0.5, 0.5, 0.5) );
	Ogre::Entity* ent;

	// TODO: make functional
	//Ogre::CompositorManager::getSingleton().addCompositor(mViewport, "Bloom3");
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled(mViewport, "Bloom3", true); 

	// set up camera
    //mCamera->setPosition(0, 50, 0);
    //mCamera->lookAt(5000, 0, 0);
	mCamera->setNearClipDistance(5);
	mCamera->lookAt(0,0,1);

	// create skysphere
	ent = mSceneMgr->createEntity("Starfield", "starfield/starfield.mesh");
	ent->setMaterialName("BenchyMaterials/Starfield");
	Ogre::SceneNode* starnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Starfield");
	starnode->setPosition(-0, 0, 0);
	starnode->setScale(100000, 100000, 100000);
	starnode->attachObject(ent);

	Ogre::ManualObject* tunnel = mSceneMgr->createManualObject("tunnel");
	
	tunnel->begin("BenchyMaterials/Tunnel", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    tunnel->colour(0.5f, 0.5f, 0.5f, 0.5f);

	float radius = 30,
		  part_length = 50,
		  segments = 8,
		  parts = 1000;

	float alphadiff = Ogre::Math::PI / segments;

	for(unsigned int p = 0; p < parts; ++p) {
		// create one ring
		for(float alpha = 0; alpha < 2*Ogre::Math::PI; alpha += alphadiff) {

			float df = p * part_length / 500.0;
			float df2 = (p+1) * part_length / 500.0;

			float dx = cos(df) * 100;
			float dy = sin(df) * 100;
			float dx2 = cos(df2) * 100;
			float dy2 = sin(df2) * 100;

			float x = cos(alpha) * radius;
			float y = sin(alpha) * radius;
			float xn = cos(alpha + alphadiff) * radius;
			float yn = sin(alpha + alphadiff) * radius;

			Ogre::Vector3 normal1 = Ogre::Vector3(x,y,0).normalisedCopy();
			Ogre::Vector3 normal2 = Ogre::Vector3(xn,yn,0).normalisedCopy();
			
			// triangle 1
			tunnel->position(xn + dx, yn + dy, p * part_length);
			tunnel->normal(Ogre::Real(normal2.x), Ogre::Real(normal2.y), Ogre::Real(normal2.z));
			tunnel->textureCoord(Ogre::Real(0), Ogre::Real(0));
	
			tunnel->position(x + dx, y + dy, p * part_length);
			tunnel->normal(Ogre::Real(normal1.x), Ogre::Real(normal1.y), Ogre::Real(normal1.z));
			tunnel->textureCoord(Ogre::Real(0), Ogre::Real(1));
		
			tunnel->position(x + dx2, y + dy2, (p+1) * part_length);
			tunnel->normal(Ogre::Real(normal1.x), Ogre::Real(normal1.y), Ogre::Real(normal1.z));
			tunnel->textureCoord(Ogre::Real(1), Ogre::Real(1));

			// triangle 2
			tunnel->position(xn + dx2, yn + dy2, (p+1) * part_length);
			tunnel->normal(Ogre::Real(normal2.x), Ogre::Real(normal2.y), Ogre::Real(normal2.z));
			tunnel->textureCoord(Ogre::Real(1), Ogre::Real(0));
			
			tunnel->position(xn + dx, yn + dy, p * part_length);
			tunnel->normal(Ogre::Real(normal1.x), Ogre::Real(normal1.y), Ogre::Real(normal1.z));
			tunnel->textureCoord(Ogre::Real(0), Ogre::Real(0));
			
			tunnel->position(x + dx2, y + dy2, (p+1) * part_length);
			tunnel->normal(Ogre::Real(normal2.x), Ogre::Real(normal2.y), Ogre::Real(normal2.z));
			tunnel->textureCoord(Ogre::Real(1), Ogre::Real(1));
			

		}
	}
		  
	tunnel->end();

	Ogre::SceneNode *tunnelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	tunnelNode->attachObject(tunnel);

}

void Tunnel::stepScene() {
	float last_fps = mRenderWin->getLastFPS();

	// get frame time using 1/fps
	float last_frame_time = 1.0f/last_fps;

	// the first second or so, FPS will be 0 and we'd get an error
	if(last_fps == 0)
		return;

	//Ogre::Node* ground = mSceneMgr->getRootSceneNode()->getChild("ground");
	//ground->pitch(Ogre::Degree(10.0f*last_frame_time));
	Ogre::Camera* cam = mSceneMgr->getCamera("Camera");
	cam->move(Ogre::Vector3(0.f,0.f, 100*last_frame_time));
	cam->setPosition( 
			100 * cos(cam->getPosition().z / 500.f), 
			100 * sin(cam->getPosition().z / 500.f), 
			cam->getPosition().z);

	float forward = 30;
	cam->lookAt(Ogre::Vector3(
			100 * cos((forward+cam->getPosition().z) / 500.f), 
			100 * sin((forward+cam->getPosition().z) / 500.f), 
			cam->getPosition().z + forward));
}

void Tunnel::run() {
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
		if(mKeyboard->isKeyDown(OIS::KC_F11))
			takeScreenshot();
	
		// TODO: remove this
		debugStuff();

		stepScene();

		// pump those messages or the window goes starving!
	    Ogre::WindowEventUtilities::messagePump();
	}
}

