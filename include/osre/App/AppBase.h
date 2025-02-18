/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2022 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include <osre/App/AppCommon.h>
#include <osre/App/TAbstractCtrlBase.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformCommon.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Platform/KeyTypes.h>
#include <osre/Animation/AnimatorBase.h>

#include <cppcore/Container/TQueue.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Common {
    class ArgumentParser;
    class Environment;
    class Ids;
    class CommandQueue;
}

namespace Properties {
    class Settings;
}

namespace RenderBackend {
    struct TransformMatrixBlock;
}

namespace App {

class Camera;
class Stage;
class World;
class AppBase;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT KeyboardEventListener : public Platform::OSEventListener {
public:
    KeyboardEventListener() :
            OSEventListener("App/KeyboardEventListener"),
            mLast(Platform::KEY_UNKNOWN) {
        clearKeyMap();
    }

    ~KeyboardEventListener() override {
        // empty
    }

    void onOSEvent(const Common::Event &osEvent, const Common::EventData *data) override {
        auto keyData = (Platform::KeyboardButtonEventData *)data;
        if (osEvent == Platform::KeyboardButtonDownEvent) {
            mKeymap[keyData->m_key] = 1;
            mLast = keyData->m_key;
        } else {
            mKeymap[keyData->m_key] = 0;
            mLast = Platform::KEY_UNKNOWN;
        }
    }

    bool isKeyPressed(Platform::Key key) const {
        return mKeymap[key] == 1;
    }

    Platform::Key getLastKey() const {
        return mLast; 
    }
    
