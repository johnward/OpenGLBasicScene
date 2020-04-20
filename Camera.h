#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera();

    ~Camera();

    void ProcessCameraInput(GLFWwindow *window);

    void calculateDeltaTime();

    glm::mat4 GetView() { return _view; }

private:
    float _deltaTime;
    float _lastFrame;
    glm::vec3 _cameraPos;
    glm::vec3 _cameraFront;
    glm::vec3 _cameraUp;
    glm::mat4 _view;
};

#endif