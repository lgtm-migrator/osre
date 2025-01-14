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

#include <osre/Animation/AnimatorBase.h>
#include <osre/Platform/PlatformCommon.h>
#include <osre/Platform/KeyTypes.h>

namespace OSRE {

namespace RenderBackend {
    struct TransformMatrixBlock;
}

namespace App {

struct MouseInputState;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the default keyboard controlling.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT TransformController : public Animation::AnimationControllerBase {
public:
    /// @brief The class constructor.
    /// @param tmb  The transform matrix block to control.
    TransformController(RenderBackend::TransformMatrixBlock &tmb);

    ///	@brief The class destructor.
    ~TransformController() override = default;

    /// @brief Will return the command code from a key binding.
    /// @param key  The key binding
    /// @return The command code.
    static Animation::TransformCommandType getKeyBinding(Platform::Key key);

    /// @brief Will perform the mouse update.
    /// @param mis  The mouse input state.
    void getMouseUpdate(const MouseInputState &mis) override;

    /// @brief Will update the transformation.
    /// @param cmdType  The command.
    void update(Animation::TransformCommandType cmdType) override;

private:
    RenderBackend::TransformMatrixBlock &mTransform;
};

} // namespace App
} // namespace OSRE
