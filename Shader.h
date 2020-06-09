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
    GLuint GetAmbientIntensityLocation() { return uniformAmbientIntensity; }
    GLuint GetAmbientColourLocation() { return uniformAmbientColour; }
    GLuint GetDiffuseIntensityLocation() { return uniformDiffuseIntensity; }
    GLuint GetDirectionLocation() { return uniformDirection; }
    GLuint GetSpecularIntensityLocation() { return uniformSpecularIntensity; }
    GLuint GetShininess() { return uniformShininess; }

    void GetShaderUniformsInfo();

    void UseShader();
    void ClearShader();

    ~Shader();

private:
    GLuint shaderID, uniformProjection, uniformModel, uniformView;
    GLuint uniformEyePosition;
    GLuint uniformAmbientIntensity, uniformAmbientColour;
    GLuint uniformDiffuseIntensity, uniformDirection;
    GLuint uniformSpecularIntensity, uniformShininess; // Material Values

    void CompileShader(const char *vertexCode, const char *fragmentCode);
    void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};

#endif