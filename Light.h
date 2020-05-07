#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stdio.h>

class Light
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);

    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);
    void getUniformData(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLuint shaderID);

    ~Light();

private:
    glm::vec3 colour;
    GLfloat ambientIntensity;
};

#endif