/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace RenderBackend {

struct Buffer {
    ui32 m_handle;
    ui32 m_size;
};

class GPUBufferManager {
public:
    struct Impl {
        ~Impl();
        virtual Buffer *createBuffer( const String &desc ) = 0;
        virtual Buffer *getBufferByDesc( const String &desc );
        virtual void updateBuffer( BufferData &data ) = 0;
        virtual void appendToBuffer( BufferData &data ) = 0;
        virtual void releaseBuffer( Buffer *buffer ) = 0;
    };

public:
    GPUBufferManager( Impl *impl );
    ~GPUBufferManager();

    Buffer *createBuffer( const String &desc );
    Buffer *getBufferByDesc( const String &desc );
    void updateBuffer( BufferData &data );
    void appendToBuffer( BufferData &data );
    void releaseBuffer( Buffer *buffer );

private:
    typedef CPPCore::THashMap<String, Buffer*> BufferMap;
    BufferMap m_bufferMap;
    RenderBackendType m_backendType;
    Impl *m_impl;
};

inline
GPUBufferManager::Impl::~Impl() {
    // empty
}

} // Namespace RenderBackend
} // Namespace OSRE
