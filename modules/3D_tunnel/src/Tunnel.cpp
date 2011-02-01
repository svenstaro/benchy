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
		  part_length = 32,
		  segments = 6,
		  parts = 1000;

	float alphadiff = 2 * Ogre::Math::PI / segments;

	for(unsigned int p = 0; p < parts; ++p) {
		// create one ring
		float df = p * part_length;
		float df2 = (p+1) * part_length;

		float dx = GetDisplacement(df).x;
		float dy = GetDisplacement(df).y;
		float dx2 = GetDisplacement(df2).x;
		float dy2 = GetDisplacement(df2).y;


		for(float alpha = 0; alpha < 2*Ogre::Math::PI; alpha += alphadiff) {

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
	
	if(last_frame_time > 0.1) last_frame_time = 0.1;
	Ogre::Camera* cam = mSceneMgr->getCamera("Camera");
	cam->move(Ogre::Vector3(0.f,0.f, 200*last_frame_time));
	float z = cam->getPosition().z;
	cam->setPosition( 
			GetDisplacement(z).x,
			GetDisplacement(z).y,
			z);

	z += 30;
	cam->lookAt( 
			GetDisplacement(z).x,
			GetDisplacement(z).y,
			z);

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

Ogre::Vector3 Tunnel::GetDisplacement(float z) {
	z /= 20000;
	noise::module::Perlin perlin;
	return Ogre::Vector3(
			perlin.GetValue(z,0,0) * 5000,
			perlin.GetValue(0,z,0) * 5000,
			0);
}
