#include "GLWindow.h"

GLWindow::GLWindow()
{
    width = 800;
    height = 600;
    lastX = 0.0f;
    lastY = 0.0f;
    xChange = 0.0f;
    yChange = 0.0f;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    lastX = 0.0f;
    lastY = 0.0f;
    xChange = 0.0f;
    yChange = 0.0f;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

int GLWindow::Initialise()
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
    // This window is where OpenGL should draw to
    glfwMakeContextCurrent(mainWindow);

    //handle key + mouse input
    createCallbacks();

    //disable the curson
    //glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glfwSetWindowUserPointer(mainWindow, this);

    return 0;
}

// Set the key press callback etc
void GLWindow::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat GLWindow::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat GLWindow::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

void GLWindow::handleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
    GLWindow *theWindow = static_cast<GLWindow *>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            //printf("Pressed: %d", key);
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
            //printf("Released: %d", key);
        }
    }
}

void GLWindow::handleMouse(GLFWwindow *window, double xPos, double yPos)
{
    GLWindow *theWindow = static_cast<GLWindow *>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMove)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMove = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos; // Inverted mouse controls

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    //printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}