/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/View.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/Model.h>
#include <osre/Assets/Model.h>
#include <osre/IO/Uri.h>
#include <osre/Platform/AbstractSurface.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Common/Ids.h>
#include <osre/Scene/GeometryBuilder.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace ::OSRE;
using namespace ::OSRE::Assets;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

// To identify local log entries 
static const String Tag = "ModelLoadingApp"; 

static const String ModelPath = "file://assets/Models/Obj/box.obj";
//static const String ModelPath = "file://assets/Models/Obj/spider.obj";

// The example application, will create the render environment and render a simple triangle onto it
class ModelLoadingApp : public App::AppBase {
    Scene::Stage *m_stage;
    Scene::View  *m_view;
    TransformMatrixBlock m_transformMatrix;

public:
    ModelLoadingApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_stage( nullptr )
    , m_view( nullptr ) {
        // empty
    }

    virtual ~ModelLoadingApp() {
        // empty
    }

protected:
    bool onCreate( Properties::Settings *settings = nullptr ) override {
        Properties::Settings *baseSettings( AppBase::getSettings() );
        if ( nullptr == baseSettings ) {
            return false;
        }
       
        baseSettings->setString( Properties::Settings::WindowsTitle, "Model Loader!" );
        if ( !AppBase::onCreate( baseSettings ) ) {
            return false;
        }

#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::registerAssetPath( "assets", "../../media" );
#else
        Assets::AssetRegistry::registerAssetPath( "assets", "../media" );
#endif 

        Ids ids;
        Assets::AssimpWrapper assimpWrapper( ids );
        IO::Uri modelLoc( ModelPath );
        if ( assimpWrapper.importAsset( modelLoc, 0 ) ) {
            Assets::Model *model = assimpWrapper.getModel();
            Collision::TAABB<f32> aabb = model->getAABB();

            m_stage = AppBase::createStage( "ModelLoader" );
            m_view = m_stage->addView("camera", nullptr );
            const f32 diam = aabb.getDiameter();
            const Vec3f center = aabb.getCenter();
            f32 zNear = 0.000001f;
			f32 zFar = 100000;//zNear + diam;
            f32 left = center.getX() - diam;
            f32 right = center.getX() + diam;
            f32 bottom = center.getY() - diam;
            f32 top = center.getY() + diam;
            Platform::AbstractSurface *rootSurface( getRootSurface() );
            if ( nullptr == rootSurface ) {
                return false;
            }
            
            const i32 w = rootSurface->getProperties()->m_width;
            const i32 h = rootSurface->getProperties()->m_height;
            f32 aspect = static_cast< f32 >( w ) / static_cast< f32 >( h );
            if ( aspect < 1.0 ) { 
                // window taller than wide 
                bottom /= aspect; 
                top /= aspect; 
            } else { 
                left *= aspect; 
                right *= aspect; 
            }
            f32 fov = 2.f;
            //m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );

            //m_view->setProjectionMode( fov, aspect, zNear, zFar );
            //m_view->setOrthoMode( left, right, bottom, top, zNear, zFar );
            glm::vec3 eye(2 * diam, 2 * diam, 0 ), up( 0, 1, 0 );
            //m_view->setLookAt( eye, glm::vec3( center.getX(), center.getY(), center.getZ() ), up );
			glm::vec3 c( glm::vec3( center.getX(), center.getY(), center.getZ() ) );
//			m_transformMatrix.m_view = glm::lookAt( eye, c, up);

        	m_transformMatrix.m_view = m_view->getView();
            //m_transformMatrix.m_projection = m_view->getProjection();
            m_transformMatrix.update();
            AppBase::getRenderBackendService()->setMatrix( "MVP", m_transformMatrix.m_mvp );

            AppBase::activateStage( m_stage->getName() );
            Scene::Node *node = m_stage->addNode( "modelNode", nullptr );
            const Model::GeoArray &geoArray = model->getGeoArray();
        	node->addModel( model );
        }

        return true;
    }
};

OSRE_MAIN( ModelLoadingApp )
