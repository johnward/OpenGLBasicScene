#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow
{
public:
    GLWindow();
    GLWindow(GLint windowWidth, GLint windowHeight);

    int Initialise();

    GLfloat getBufferWidth() { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    bool *getsKeys() { return keys; }

    GLfloat getXChange();
    GLfloat getYChange();

    GLFWwindow *Window() { return mainWindow; }

    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    void FramebufferResize(GLFWwindow *window, int width, int height);

    ~GLWindow();

private:
    GLFWwindow *mainWindow;
    GLint width, height;
    GLint bufferWidth, bufferHeight;

    bool keys[1024];

    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange; // need to see how much you have moved the mouse
    GLfloat yChange; // this is used to move round the screen
    bool mouseFirstMove;

    void createCallbacks();
    static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};

#endif