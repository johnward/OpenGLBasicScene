#include "JPShader.h"

JPShader::JPShader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void JPShader::CreateFromString(const char *vertexCode, const char *fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void JPShader::CreateFromFiles(const char *vertexLocation, const char *fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    const char *vertexCode = vertexString.c_str();
    const char *fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

std::string JPShader::ReadFile(const char *fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("Failed to read %s file doesn't exist", fileLocation);
        return "";
    }

    std::string line = "";

    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

    return content;
}

GLuint JPShader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint JPShader::GetModelLocation()
{
    return uniformModel;
}

void JPShader::UseShader()
{
    glUseProgram(shaderID);
}

void JPShader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID); // deletes program off graphics card
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void JPShader::CompileShader(const char *vertexCode, const char *fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error creating shader program");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error Linking Program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error Validate Program: '%s'\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
}

void JPShader::AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar *theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, &shaderCode, NULL);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error Compile the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

JPShader::~JPShader()
{
    ClearShader();
}