    void clearKeyMap() {
        ::memset(mKeymap, 0, sizeof(char) * Platform::KEY_LAST);
    }

private:
    Platform::Key mLast;
    char mKeymap[Platform::KEY_LAST];
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class provides the basic API for managing a OSRE_base application.
/// The following features are currently provided:
/// - Create all subsystems on startup
/// - Prepares the ServiceProvider
/// - Destroy all subsystems on shutdown
/// - Managing worlds and their scene-graphs for rendering.
/// - Manages all render-pipelines for the frame-rendering.
/// - Setting up a root window
/// - Managing easy user-inputs.
/// - Hooks for creating, rendering, frame-updates and destroying. Each Hook is called on<Event> 
///   like onUpdate for the update hook. So if you want to write a bundle of updates you can derive 
///   your class from AppBase, override the onUpdate method with your own updates for the scene etc
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AppBase {
public:
    /// @brief  The class constructor.
    /// @param  argc            [in] Number of arguments from command prompt.
    /// @param  argv            [in] The argument array.
    /// @param  supportedArgs   [in] A list of supported arguments, separated by a :
    /// @param  desc            [in] The description for the arguments.
    AppBase(i32 argc, const c8 *argv[], const String &supportedArgs = "api:run_dll:break_on_startup", 
        const String &desc = "The render API:The module to run:Breakpoint of startup");

    /// @brief  The class destructor, virtual.
    virtual ~AppBase();

    /// @brief  Will initialize a render window, the whole render engine will be started.
    /// @param  x           [in] The left position of the render windows for windowed mode.
    /// @param  y           [in] The top position of the render windows for windowed mode.
    /// @param  width       [in] The width of the window.
    /// @param  height      [in] The height of the render window.
    /// @param  title       [in] The Windows title.
    /// @param  fullscreen  [in] true for fullscreen mode, false for windowed mode.
    /// @param  renderer    [in] The requested render mode.
    /// @return true, if the window was generated.
    virtual bool initWindow( ui32 x, ui32 y, ui32 width, ui32 height, const String &title, 
        bool fullscreen, RenderBackendType renderer );

    /// @brief  Creates the application.
    /// @param  settings         [in] The user-defined settings.
    /// @return true, if successful, false in case of an error.
    virtual bool create( Properties::Settings *settings = nullptr );

    /// @brief  Destroys the application.
    /// @return true, if successful, false in case of an error.
    virtual bool destroy();

    /// @brief  Updates the current frame.
    virtual void update();

    /// @brief  Will perform a resize for the root window.
    /// @param  x   [in] X-coordinate of origin.
    /// @param  y   [in] Y-coordinate of origin.
    /// @param  w   [in] The width for the window.
    /// @param  h   [in] The height for the window.
    virtual void resize(i32 x, i32 y, i32 w, i32 h);

    /// @brief  Requests a new frame.
    virtual void requestNextFrame();

    /// @brief  Handles all events.
    /// @return true, if successful, false in case of an error.
    virtual bool handleEvents();

    /// @brief  Returns the current global settings.
    /// @return The global settings.
    virtual Properties::Settings *getSettings() const;

    /// @brief Will return the current stage or nullptr if none is active.
    /// @return The current stage or nullptr.
    virtual Stage *getStage() const;

    /// @brief  Will activate the given view in the active world instance.
    /// @param  view        [in] A pointer showing to the view.
    /// @return true, if the view activation was successful, false if not.
    virtual Camera *setActiveCamera(Camera *view);

    /// @brief  Will requested a shutdown.
    virtual void requestShutdown();
    
    /// @brief  Will return true, if a shutdown is requested by the user.
    /// @return true, if a shutdown is requested.
    virtual bool shutdownRequested() const;
        
    /// @brief  Will return the active time or nullptr, if none is active.
    /// @return Pointer showing to the active timer.
    virtual Platform::AbstractTimer *getActiveTimer() const;
    
    /// @brief  Will return the render-backend-service instance.
    /// @return A pointer showing to the render-backend service instance.
    virtual RenderBackend::RenderBackendService *getRenderBackendService() const;

    /// @brief  Will return the Root-Surface instance.
    /// @return A pointer showing to the Root-Surface.
    virtual Platform::AbstractWindow *getRootWindow() const;

    /// @brief  Will set the windows tile of the main engine window.
    /// @param  title       [in] The new windows title.
    virtual void setWindowsTitle( const String &title );

    /// @brief  Will return the id-container of the application
    /// @return The id container.
    virtual Common::Ids *getIdContainer() const;

    /// @brief  Will create the default pipeline for rendering.
    /// @param  rbService   The renderbackend service instance.
    /// @return The default pipeline.

    /// @brief  Checks if the key was pressed in this frame.
    /// @param  key     [in] The key-code to look for.
    /// @return true if the key was pressed, false if not.
    virtual bool isKeyPressed(Platform::Key key) const;

    /// @brief  Will return the resolution of the root render surface.
    /// @param  w       [out] The width.
    /// @param  height  [out] The width.
    virtual void getResolution(ui32 &w, ui32 &h);

    /// @brief  Will return the default transform controller.
    /// @param  type    [in] The requested controller type.
    /// @param  tmb     [in] The controlled transform block.
    /// @return The transform controller or nullptr if none is there.
    virtual Animation::AnimationControllerBase *getTransformController(RenderBackend::TransformMatrixBlock &tmb);

    ///	@brief
    /// @return
    virtual MouseEventListener *getMouseEventListener() const;

    ///	@brief
    /// @return
    virtual KeyboardEventListener *getKeyboardEventListener() const;

 protected:
    /// @brief  The onCreate callback, override this for your own creation stuff.
    /// @return true if successful,  false if not.
    virtual bool onCreate();

    /// @brief  The onDestroy callback, override this for your own destroying stuff.
    /// @return true if successful,  false if not.
    virtual bool onDestroy();

    /// @brief  The onRener callback.
    virtual void onRender();

    /// @brief  The onUpdate callback, override this for your own update stuff.
    virtual void onUpdate();

    /// @brief  Argument parser getter.
    /// @return The argument parser.
    const Common::ArgumentParser &getArgumentParser() const;

private:
    enum class State {
        Uninited,
        Created,
        Running,
        Destroyed
    };

    State mAppState;
    Common::ArgumentParser m_argParser;
    Common::Environment* m_environment;
    Properties::Settings *m_settings;
    Platform::PlatformInterface *m_platformInterface;
    Platform::AbstractTimer *m_timer;
    RenderBackend::RenderBackendService *m_rbService;
    CPPCore::TArray<World*> m_worlds;
    Stage *mStage;
    MouseEventListener *m_mouseEvListener;
    KeyboardEventListener *m_keyboardEvListener;
    Common::Ids *m_ids;
    bool m_shutdownRequested;
};

inline Stage *AppBase::getStage() const {
    return mStage;
}

inline MouseEventListener *AppBase::getMouseEventListener() const {
    return m_mouseEvListener;
}

inline KeyboardEventListener *AppBase::getKeyboardEventListener() const {
    return m_keyboardEvListener;
}

//-------------------------------------------------------------------------------------------------
///	@brief  Shortcut to get a OSRE-main function.
/// To use this you need to derive your application class from the ::OSRE::App::AppBase class and
/// put this into your main source file:
/// @code
///
/// class MyApp : public ::OSRE::App::AppBase { ... };
///
/// OSRE_MAIN(MyApp);
///
/// @endcode
//-------------------------------------------------------------------------------------------------
#define OSRE_MAIN( APPNAME )          \
int main( int argc, char *argv[] )  { \
    APPNAME myApp( argc, argv );      \
    if ( !myApp.create() ) {          \
        return 1;                     \
    }                                 \
    while ( myApp.handleEvents() ) {  \
        myApp.update();               \
        myApp.requestNextFrame();     \
    }                                 \
    myApp.destroy();                  \
                                      \
    return 0;                         \
}

} // Namespace App
} // Namespace OSRE
