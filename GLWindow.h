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

    int initialise();

    GLfloat getBufferWidth();
    GLfloat getBufferHeight();

    bool getShouldClose();

<<<<<<< HEAD
    bool *getKeys() { return keys; }

    GLfloat getXChange();
    GLfloat getYChange();

    GLFWwindow *Window() { return mainWindow; }

    void swapBuffers() { glfwSwapBuffers(mainWindow); }
=======
    void swapBuffers();
>>>>>>> 085cdb422d8fe3a0322fe1bc40b311b8ebd49d76

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