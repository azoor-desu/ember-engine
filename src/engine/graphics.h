#pragma once

#include "util/types.h"

EMB_NAMESPACE_START

class Graphics
{
  public:
    static Graphics& Instance()
    {
        // thread safe singeton
        static Graphics obj;
        return obj;
    }

    void Init();
    void PreRender();
    void Render();
    void Destroy();
};

EMB_NAMESPACE_END
