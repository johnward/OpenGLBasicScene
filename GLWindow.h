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

    void swapBuffers();

    ~GLWindow();

private:
    GLFWwindow *mainWindow;
    GLint width, height;
    GLint bufferWidth, bufferHeight;
};

#endif