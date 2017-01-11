/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/Parameter.h>
#include <osre/IO/Uri.h>

#include <cppcore/Container/TArray.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OSRE {
namespace RenderBackend {

struct Parameter;

static const i32  UnsetHandle = -1;
static const ui32 MaxEntNameLen = 256;

///	@brief  This enum describes the usage of a buffer object.
enum class BufferType {
    EmptyBuffer,    ///< Empty buffer, no special use.
    VertexBuffer,   ///< Vertex buffer, stores vertex data inside.
    IndexBuffer,    ///< Index buffer, stores indices inside.
    InstanceBuffer  ///< Instance buffer, will store instance-specific data.
};

/// @brief  This enum describes the supported access types for render buffers.
enum class BufferAccessType {
    ReadOnly,       ///< Read only access.
    WriteOnly,      ///< Write only access.
    ReadWrite       ///< Read and write access.
};

///	@brief
enum class VertexType {
    ColorVertex,
    RenderVertex
};

///	@brief
enum class TextureTargetType {
    Texture1D,
    Texture2D,
    Texture3D
};

///	@brief
enum class TextureStageType {
    TextureStage0 = 0,
    TextureStage1,
    TextureStage2,
    TextureStage3
};

///	@brief
enum class TextureParameterName {
    TextureParamMinFilter = 0,
    TextureParamMagFilter,
    TextureParamWrapS,
    TextureParamWrapT
};

///	@brief
enum class TextureParameterType {
    TexturePTNearest = 0,
    TexturePTLinear,
    TexturePTClamp,
    TexturePTMirroredRepeat,
    TexturePTRepeat
};

///	@brief
enum class IndexType {
    UnsignedByte,   ///<
    UnsignedShort   ///<
};

///	@brief
enum class PrimitiveType {
    PointList,		///< A list of points, one index per point.
    LineList,		///< A list of separate lines, 2 indices per line.
    LineStrip,		///< A line strip, Start and end-index and all indices between.
    TriangleList,	///< A list of triangles, 3 indices per triangle.
    TriangelStrip,	///< A strip of triangles
    TriangleFan		///< A triangle fan.
};

///	@brief
struct OSRE_EXPORT ColorVert {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color0;

    static ui32 getNumAttributes();
    static const String *getAttributes();
};

///	@brief
struct RenderVert {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color0;
    glm::vec2 tex0;

    static ui32 getNumAttributes(); 
    static const String *getAttributes();
};

///	@brief
enum class VertexAttribute : int {
    Position = 0,   ///< "position"
    Normal,         ///< "normal"
    TexCoord0,      ///< "texcoord0"
    TexCoord1,      ///< "texcoord1"
    TexCoord2,      ///< "texcoord2"
    TexCoord3,      ///< "texcoord3"
    Tangent,        ///< "tangent
    Binormal,       ///< "binormal"
    Weights,        ///< "weights" (skin weights)
    Indices,        ///< "indices" (skin indices)
    Color0,         ///< "color0"
    Color1,         ///< "color1"
    Instance0,      ///< "instance0"
    Instance1,      ///< "instance1"
    Instance2,      ///< "instance2"
    Instance3,      ///< "instance3"
    NumVertexAttrs,
    InvalidVertexAttr
};

OSRE_EXPORT const String &getVertCompName( VertexAttribute attrib );

///	@brief
enum class VertexFormat : int {
    Float,          ///< single component float, expanded to (x, 0, 0, 1)
    Float2,         ///< 2-component float, expanded to (x, y, 0, 1)
    Float3,         ///< 3-component float, expanded to (x, y, z, 1)
    Float4,         ///< 4-component float
    Byte4,          ///< 4-component float (-128.0f..+127.0f) mapped to byte (-128..+127)
    UByte4,         ///< 4-component float (0.0f..255.0f) mapped to byte (0..255)
    Short2,         ///< 2-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32768)
    Short4,         ///< 4-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32768)
    NumVertexFormats,       
    InvalidVertexFormat, 
};

///	@brief
inline
ui32 getVertexFormatSize( VertexFormat format ) {
    ui32 size( 0 );
    switch( format ) {
        case VertexFormat::Float:
            size = sizeof( f32 );
            break;
        case VertexFormat::Float2:
            size = sizeof( f32 )*2;
            break;
        case VertexFormat::Float3:
            size = sizeof( f32 )*3;
            break;
        case VertexFormat::Float4:
            size = sizeof( f32 )*4;
            break;
        case VertexFormat::Byte4:
            size = sizeof( c8 ) * 4;
            break;
        case VertexFormat::UByte4:
            size = sizeof( uc8 ) * 4;
            break;
        case VertexFormat::Short2:
            size = sizeof( ui16 ) * 2;
            break;
        case VertexFormat::Short4:
            size = sizeof( ui16 ) * 4;
            break;
        case VertexFormat::NumVertexFormats:
        case VertexFormat::InvalidVertexFormat:
            break;

        default:
            break;
    }

    return size;
}

struct ExtensionProperty {
    c8   m_extensionName[ MaxEntNameLen ];
    ui32 m_version;

    ExtensionProperty() {
        ::memset( m_extensionName, '\0', sizeof( c8 ) * MaxEntNameLen );
        m_version = 0;
    }
};

///	@brief
struct OSRE_EXPORT VertComponent {
    VertexAttribute m_attrib;
    VertexFormat    m_format;

    VertComponent();    
    VertComponent( VertexAttribute attrib, VertexFormat format );
    ~VertComponent();

