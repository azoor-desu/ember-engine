#pragma once

#include "util/defines.h"
#include "util/types.h"

EMB_NAMESPACE_START

class WindowManager
{
  public:
    EMB_CLASS_SINGLETON_MACRO(WindowManager)

    void Init();
    void Destroy();

    void PollInputEvents() noexcept;

    void CreateWindow();
    void SetWindowTitle();
    void SetWindowSize() noexcept;
    void GetWindowSize() const noexcept;
    embPtr* GetWindowHandle() const noexcept;

  private:
    embPtr* m_WindowHandle = nullptr;
};

EMB_NAMESPACE_END
