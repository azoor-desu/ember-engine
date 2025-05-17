#include "pch-engine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util/types.h"

#include "graphics.h"

GLFWwindow* window; // TODO move this shit away

void error_cb(int error, const char* description)
{
    std::cerr << "GLFW error: " << description << std::endl;
}

EMB_NAMESPACE_START

void Graphics::Init()
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
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // set callbacks
    glfwSetErrorCallback(error_cb);
    // glfwSetFramebufferSizeCallback(GLHelper::ptr_window, GLHelper::fbsize_cb);
    // glfwSetKeyCallback(GLHelper::ptr_window, GLHelper::key_cb);
    // glfwSetMouseButtonCallback(GLHelper::ptr_window, GLHelper::mousebutton_cb);
    // glfwSetCursorPosCallback(GLHelper::ptr_window, GLHelper::mousepos_cb);
    // glfwSetScrollCallback(GLHelper::ptr_window, GLHelper::mousescroll_cb);

    glewExperimental = TRUE;
    GLenum err = glewInit();
    // BUG WITH GLFW ON LINUX: https://github.com/nigels-com/glew/issues/417
    // return code somehow returns 4 instead of 0 even if everything is ok.
    if (GLEW_OK != err && err != 4)
    {
        std::cerr << "Unable to initialize GLEW - error: "
                  << glewGetErrorString(err) << "| Code: " << err << " | abort program" << std::endl;
        exit(1);
    }
}


void Graphics::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Graphics::Destroy()
{
    glfwTerminate();
}
EMB_NAMESPACE_END
