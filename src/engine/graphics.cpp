#include "pch-engine.h"

#include <GLFW/glfw3.h>

#include "util/types.h"

#include "graphics.h"
#include "window.h"

EMB_NAMESPACE_START

void Graphics::Init()
{
}


void Graphics::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers((GLFWwindow*)WindowManager::Instance().GetWindowHandle());
    glfwPollEvents();
}

void Graphics::Destroy()
{
}

EMB_NAMESPACE_END
