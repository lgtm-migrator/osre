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

#include <osre/App/AssetsCommon.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Animation/AnimatorBase.h>
#include <osre/Common/Ids.h>
#include <osre/Common/TAABB.h>

#include <cppcore/Container/TArray.h>

#include <map>

// Forward declarations ---------------------------------------------------------------------------
struct aiScene;
struct aiMesh;
struct aiNode;
struct aiMaterial;
struct aiAnimation;
struct aiSkeleton;

namespace OSRE {

namespace Common {
    class Ids;
}

namespace RenderBackend {
    class Mesh;
    class Material;

    struct UniformVar;
}

namespace IO {
    class Uri;
}
    
namespace App {

class Entity;
class World;
class Node;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief  This class will perform an import of model assets. 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AssimpWrapper {
public:
    using MaterialArray = CPPCore::TArray<RenderBackend::Material *>;
    using Bone2NodeMap = std::map<const char *, const aiNode *>;
    using AnimationMap = std::map<const char*, Animation::AnimationTrack*>;

    /// @brief The class constructor.
    /// @param ids      The id container.
    /// @param world    The world to put the imported entity in.
    AssimpWrapper(Common::Ids &ids, World *world);

    ///	@brief  The class destructor.
    ~AssimpWrapper();

    /// @brief Will perform the import operation.
    /// @param file     The file to load.
    /// @param flags    The flags for the import.
    /// @return true, if successful. false if not.
    bool importAsset( const IO::Uri &file, ui32 flags );

    /// @brief  Will return the imported entity.
    /// @return The imported entity, nullptr if nothing was imported.
    Entity *getEntity() const;

    /// @brief Will return the model statistics.
    /// @param numVertices      The number of vertices in the model.
    /// @param numTriangles     The number of triangles in the model.
    void getStatistics(ui32 &numVertices, ui32 &numTriangles);

protected:
    Entity *convertScene();
    void importMeshes( aiMesh **meshes, ui32 numMeshes );
	//void importBones(aiMesh* mesh);
    void importNode( aiNode *node, Node *parent );
    void importMaterial( aiMaterial *material );
    void importSkeletons(aiSkeleton *skeletons, size_t numSkeletons);
    void importAnimation(aiAnimation *animation, Animation::AnimationTrack &currentAnimationTrack, AnimationMap &animLookup);
    void optimizeVertexBuffer();

private:
    struct AssetContext {
        const aiScene *mScene;
        RenderBackend::MeshArray mMeshArray;
        RenderBackend::Texture *mDefaultTexture;
        Entity *mEntity;
        World *mWorld;
        MaterialArray mMatArray;
        App::Node *mParentNode;
        Common::Ids &mIds;
        String mRoot;
        String mAbsPathWithFile;
        Bone2NodeMap mBone2NodeMap;
        ui32 mNumVertices;
        ui32 mNumTriangles;
        AssetContext(Common::Ids &ids, World *world);
        ~AssetContext();

        AssetContext(const AssetContext &) = delete;
        AssetContext &operator=(const AssetContext&) = delete;
    };

    AssetContext mAssetContext;
};

} // Namespace Assets
} // Namespace OSRE
