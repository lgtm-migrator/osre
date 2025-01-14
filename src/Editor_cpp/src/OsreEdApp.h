/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Modules/ModuleRegistry.h"
#include "SceneData.h"
#include <osre/App/AppBase.h>
#include <osre/App/World.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>
#include <osre/App/Camera.h>
#include <osre/App/Node.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {

namespace App {
    class Project;
    class World;
} // namespace App

namespace IO {
    class Uri;
}

namespace Editor {

class PythonInterface;
class MainRenderView;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief  The main-application-class of the OSRE-Editor.
//-------------------------------------------------------------------------------------------------
class OsreEdApp : public App::AppBase {
public:
    /// brief   The module list
    using ModulePathArray = StringArray;

    /// @brief  The class constructor with the incoming command line arguments.
    /// @param  argc    [in] The number of command-line parameters.
    /// @param  argv    [in] The command-line arguments.
    OsreEdApp(int argc, char *argv[]);

    /// @brief  The class destructor.
    ~OsreEdApp() override = default;

    /// @brief The New-project command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void newProjectCmd(ui32 cmdId, void *data);

    /// @brief The Load-project command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void loadProjectCmd(ui32 cmdId, void *data);

    /// @brief The Save-project command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void saveProjectCmd(ui32 cmdId, void *data);

    /// @brief The Import-Asset command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void importAssetCmd(ui32 cmdId, void *data);

    
    /// @brief The New-Mesh command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void newMeshCmd(ui32 cmdId, void *data);

    /// The Quit command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void quitEditorCmd(ui32 cmdId, void *data);

    /// @brief  Will load an asset from a given uri.
    /// @para   modelLoc    [in] The model location.
    void loadAsset(const IO::Uri &modelLoc);

    /// @brief  The Getting help command implementation.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void gettingHelpCmd(ui32 cmdId, void *data);

    /// @brief  The show version command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void showVersionCmd(ui32 cmdId, void *data);

    /// @brief  Will return true, if a model was loaded.
    /// @return true, if a model is stored.
    bool hasModel() const;

    /// @brief  Will return the module registry.
    /// @return The module registry.
    ModuleRegistry &getModuleRegistry();

    /// @brief Will set the status bar text.
    /// @param mode         The mode, describes the edit mode.
    /// @param Model        The model name
    /// @param numVertices  The number of vertices in the scene
    /// @param numTriangles The number of triangles in the scene.
    void setStatusBarText(const String &mode, const String &Model, i32 numVertices, i32 numTriangles);

protected:
    /// @brief The onCreate callback
    /// @return true, if successful, false in case of an error.
    bool onCreate() override;

    /// @brief The onUpdate callback.
    void onUpdate() override;

    /// @brief The onRender callback.
    void onRender() override;

    /// @brief The onDestrox callback.
    /// @return true, if successful, false in case of an error.
    bool onDestroy() override;

    /// @brief Will load a scene from a file.
    /// @param filename The filename.
    /// @param sd       The scene data container.
    /// @return true, if successful, false if not.
    bool loadSceneData(const IO::Uri &filename, SceneData &sd);

    /// @brief Will save a scene from a file.
    /// @param filename The filename.
    /// @param sd       The scene data container.
    /// @return true, if successful, false if not.
    bool saveSceneData(const IO::Uri &filename, SceneData &sd);

    /// @brief Will setup the UI.
    /// @return true, if successful, false if not.
    bool setupUserInterface();

    /// @brief  Will setup the render view.
    /// @return true, if successful, false if not.
    bool setupRenderView();

    /// @brief Will setup the python interface.
    /// @return true, if successful, false if not.
    bool setupPythonInterface();

private:
    MainRenderView *mMainRenderView;
    glm::mat4 m_model;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    SceneData mSceneData;
    App::Project *mProject;
    ModuleRegistry mModuleRegistry;
    Rect2ui mResolution;
    RenderBackend::Mesh *mMesh2D;
    PythonInterface *mPythonInterface;
    Animation::AnimationControllerBase *mTransformController;
};

inline bool OsreEdApp::hasModel() const {
    return mSceneData.m_modelNode.isValid();
}

} // namespace Editor
} // namespace OSRE
