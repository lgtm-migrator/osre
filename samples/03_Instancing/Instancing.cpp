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
#include <osre/App/App.h>
#include <osre/Common/Ids.h>
#include <osre/IO/Uri.h>
#include <osre/Common/BaseMath.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Properties/Settings.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/Scene.h>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

// To identify local log entries
static const c8 *Tag = "InstancingApp";

/// The example application, will create the render environment and render a simple triangle onto it
class InstancingApp : public App::AppBase {
    App::Entity *mEntity;
    App::Camera *mCamera;

public:
    InstancingApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv, "api:model", "The render API:The model to load"),
            mEntity(nullptr),
            mCamera(nullptr) {
        // empty
    }

    virtual ~InstancingApp() {
        // empty
    }

protected:
    bool onCreate() override {
        if (!AppBase::onCreate()) {
            osre_debug(Tag, "Error while creating.");
            return false;
        }
        AppBase::setWindowsTitle("Instancing sample!");

#ifdef OSRE_WINDOWS
        AssetRegistry::registerAssetPath("assets", "../../assets");
#else
        AssetRegistry::registerAssetPath("assets", "../assets");
#endif

        RenderBackendService *rbSrv = getRenderBackendService();
        if (nullptr == rbSrv) {
            return false;
        }

        Platform::AbstractWindow *rootWindow = getRootWindow();
        if (nullptr == rootWindow) {
            return false;
        }

        Rect2ui windowsRect;
        rootWindow->getWindowsRect(windowsRect);

        Camera *camera = getStage()->getActiveWorld()->addCamera("cam1");
        camera->setProjectionParameters(60.f, (f32)windowsRect.width, (f32)windowsRect.height, 0.0001f, 1000.f);
        World *world = getStage()->getActiveWorld();
        mEntity = new App::Entity("instance", world->getIds(), world);
        MeshBuilder meshBuilder;
        AppBase::getStage()->getActiveWorld()->addEntity(mEntity);
        RenderBackend::Mesh *mesh = meshBuilder.createTriangle(VertexType::RenderVertex, BufferAccessType::ReadWrite).getMesh();
        if (nullptr != mesh) {
            RenderComponent *rc = (RenderComponent *)mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);
            Time dt;
            world->update(dt);
            camera->observeBoundingBox(mEntity->getAABB());
        }

        return true;
    }

    void onUpdate() override {
        // Rotate the model
        glm::mat4 rot(1.0);

        RenderBackendService *rbSrv(getRenderBackendService());

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        // Scene::DbgRenderer::getInstance()->renderDbgText(0, 0, 2U, "XXX");

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    InstancingApp myApp(argc, argv);
    if (!myApp.initWindow(10, 10, 1024, 768, "ModelLoader-Sample", false, App::RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }

    myApp.destroy();

    return 0;
}
