#ifndef JPSHADER_H
#define JPSHADER_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <GL/glew.h>

class JPShader
{
public:
    JPShader();

    void CreateFromString(const char *vertexCode, const char *fragmentCode);

    void CreateFromFiles(const char *vertexLocation, const char *fragmentLocation);

    std::string ReadFile(const char *fileLocation);

    GLuint GetProjectionLocation() { return uniformProjection; }
    GLuint GetModelLocation() { return uniformModel; }
    GLuint GetViewLocation() { return uniformView; }
    GLuint GetShaderID() { return shaderID; }
    GLuint GetAmbientIntensityLocation() { return uniformAmbientIntensity; }
    GLuint GetAmbientColourLocation() { return uniformAmbientColour; }
    GLuint GetDiffuseIntensityLocation() { return uniformDiffuseIntensity; }
    GLuint GetDirectionLocation() { return uniformDirection; }

    void GetShaderUniformsInfo();

    void UseShader();
    void ClearShader();

    ~JPShader();

private:
    GLuint shaderID, uniformProjection, uniformModel, uniformView;
    GLuint uniformAmbientIntensity, uniformAmbientColour;
    GLuint uniformDiffuseIntensity, uniformDirection;

    void CompileShader(const char *vertexCode, const char *fragmentCode);
    void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);
};

#endif