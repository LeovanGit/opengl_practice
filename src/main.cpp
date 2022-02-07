#include <glad/glad.h>  // loads pointers to OpenGL functions at runtime
// #include <GL/glew.h> // same as GLAD
#include <GLFW/glfw3.h> // creating windows and handle user input
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);

int main()
{
    glfwInit();

    // version 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // get screen size
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode * mode = glfwGetVideoMode(monitor);

    GLFWwindow * window = glfwCreateWindow(mode->width,
                                           mode->height,
                                           "OpenGL practice",
                                           monitor,
                                           nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD\n";
        glfwTerminate();
        return -1;
    }

    glClearColor(0.47f, 0.47f, 0.47f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);
        
        glClear(GL_COLOR_BUFFER_BIT);

        // draw

        glfwSwapBuffers(window);
    }

    glfwTerminate(); // clear all allocated GLFW resources
    return 0;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
