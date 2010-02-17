#include <Ogre.h>
#include <OIS/OIS.h>

using namespace Ogre;

class ExitListener : public FrameListener
{
public:
    ExitListener(OIS::Keyboard *keyboard, Camera* cam, SceneManager* mgr)
        : mKeyboard(keyboard)
    {
    }

    bool frameStarted(const FrameEvent& evt)
    {
		cam->rotate(mgr->getRootSceneNode()->getPosition(), 0.1*evt.timeSinceLastFrame);

        mKeyboard->capture();
        return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
    }

private:
    OIS::Keyboard *mKeyboard;
};

class Application
{
public:
    void go()
    {
        createRoot();
        defineResources();
        setupRenderSystem();
        createRenderWindow();
        initializeResourceGroups();
        setupScene();
        setupInputSystem();
        createFrameListener();
        startRenderLoop();
    }

    ~Application()
    {
        mInputManager->destroyInputObject(mKeyboard);
        OIS::InputManager::destroyInputSystem(mInputManager);

        delete mListener;
        delete mRoot;
    }

private:
    Root *mRoot;
    OIS::Keyboard *mKeyboard;
    OIS::InputManager *mInputManager;
    ExitListener *mListener;

    void createRoot()
    {
        mRoot = new Root();
    }
    
    void defineResources()
    {
        String secName, typeName, archName;
        ConfigFile cf;
        cf.load("resources.cfg");

        ConfigFile::SectionIterator seci = cf.getSectionIterator();
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap *settings = seci.getNext();
            ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }
    }
    
    void setupRenderSystem()
    {
        if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
            throw Exception(52, "User canceled the config dialog!", "Application::setupRenderSystem()");

        //// Do not add this to the application
        //RenderSystem *rs = mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
        //                                      // or use "OpenGL Rendering Subsystem"
        //mRoot->setRenderSystem(rs);
        //rs->setConfigOption("Full Screen", "No");
        //rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");
    }
    
    void createRenderWindow()
    {
        mRoot->initialise(true, "Tutorial Render Window");

        //// Do not add this to the application
        //mRoot->initialise(false);
        //HWND hWnd = 0;  // Get the hWnd of the application!
        //NameValuePairList misc;
        //misc["externalWindowHandle"] = StringConverter::toString((int)hWnd);
        //RenderWindow *win = mRoot->createRenderWindow("Main RenderWindow", 800, 600, false, &misc);
    }

    void initializeResourceGroups()
    {
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

    void setupScene()
    {
		SceneNode *teanode;
        SceneManager *mgr = mRoot->createSceneManager(ST_GENERIC, "Default SceneManager");
        Camera *cam = mgr->createCamera("Camera");
        Viewport *vp = mRoot->getAutoCreatedWindow()->addViewport(cam);

		vp->setBackgroundColour( ColourValue( 0.1, 0.1, 0.1 ) );
		mgr->setAmbientLight( ColourValue( 1, 1, 1 ) );

		Entity *ent1 = mgr->createEntity( "Teatank", "teatank.mesh" );
		teanode = mgr->getRootSceneNode()->createChildSceneNode("TeaNode");
		teanode->attachObject(ent1);
		teanode->setScale(3.0f, 3.0f, 3.0f);

		Entity *ent2 = mgr->createEntity( "Robot", "robot.mesh" );
		mgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent2);

        cam->setPosition(Vector3(0,10,500));
		cam->lookAt(mgr->getRootSceneNode()->getPosition());
    }

    void setupInputSystem()
    {
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        OIS::ParamList pl;
        RenderWindow *win = mRoot->getAutoCreatedWindow();

        win->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        mInputManager = OIS::InputManager::createInputSystem(pl);

        try
        {
            mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));
            //mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));
            //mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, false));
        }
        catch (const OIS::Exception &e)
        {
            throw new Exception(42, e.eText, "Application::setupInputSystem");
        }
    }

    void createFrameListener()
    {
        mListener = new ExitListener(mKeyboard, cam, mgr);
        mRoot->addFrameListener(mListener);
    }

    void startRenderLoop()
    {
        mRoot->startRendering();

        //// Do not add this to the application
        //while (mRoot->renderOneFrame())
        //{
        //    // Do some things here, like sleep for x milliseconds or perform other actions.
        //}
    }
};

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
        Application app;
        app.go();
    }
    catch(Exception& e)
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
    }

    return 0;
}
