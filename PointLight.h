#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

class PointLight : public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue,
               GLfloat aIntensity, GLfloat dIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

    ~PointLight();

protected:
    glm::vec3 position; // where light is coming from

    GLfloat constant, linear, exponent; // Attenuation
};

#endif