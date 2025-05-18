#pragma once

#include "util/defines.h"
#include "util/str.h"
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

    void SetWindowTitle(embStrview strView) noexcept;

    void SetWindowSize(embS32 w, embS32 h) noexcept;
    embCoordInt GetWindowSize() const noexcept;

    void SetFramebufferSize(embS32 w, embS32 h) noexcept;
    embCoordInt GetFramebufferSize() const noexcept;

    embPtr GetWindowHandle() const noexcept;
    embBool GetWindowShouldClose() const noexcept;

    void SetWindowIconified(embBool iconify);
    void MaximizeWindow();

  private:
    embPtr m_WindowHandle = nullptr;
    embBool m_IsIconified = false;
};

EMB_NAMESPACE_END
