#include "Benchmark.hpp"

Benchmark::Benchmark(const Ogre::String& log_path, const Ogre::String& plugins_path,
		  const Ogre::String& res_path, const Ogre::String& title) {
	mLogPath = log_path;
	mPluginsPath = plugins_path;
	mResPath = res_path;
	mTitle = title;
}

Benchmark::~Benchmark() {
	mInputManager->destroyInputObject(mKeyboard);
	OIS::InputManager::destroyInputSystem(mInputManager);
	delete mRoot;
}

void Benchmark::init() {
	// create log
	mLog = Ogre::LogManager::getSingleton().createLog(mLogPath, true, true, false);

	// create root
	mRoot = new Ogre::Root(mPluginsPath); // root of all evil

	// parse resources
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
	cf.load(mResPath);
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements()) {
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i) {
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	// init resources
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// setup render system
	// TODO: support D3D as well
	mRenderSys = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	mRoot->setRenderSystem(mRenderSys);
	mRenderSys->setConfigOption("Full Screen", "No");
	mRenderSys->setConfigOption("Video Mode", "1024 x  768");
	mRenderSys->setConfigOption("RTT Preferred Mode", "VBO");

	// create render window
	mRenderWin = mRoot->initialise(true, mTitle);

	// setup input system
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	OIS::ParamList pl;
	Ogre::RenderWindow *win = mRoot->getAutoCreatedWindow();

	win->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);

	try {
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->
				createInputObject(OIS::OISKeyboard, false));
	}
	catch (const OIS::Exception &e) {
		throw new Ogre::Exception(42, e.eText, "Application::setupInputSystem");
	}

	// setup overlay
	mInfoOverlay = Ogre::OverlayManager::getSingleton().
		getByName("BenchyOverlays/InfoOverlay");
	mInfoOverlay->getChild("BenchyOverlayElements/InfoPanel")->
		getChild("BenchyOverlayElements/InfoPanelName")->
			setCaption(mTitle);
	mInfoOverlay->show();
}

void Benchmark::updateStats(const Ogre::uint32 frame_no) {
	// temporary stuff used for gathering
	float lastFPS;
	float avgFPS;
	float bestFPS;
	float worstFPS;
	Ogre::uint32 triangle_count;

	// gather data
	Ogre::uint16 seconds_since_start = mRoot->getTimer()->getMilliseconds()/1000;
	mRenderWin->getStatistics(lastFPS, avgFPS, bestFPS, worstFPS);
	triangle_count = mRenderWin->getTriangleCount();

	// set data
	Ogre::OverlayContainer* info_panel = mInfoOverlay->getChild("BenchyOverlayElements/InfoPanel");
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

