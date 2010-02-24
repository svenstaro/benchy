#ifndef BENCHMARK_HPP_INCLUDED
#define BENCHMARK_HPP_INCLUDED

#include <Ogre.h>
#include <OIS.h>

class Benchmark {
public:
	void init(const Ogre::String& log_path, const Ogre::String& plugins_path,
			  const Ogre::String& res_path, const Ogre::String& title);
	virtual void setupScene() = 0;
	virtual void run() = 0;
	void updateStats(const Ogre::uint32);

protected:
	Benchmark();
	virtual ~Benchmark();

	Ogre::String mLogPath;
	Ogre::String mPluginsPath;
	Ogre::String mResPath;
	Ogre::String mTitle;

	Ogre::Log* mLog;
	Ogre::Root* mRoot;
	Ogre::RenderSystem* mRenderSys;
	Ogre::RenderWindow* mRenderWin;
	Ogre::Overlay* mInfoOverlay;
	OIS::InputManager* mInputManager;
	OIS::Keyboard* mKeyboard;
};
#endif 
