#pragma once

#include "util/macros.h"
#include "util/types.h"

EMB_NAMESPACE_START

class Graphics
{
  public:
    EMB_CLASS_SINGLETON_MACRO(Graphics)

    void Init();
    void PreRender();
    void Render();
    void Destroy();

    unsigned int VAO;
    unsigned int shaderProgram;
};

EMB_NAMESPACE_END
