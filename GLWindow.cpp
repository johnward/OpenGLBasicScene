#include "GLWindow.h"

GLWindow::GLWindow()
{
    width = 800;
    height = 600;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
}

int GLWindow::initialise()
{
    if (!glfwInit())
    {
        printf("GLFW Failed!");
        glfwTerminate();
        return 1;
    }

    // setup glfw window properties 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Profile - Core profile is not backwards compatable
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatability
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(
        width,
        height,
        "Test Window",
        NULL,  // Monitor
        NULL); // If it is shared accross systems

    if (!mainWindow)
    {
        printf("GLFW Window Creation failed");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information in the middle of window
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set the context for glew to use
    // THis window is where OpenGL should draw to
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extensions features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("Glew Initialisation failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Set up our view port size
    glViewport(0, 0, bufferWidth, bufferHeight);

    return 0;
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}