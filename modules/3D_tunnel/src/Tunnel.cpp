#include <iostream>

#include "Tunnel.hpp"

Tunnel::Tunnel(float radius, float segments, float parts, float part_length, float speed) {
	mRadius = radius;
	mSegments = segments;
	mParts = parts;
	mPartLength = part_length;
	mSpeed = speed;
}

void Tunnel::setupScene() {
	// generic scene stuff
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
	mCamera = mSceneMgr->createCamera("Camera");
	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	//mSceneMgr->setShadowColour( Ogre::ColourValue(0.5, 0.5, 0.5) );

	Ogre::ColourValue fadeColour(0, 0, 0);
	mViewport->setBackgroundColour(fadeColour);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 50, 500);

	// TODO: make functional
	//Ogre::CompositorManager::getSingleton().addCompositor(mViewport, "Bloom3");
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled(mViewport, "Bloom3", true); 

	// set up camera
	Ogre::SceneNode* camnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("camnode");
	mCamera->setNearClipDistance(5);
	camnode->attachObject(mCamera);

	// create skysphere
	Ogre::Entity* ent = mSceneMgr->createEntity("Starfield", "starfield/starfield.mesh");
	ent->setMaterialName("BenchyMaterials/Starfield");
	Ogre::SceneNode* starnode = camnode->createChildSceneNode("Starfield");
	starnode->setPosition(-0, 0, 0);
	starnode->setScale(100000, 100000, 100000);
	starnode->attachObject(ent);

	mTunnelObject = mSceneMgr->createManualObject("tunnel");
	Ogre::SceneNode *tunnelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("tunnelnode");
	tunnelNode->attachObject(mTunnelObject);

	GenerateTunnel();

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

	Ogre::SceneNode* cam = mSceneMgr->getSceneNode("camnode");
	cam->translate(Ogre::Vector3(0.f,0.f, mSpeed*last_frame_time));
	float z = cam->getPosition().z;
	cam->setPosition(
			GetDisplacement(z).x,
			GetDisplacement(z).y,
			z);

	z += 30;
	cam->lookAt( Ogre::Vector3(
				GetDisplacement(z).x,
				GetDisplacement(z).y,
				z), Ogre::Node::TS_WORLD);

	GenerateTunnel(round(z / mPartLength) - 10);

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

void Tunnel::GenerateTunnel(int first_part) {
	mTunnelObject->clear();
	mTunnelObject->begin("BenchyMaterials/Tunnel", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	mTunnelObject->colour(0.5f, 0.5f, 0.5f, 0.5f);

	float alphadiff = 2 * Ogre::Math::PI / mSegments;
	for(unsigned int p = 0; p < mParts; ++p) {
		for(float alpha = 0; alpha < 2*Ogre::Math::PI; alpha += alphadiff) {
			AddQuad(mTunnelObject, alpha, p + first_part);
		}
	}
	mTunnelObject->end();
}

void Tunnel::AddQuad(Ogre::ManualObject* o, float alpha, int part) {
	float alphadiff = 2 * Ogre::Math::PI / mSegments;
	float df = part * mPartLength;
	float df2 = (part+1) * mPartLength;

	float dx = GetDisplacement(df).x;
	float dy = GetDisplacement(df).y;
	float dx2 = GetDisplacement(df2).x;
	float dy2 = GetDisplacement(df2).y;

	float x = cos(alpha) * mRadius;
	float y = sin(alpha) * mRadius;
	float xn = cos(alpha + alphadiff) * mRadius;
	float yn = sin(alpha + alphadiff) * mRadius;

	Ogre::Vector3 normal1 = -Ogre::Vector3(x,y,0).normalisedCopy();
	Ogre::Vector3 normal2 = -Ogre::Vector3(xn,yn,0).normalisedCopy();

	// triangle 1
	o->position(xn + dx, yn + dy, part * mPartLength);
	o->normal(Ogre::Real(normal2.x), Ogre::Real(normal2.y), Ogre::Real(normal2.z));
	o->textureCoord(Ogre::Real(0), Ogre::Real(0));

	o->position(x + dx, y + dy, part * mPartLength);
	o->normal(Ogre::Real(normal1.x), Ogre::Real(normal1.y), Ogre::Real(normal1.z));
	o->textureCoord(Ogre::Real(0), Ogre::Real(1));

	o->position(x + dx2, y + dy2, (part+1) * mPartLength);
	o->normal(Ogre::Real(normal1.x), Ogre::Real(normal1.y), Ogre::Real(normal1.z));
	o->textureCoord(Ogre::Real(1), Ogre::Real(1));

	// triangle 2
	o->position(xn + dx2, yn + dy2, (part+1) * mPartLength);
	o->normal(Ogre::Real(normal2.x), Ogre::Real(normal2.y), Ogre::Real(normal2.z));
	o->textureCoord(Ogre::Real(1), Ogre::Real(0));

	o->position(xn + dx, yn + dy, part * mPartLength);
	o->normal(Ogre::Real(normal1.x), Ogre::Real(normal1.y), Ogre::Real(normal1.z));
	o->textureCoord(Ogre::Real(0), Ogre::Real(0));

	o->position(x + dx2, y + dy2, (part+1) * mPartLength);
	o->normal(Ogre::Real(normal2.x), Ogre::Real(normal2.y), Ogre::Real(normal2.z));
	o->textureCoord(Ogre::Real(1), Ogre::Real(1));
}


Ogre::Vector3 Tunnel::GetDisplacement(float z) {
	z /= 20000;
	noise::module::Perlin perlin;
	return Ogre::Vector3(
			perlin.GetValue(z,0,0) * 5000,
			perlin.GetValue(0,z,0) * 5000,
			0);
}
