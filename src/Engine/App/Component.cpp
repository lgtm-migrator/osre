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
#include <osre/App/Component.h>
#include <osre/App/Entity.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::RenderBackend;
using namespace ::CPPCore;

static const glm::vec3 Dummy = glm::vec3(-1, -1, -1);

Component::Component(Entity *owner, ui32 id, ComponentType type) :
        m_owner(owner),
        m_id(id),
        mType(type) {
    osre_assert(nullptr != owner);
}

void Component::update(Time dt) {
    onUpdate(dt);
}

void Component::render(RenderBackend::RenderBackendService *renderBackendSrv) {
    onRender(renderBackendSrv);
}

RenderComponent::RenderComponent(Entity *owner, ui32 id) :
        Component(owner, id, ComponentType::RenderComponentType), m_newGeo() {
    // empty
}

void RenderComponent::addStaticMesh(Mesh *geo) {
    if (nullptr == geo) {
        return;
    }

    m_newGeo.add(geo);
}

void RenderComponent::addStaticMeshArray(const RenderBackend::MeshArray &array) {
    if (array.isEmpty()) {
        return;
    }

    for (size_t i = 0; i < array.size(); ++i) {
        m_newGeo.add(array[i]);
    }
}

size_t RenderComponent::getNumGeometry() const {
    return m_newGeo.size();
}

Mesh *RenderComponent::getMeshAt(size_t idx) const {
    return m_newGeo[idx];
}

void RenderComponent::getMeshArray(RenderBackend::MeshArray &meshArray) {
    meshArray = m_newGeo;
}

bool RenderComponent::onPreprocess() {
    return true;
}

bool RenderComponent::onUpdate(Time) {
    return true;
}

bool RenderComponent::onRender(RenderBackendService *renderBackendSrv) {
    if (!m_newGeo.isEmpty()) {
        for (ui32 i = 0; i < m_newGeo.size(); i++) {
            renderBackendSrv->addMesh(m_newGeo[i], 0);
        }
        m_newGeo.resize(0);
    }

    return true;
}

bool RenderComponent::onPostprocess() {
    return true;
}

TransformComponent::TransformComponent(Entity *owner, ui32 id) :
        Component(owner, id, ComponentType::TransformComponentType), mNode( nullptr ) {}

void TransformComponent::setNode(Node *node) {
    mNode = node;
}

bool TransformComponent::onPreprocess() {
    return true;
}

bool TransformComponent::onUpdate( Time dt ) {
    return true;
}

bool TransformComponent::onRender(RenderBackend::RenderBackendService *rbSrv) {
    return true;
}

bool TransformComponent::onPostprocess() {
    return true;
}

LightComponent::LightComponent(Entity *owner, ui32 id) 
        : Component(owner, id, ComponentType::LightComponentType), mLight(nullptr) {}

void LightComponent::setLight(RenderBackend::Light *light) {
    mLight = light;
}

bool LightComponent::onPreprocess() {
    return true;
}

bool LightComponent::onUpdate(Time dt) {
    return true;
}

bool LightComponent::onRender(RenderBackend::RenderBackendService *rbSrv) {
    return true;
}

bool LightComponent::onPostprocess() {
    return true;
}

ScriptComponent::ScriptComponent(Entity *owner, ui32 id) 
        : Component(owner, id, ComponentType::ScriptComponentType) {}

bool ScriptComponent::onPreprocess() {
    return true;
}

bool ScriptComponent::onUpdate(Time dt) {
    return true;
}

bool ScriptComponent::onRender(RenderBackend::RenderBackendService *rbSrv) {
    return true;
}

bool ScriptComponent::onPostprocess() {
    return true;
}

} // namespace App
} // namespace OSRE
