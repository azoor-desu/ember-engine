#pragma once

#include "util/defines.h"
#include "util/types.h"
#include "util/vec2_coord_container.h"

EMB_NAMESPACE_START

class WindowManager
{
  public:
    EMB_CLASS_SINGLETON_MACRO(WindowManager)

    void Init();
    void Destroy();

    void PollInputEvents() noexcept;

    //void CreateWindow();
    void SetWindowTitle();
    void SetWindowSize(embU32 w, embU32 h) noexcept;
    embCoordInt GetWindowSize() const noexcept;
    embPtr GetWindowHandle() const noexcept;
    embBool GetWindowShouldClose() const noexcept;

  private:
    embPtr m_WindowHandle = nullptr;
};

EMB_NAMESPACE_END
