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

    void CompileShader();
    void CompileShaderProgram();

    unsigned int VAO;
    unsigned int EBO;
    unsigned int shaderProgram;
    unsigned int texture;
    unsigned int texture2;
};

EMB_NAMESPACE_END
