#include "Asteroids.hpp"

void Asteroids::setupScene() {
	Ogre::SceneManager *mgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
	Ogre::Camera *cam = mgr->createCamera("Camera");
	Ogre::Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);
}

void Asteroids::run() {
	mRenderWin->resetStatistics();
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

		// pump those messages or the window goes starving!
	    Ogre::WindowEventUtilities::messagePump();
	}
}
