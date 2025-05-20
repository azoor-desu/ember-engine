#pragma once
#include "util/macros.h"
#include "util/types.h"

EMB_NAMESPACE_START

class ResourceManager
{
  public:
    enum class ResourceType : embU8
    {
        SHADER_VERTEX,
        SHADER_FRAG,
        SHADER_COMPUTE,
        SHADER_PROGRAM,

        TEXTURE_SPRITE,
        TEXTURE_ALBEDO,

        AUDIO,
        MODEL, // or mesh...?
        FONT,
        SCENE,
        OPENGL_VAO, // ??? Maybe model will do...?
    };

    void LoadAsset();
    void GetAsset();
};

EMB_NAMESPACE_END
