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

#include "Actions/ActionBase.h"

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Common {
    class Ids;
}

namespace App {
    class World;
    class Entity;
}

namespace Editor {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
///	@brief  This class will wrap the asset import command.
//-------------------------------------------------------------------------------------------------
class ImportAction : public ActionBase {
public:
    ImportAction(Common::Ids *ids, App::World *activeWorld);
    ~ImportAction() override;
    App::Entity *getEntity() const;
    ui32 getNumVertices() const;
    ui32 getNumTriangles() const;
    
protected:
    bool onRun(const ArgumentList &args) override;

private:
    Common::Ids *mIds;
    App::World *mActiveWorld;
    App::Entity *mEntity;
    ui32 mNumVertices, mNumTriangles;
};

inline App::Entity *ImportAction::getEntity() const {
    return mEntity;
}

inline ui32 ImportAction::getNumVertices() const {
    return mNumVertices;
}

inline ui32 ImportAction::getNumTriangles() const {
    return mNumTriangles;
}

} // namespace Editor
} // namespace OSRE
