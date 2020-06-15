#include "Light.h"

Light::Light()
{
    colour = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;
    diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
    colour = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;
}

void Light::getUniformData(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLuint shaderID)
{
    float colourLocationVal[3] = {0.0f};
    float intensityLocationVal[1] = {0.0f};
    glGetUniformfv(shaderID, ambientColourLocation, colourLocationVal);
    glGetUniformfv(shaderID, ambientIntensityLocation, intensityLocationVal);
}

Light::~Light()
{
}