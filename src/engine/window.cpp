#include "engine/engine.h"
#include "pch-engine.h"

#include <GLFW/glfw3.h>

#include "util/macros.h"
#include "util/types.h"

#include "window.h"

EMB_NAMESPACE_START

// ========= Helper Macros =========

#define EMB_WINDOWPTR \
    (GLFWwindow*)m_WindowHandle

// ========= End Helper Macros =========

// ========= Callback Events =========

void callbackError(int error, const char* description)
{
    std::cerr << "GLFW error (code " << error << "): " << description << '\n';
}

void callbackWindowClosed(GLFWwindow*)
{
    Engine::Instance().SignalEngineStop();
}

void callbackWindowResized(GLFWwindow*, int width, int height)
{
    // Not to be confused with framebuffer size.
    // window size is in SCREEN coordinates, not PIXELS.
}

void callbackFramebufferResized(GLFWwindow*, int width, int height)
{
    // In PIXELS, not SCREEN coordinates.
    // glViewport takes in pixels.
    glViewport(0, 0, width, height);
}

// ========= End Callback Events =========

void WindowManager::Init()
{
    // Create window
    // Initialize and setup graphics backend

    if (!glfwInit())
    {
        printf("FATAL: GLFW failed to init\n");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

    m_WindowHandle = (embGenericPtr)glfwCreateWindow(800, 600, "EmberEngine", NULL, NULL);
    if (m_WindowHandle == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    // set callbacks
    glfwSetErrorCallback(callbackError);
    glfwSetWindowCloseCallback(EMB_WINDOWPTR, callbackWindowClosed);
    //glfwSetWindowSizeCallback(EMB_WINDOWPTR, callbackWindowResized);
    glfwSetFramebufferSizeCallback(EMB_WINDOWPTR, callbackFramebufferResized);
    // glfwSetKeyCallback(EMB_WINDOWPTR, GLHelper::key_cb);
    // glfwSetMouseButtonCallback(EMB_WINDOWPTR, GLHelper::mousebutton_cb);
    // glfwSetCursorPosCallback(EMB_WINDOWPTR, GLHelper::mousepos_cb);
    // glfwSetScrollCallback(EMB_WINDOWPTR, GLHelper::mousescroll_cb);

    // Other options
    //glfwSetWindowSizeLimits(EMB_WINDOWPTR, 200, 200, 400, 400); // min/max size
    glfwSetWindowAspectRatio(EMB_WINDOWPTR, 16, 9);
}

void WindowManager::Destroy()
{
    glfwDestroyWindow(EMB_WINDOWPTR);
    glfwTerminate();
}

void WindowManager::PollInputEvents() noexcept
{
    glfwPollEvents();
}

void WindowManager::SetWindowTitle(embStrView strView) noexcept
{
    glfwSetWindowTitle(EMB_WINDOWPTR, strView.begin());
}

void WindowManager::SetWindowSize(embS32 w, embS32 h) noexcept
{
    glfwSetWindowSize(EMB_WINDOWPTR, w, h);
}

embCoordInt WindowManager::GetWindowSize() const noexcept
{
    int width, height;
    glfwGetWindowSize(EMB_WINDOWPTR, &width, &height);
    return embCoordInt(width, height);
}

void WindowManager::SetFramebufferSize(embS32 w, embS32 h) noexcept
{
    glViewport(0, 0, w, h);
}

embCoordInt WindowManager::GetFramebufferSize() const noexcept
{
    int width, height;
    glfwGetFramebufferSize(EMB_WINDOWPTR, &width, &height);
    return embCoordInt(width, height);
}

embGenericPtr WindowManager::GetWindowHandle() const noexcept
{
    return m_WindowHandle;
}

embBool WindowManager::GetWindowShouldClose() const noexcept
{
    return glfwWindowShouldClose(EMB_WINDOWPTR);
}

void WindowManager::SetWindowIconified(embBool iconify)
{
    if (iconify == m_IsIconified)
        return;

    m_IsIconified = iconify;

    if (iconify)
        glfwIconifyWindow(EMB_WINDOWPTR);
    else
        glfwRestoreWindow(EMB_WINDOWPTR);
}

EMB_NAMESPACE_END
