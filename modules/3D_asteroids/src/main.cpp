#include <Ogre.h>
#include <OIS.h>

#include "Benchmark.hpp"

void setupScene(const Ogre::Root&) {
	Ogre::SceneManager *mgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
	Ogre::Camera *cam = mgr->createCamera("Camera");
	Ogre::Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);
}

void runScene(const Ogre::Root&) {
	mRenderWin->resetStatistics();
	Ogre::uint32 frame_no = 0;

	bool running;

	while(running)
	{
		// let's render these manually for better control
		root->renderOneFrame();

		++frame_no;
		updateStats(overlay, frame_no);

		mKeyboard->capture();
		if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
			running = false;

		// pump those messages or the window goes starving!
	    Ogre::WindowEventUtilities::messagePump();
	}
}

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
	try	{
		Benchmark("ogre.log", "../../common/cfg/plugins.cfg", 
				"../../common/cfg/resources.cfg",
				"Benchy - 3D Benchmark: Asteroids") benchapp;
		setupScene();
		runScene();
    }
	catch(std::exception& e)
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
#endif
    }
    return 0;
}

