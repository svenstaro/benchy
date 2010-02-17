#include <Ogre.h>
#include <OIS.h>

void initLogmanager(const Ogre::String& log_path);
void createRoot(const Ogre::String& plugins_path);
void defineResources(const Ogre::String& res_path);
void setupRenderSystem();
void createRenderWindow();
void initResourceGroups();
void setupInputSystem();
void setupOverlay();
void updateStats(const Ogre::uint32 frame_no);
void setupScene();
void runScene();

Ogre::Root *mRoot;
Ogre::RenderWindow *mRenderWin;
OIS::InputManager *mInputManager;
OIS::Keyboard *mKeyboard;

bool running;

void initLogmanager(const Ogre::String& log_path)
{
	Ogre::LogManager* logMgr = new Ogre::LogManager();
	Ogre::Log* mLog = Ogre::LogManager::getSingleton().createLog("ogre.log", true, true, false);
}

void createRoot(const Ogre::String& plugins_path)
{
	mRoot = new Ogre::Root(plugins_path);
}

void defineResources(const Ogre::String& res_path)
{
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
	cf.load(res_path);

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}

void setupRenderSystem()
{
	// TODO: Supprt D3D as well
	Ogre::RenderSystem *rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "1024 x  768");
	rs->setConfigOption("RTT Preferred Mode", "VBO");
}

void createRenderWindow()
{
	mRenderWin = mRoot->initialise(true, "Benchy - 3D Benchmark: Dancers");
}

void initResourceGroups()
{
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void setupInputSystem()
{
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	OIS::ParamList pl;
	Ogre::RenderWindow *win = mRoot->getAutoCreatedWindow();

	win->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);

	try
	{
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
	}
	catch (const OIS::Exception &e)
	{
		throw new Ogre::Exception(42, e.eText, "Application::setupInputSystem");
	}
}

void setupOverlay()
{
	Ogre::Overlay *info_overlay = Ogre::OverlayManager::getSingleton().getByName("BenchyOverlays/InfoOverlay");

	// set name
	info_overlay->getChild("BenchyOverlayElements/InfoPanel")->getChild("BenchyOverlayElements/InfoPanelName")->setCaption("Benchy - 3D Benchmark: Dancers");
	info_overlay->show();
}

void updateStats(const Ogre::uint32 frame_no)
{
	// Temporary stuff used for gathering
	float lastFPS;
	float avgFPS;
	float bestFPS;
	float worstFPS;
	Ogre::uint32 triangle_count;
	Ogre::Overlay *info_overlay = Ogre::OverlayManager::getSingleton().getByName("BenchyOverlays/InfoOverlay");

	// Gather data
	Ogre::uint16 seconds_since_start = mRoot->getTimer()->getMilliseconds()/1000;
	mRenderWin->getStatistics(lastFPS, avgFPS, bestFPS, worstFPS);
	triangle_count = mRenderWin->getTriangleCount();

	// Set data
	Ogre::OverlayContainer* info_panel = info_overlay->getChild("BenchyOverlayElements/InfoPanel");
	info_panel->getChild("BenchyOverlayElements/InfoPanelRuntime")->
		setCaption("Elapsed: "+Ogre::StringConverter::toString(seconds_since_start));
	info_panel->getChild("BenchyOverlayElements/InfoPanelFrame")->
		setCaption("Frame: "+Ogre::StringConverter::toString(frame_no));
	info_panel->getChild("BenchyOverlayElements/InfoPanelFPS")->
		setCaption("FPS: "+Ogre::StringConverter::toString(lastFPS));
	info_panel->getChild("BenchyOverlayElements/InfoPanelAvgFPS")->
		setCaption("Avg FPS: "+Ogre::StringConverter::toString(avgFPS));
	info_panel->getChild("BenchyOverlayElements/InfoPanelBestFPS")->
		setCaption("Best FPS: "+Ogre::StringConverter::toString(bestFPS));
	info_panel->getChild("BenchyOverlayElements/InfoPanelWorstFPS")->
		setCaption("Worst FPS: "+Ogre::StringConverter::toString(worstFPS));
	info_panel->getChild("BenchyOverlayElements/InfoPanelTriangleCount")->
		setCaption("Triangle Count: "+Ogre::StringConverter::toString(triangle_count));
}

void setupScene()
{
	Ogre::SceneManager *mgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default SceneManager");
	Ogre::Camera *cam = mgr->createCamera("Camera");
	Ogre::Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);
}

void runScene()
{
	mRenderWin->resetStatistics();
	Ogre::uint32 frame_no = 0;

	while(running)
	{
		// Let's render these manually for better control
		mRoot->renderOneFrame();

		++frame_no;
		updateStats(frame_no);

		mKeyboard->capture();
		if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
			running = false;

		// Pump those messages or the window goes starving!
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
	try
	{
		running = true;
		initLogmanager("ogre.log");
		createRoot("../../common/cfg/plugins.cfg");
		defineResources("../../common/cfg/resources.cfg");
		setupRenderSystem();
		createRenderWindow();
		initResourceGroups();
		setupInputSystem();
		setupOverlay();
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

	// cleanup
	mInputManager->destroyInputObject(mKeyboard);
	OIS::InputManager::destroyInputSystem(mInputManager);
	delete mRoot;

    return 0;
}