    OSRE_NON_COPYABLE( VertComponent )
};

///	@brief
struct OSRE_EXPORT VertexLayout {
    static VertComponent            ErrorComp;
    String                         *m_attributes;
    CPPCore::TArray<VertComponent*> m_components;
    CPPCore::TArray<ui32>           m_offsets;
    ui32                            m_currentOffset;
    ui32                            m_sizeInBytes;

    VertexLayout();     
    ~VertexLayout();
    void clear();
    ui32 numComponents() const;
    ui32 sizeInBytes();
    VertexLayout &add( VertComponent *comp );
    VertComponent &getAt( ui32 idx ) const;
    const String *getAttributes();

    OSRE_NON_COPYABLE( VertexLayout )
};

///	@brief
struct OSRE_EXPORT BufferData {
    BufferType       m_type;
    void            *m_data;
    ui32             m_size;
    BufferAccessType m_access;

    BufferData();
    ~BufferData();
    static BufferData *alloc( BufferType type, ui32 m_size, BufferAccessType access );
	static void free( BufferData *data );
    void copyFrom( void *data, ui32 size );

    OSRE_NON_COPYABLE( BufferData )
};

///	@brief
struct OSRE_EXPORT PrimitiveGroup {
    PrimitiveType m_primitive;
    ui32          m_startIndex;
    ui32          m_numPrimitives;
    IndexType     m_indexType;

    PrimitiveGroup();
    ~PrimitiveGroup();
    void init( IndexType indexType, ui32 numPrimitives, PrimitiveType primType, ui32 startIdx );

    OSRE_NON_COPYABLE( PrimitiveGroup )
};

///	@brief
enum class MaterialType {
    FlatShadingMaterial,
    ShaderMaterial
};

///	@brief
struct OSRE_EXPORT Texture {
    String            m_textureName;
    IO::Uri           m_loc;
    TextureTargetType m_targetType;
    ui32              m_size;
    uc8              *m_data;
    ui32              m_width;
    ui32              m_height;
    ui32              m_channels;
    Handle            m_texHandle;

    Texture();
    ~Texture();

    OSRE_NON_COPYABLE( Texture )
};

///	@brief
enum class ShaderType : int {
    SH_VertexShaderType = 0,
    SH_FragmentShaderType,
    SH_GeometryShaderType
};

static const ui32 MaxShaderTypes = 3;

///	@brief
struct OSRE_EXPORT Shader {
    CPPCore::TArray<String> m_parameters;
    CPPCore::TArray<String> m_attributes;
    String                  m_src[ MaxShaderTypes ];

    Shader();
    ~Shader();

    OSRE_NON_COPYABLE( Shader )
};

///	@brief
enum class MaterialColorType : unsigned int {
    Mat_Diffuse = 0,
    Mat_Specular,
    Mat_Ambient,
    Mat_Emission,
    MatColorMax
};

static const ui32 MaxMatColorType = (ui32) MaterialColorType::MatColorMax;

///	@brief
struct OSRE_EXPORT Material {
    MaterialType  m_type;
    ui32          m_numTextures;
    Texture     **m_textures;
    Shader       *m_pShader;
    ui32          m_numParameters;
    Parameter    *m_parameters;
    Color4        m_color[ MaxMatColorType ];
    
    Material();
    ~Material();

    OSRE_NON_COPYABLE( Material )
};

///	@brief
struct OSRE_EXPORT Transform {
    f32 m_translate[ 3 ];
    f32 m_scale[ 3 ];

    Transform();
    ~Transform();

    OSRE_NON_COPYABLE( Transform )
};

///	@brief
struct OSRE_EXPORT Geometry {
    Material       *m_material;
    VertexType      m_vertextype;
    BufferData     *m_vb;
    IndexType       m_indextype;
    BufferData     *m_ib;
    ui32            m_numPrimGroups;
    PrimitiveGroup *m_pPrimGroups;
    ui32            m_id;

    static Geometry *create( ui32 numGeo );
    static void destroy( Geometry **geo );

    OSRE_NON_COPYABLE( Geometry )

private:
    Geometry();
    ~Geometry();
};

struct OSRE_EXPORT GeoInstanceData {
    BufferData *m_data;

    GeoInstanceData();
    ~GeoInstanceData();

    OSRE_NON_COPYABLE( GeoInstanceData )
};

struct OSRE_EXPORT TransformBlock {
    glm::vec4 m_transform;
    glm::vec4 m_scale;
    glm::vec4 m_rotation;

    TransformBlock();
    ~TransformBlock();

    OSRE_NON_COPYABLE( TransformBlock )
};

struct OSRE_EXPORT TransformMatrixBlock {
    glm::mat4 m_projection;
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_mvp;

    TransformMatrixBlock();
    ~TransformMatrixBlock();
    void init();
    void update();
    const float *getMVP();

    OSRE_NON_COPYABLE( TransformMatrixBlock )
};

struct OSRE_EXPORT Viewport {
	i32 m_x;
	i32 m_y;
	i32 m_w;
	i32 m_h;

	Viewport();
	Viewport( i32 x, i32 y, i32 w, i32 h );
	~Viewport();
	bool operator == (const Viewport &rhs) const;
	
	OSRE_NON_COPYABLE( Viewport )
};

struct Canvas {
    ui32 m_x;
    ui32 m_y;
    ui32 m_width;
    ui32 m_height;
    ui32 m_z;
};

} // Namespace RenderBackend
} // Namespace OSRE

