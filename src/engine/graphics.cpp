#include "pch-engine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util/macros.h"
#include "util/types.h"

#include "graphics.h"
#include "window.h"

EMB_NAMESPACE_START

void Graphics::Init()
{
    // Init GLEW. make sure is after window is created.
    glfwMakeContextCurrent((GLFWwindow*)WindowManager::Instance().GetWindowHandle());

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

    // Load and compile shaders
    int success;
    char infoLog[512];

    const char* vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    const char* fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\0";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }


    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n"
                  << infoLog << std::endl;
    }

    // Cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Load models
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    // Create VAO to store all of the below configs:
    // VBO + all of the attributes and settings
    // One VAO can only be associated with one VBO.

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // bind it to bring VAO into focus, takes all of the settings below.

    // Create VBO that stores a buffer of vertices (for one object)
    unsigned int VBO;
    glGenBuffers(1, &VBO); // alloc buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bring buffer into focus
    // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // load data into focused buffer

    // Specify a layout of how to inpterpret a vertex buffer (vertex attribute), and save it to slot 0 (attribute 0)
    // e.g. [xyz][xyz][xyz]...
    // Is associated with the current VBO in focus (binded).
    // Corresponds to GLSL's vertex shader layout.
    // Offset is used in the case that you want to interleave other data other than pos (color etc),
    // and have another attribute refer to those.
    // [pos][col][pos][col][pos][col]...
    // layout0: position. Offset 0, stride [pos][col]
    // layout1: color. Offset [pos], stride [pos][col]
    // https://learnopengl.com/img/getting-started/vertex_array_objects.png
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // enable the vertex attribute feature for layout 0
}

void Graphics::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    glfwSwapBuffers((GLFWwindow*)WindowManager::Instance().GetWindowHandle());
    WindowManager::Instance().PollInputEvents();
}

void Graphics::Destroy()
{
}

EMB_NAMESPACE_END
