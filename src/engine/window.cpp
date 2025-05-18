#include "pch-engine.h"

#include "window.h"

#include "util/defines.h"
#include "util/types.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

EMB_NAMESPACE_START

// ========= Helper Macros =========

#define EMB_CAST_TO_WINDOWPTR(windowHandle) \
    (GLFWwindow*)windowHandle

// ========= End Helper Macros =========

// ========= Callback Events =========

void callbackError(int error, const char* description)
{
    std::cerr << "GLFW error: " << description << std::endl;
}

void callbackFramebufferResize(GLFWwindow* window, int width, int height)
{
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

    m_WindowHandle = (embPtr)glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (m_WindowHandle == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent((GLFWwindow*)m_WindowHandle);

    // set callbacks
    glfwSetErrorCallback(callbackError);
    // glfwSetFramebufferSizeCallback(GLHelper::ptr_window, GLHelper::fbsize_cb);
    // glfwSetKeyCallback(GLHelper::ptr_window, GLHelper::key_cb);
    // glfwSetMouseButtonCallback(GLHelper::ptr_window, GLHelper::mousebutton_cb);
    // glfwSetCursorPosCallback(GLHelper::ptr_window, GLHelper::mousepos_cb);
    // glfwSetScrollCallback(GLHelper::ptr_window, GLHelper::mousescroll_cb);

    glewExperimental = TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err
#ifdef EMB_PLATFORM_LINUX
        // BUG WITH GLFW ON LINUX: https://github.com/nigels-com/glew/issues/417
        // return code somehow returns 4 instead of 0 even if everything is ok.
        && err != 4
#endif
    )
    {
        std::cerr << "Unable to initialize GLEW - error: "
                  << glewGetErrorString(err) << "| Code: " << err << " | abort program" << std::endl;
        exit(1);
    }
}

void WindowManager::Destroy()
{
    glfwDestroyWindow(EMB_CAST_TO_WINDOWPTR(m_WindowHandle));
    glfwTerminate();
}

// void WindowManager::SetWindowTitle();

// void WindowManager::SetWindowSize(embU32 w, embU32 h) noexcept;

embCoordInt WindowManager::GetWindowSize() const noexcept
{
    int width, height;
    glfwGetWindowSize(EMB_CAST_TO_WINDOWPTR(m_WindowHandle), &width, &height);
    return embCoordInt(width, height);
}

embPtr WindowManager::GetWindowHandle() const noexcept
{
    return m_WindowHandle;
}
embBool WindowManager::GetWindowShouldClose() const noexcept
{
    return glfwWindowShouldClose(EMB_CAST_TO_WINDOWPTR(m_WindowHandle));
}

EMB_NAMESPACE_END
