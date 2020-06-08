#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    moveSpeed = startMovementSpeed;
    turnSpeed = startTurnSpeed;

    deltaTime = 1.0f;
    lastFrame = 0.0f;

    update();
}

void Camera::keyControl(bool *keys, GLfloat deltaTime)
{
    GLfloat velocity = moveSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }

    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }

    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }

    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    //printf("xChange: %.6f, yChange: %.6f\n", xChange, yChange);

    yaw += xChange;
    pitch += yChange;

    //printf("Pitch: %.6f, Yaw: %.6f\n", pitch, yaw);

    if (pitch > 89.0f) // pitch limit
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update();
}

glm::mat4 Camera::calculateViewMatrix()
{
    return glm::lookAt(position,         // where we are looking at from, our position
                       position + front, // Where the thing is that we are looking at
                       up);              // Where up is in the world
}

glm::vec3 Camera::getCameraPosition()
{
    return position;
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(pitch);
    front.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front); // remove magnitude

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front)); //!try swapping these
}

Camera::~Camera()
{
}