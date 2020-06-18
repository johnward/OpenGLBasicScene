#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"

struct UniformDirectionalLight
{
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformDirection;
};

struct UniformPointLight
{
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    GLuint uniformPosition;
    GLuint uniformConstant;
    GLuint uniformLinear;
    GLuint uniformExponent;
};

class Shader
{
public:
    Shader();

    void CreateFromString(const char *vertexCode, const char *fragmentCode);

    void CreateFromFiles(const char *vertexLocation, const char *fragmentLocation);

    std::string ReadFile(const char *fileLocation);

    GLuint GetProjectionLocation() { return uniformProjection; }
    GLuint GetModelLocation() { return uniformModel; }
    GLuint GetViewLocation() { return uniformView; }
    GLuint GetEyePositionLocation() { return uniformEyePosition; }
    GLuint GetShaderID() { return shaderID; }
    GLuint GetAmbientIntensityLocation() { return uniformDirectionalLight.uniformAmbientIntensity; }
    GLuint GetAmbientColourLocation() { return uniformDirectionalLight.uniformColour; }
    GLuint GetDiffuseIntensityLocation() { return uniformDirectionalLight.uniformDiffuseIntensity; }
    GLuint GetDirectionLocation() { return uniformDirectionalLight.uniformDirection; }
    GLuint GetSpecularIntensityLocation() { return uniformSpecularIntensity; }
    GLuint GetShininess() { return uniformShininess; }

    void SetDirectionalLight(DirectionalLight *dLight);
    void SetPointLights(PointLight *pLight, unsigned int lightCount);

    void GetShaderUniformsInfo();

    void UseShader();
    void ClearShader();

    ~Shader();

private:
    int pointLightCount;

    GLuint shaderID, uniformProjection, uniformModel, uniformView;
    GLuint uniformEyePosition;
    GLuint uniformSpecularIntensity, uniformShininess; // Material Values

    // point lights uniform location values

    UniformDirectionalLight uniformDirectionalLight;

    GLuint uniformPointLightCount;

    UniformPointLight uniformPointLight[MAX_POINT_LIGHTS];

    void CompileShader(const char *vertexCode, const char *fragmentCode);
    void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};

#endif