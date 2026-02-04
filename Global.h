#pragma once

#include <memory>
#include "OpenGLSomethingFrameDisplayerEVO/OpenGLSomethingFrameDisplayerEVO.h"

class Global
{
public:
    static std::unique_ptr<OpenGLSomethingFrameDisplayerEVO::OpenGLSomethingFrameDisplayerEVO> frameDisplayer;
};