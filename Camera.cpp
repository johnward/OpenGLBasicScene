#include "Camera.h"

Camera::Camera()
{
    _deltaTime = 1.0f;
    _lastFrame = 0.0f;
    //_view = glm::mat4(1.0);
    //_view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f),
    //glm::vec3(0.0f, 0.0f, 0.0f),
    //glm::vec3(0.0f, 1.0f, 0.0f));

    _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    _cameraFront = glm::vec3(0.0f, 0.0f, -2.0f);
    _cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
    _view = glm::mat4(1.0f);
    _view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
}

void Camera::calculateDeltaTime()
{
    float currentFrame = glfwGetTime();
    _deltaTime = currentFrame - _lastFrame;
    _lastFrame = currentFrame;
}

void Camera::ProcessCameraInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (_deltaTime == 0)
    {
        _deltaTime = 1;
    }

    float cameraSpeed = 2.5f * _deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        _cameraPos += cameraSpeed * _cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        _cameraPos -= cameraSpeed * _cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        _cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        _cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
    }

    _view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
}

Camera::~Camera()
{
}