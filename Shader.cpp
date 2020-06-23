#include "Shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
    uniformDirectionalLight.uniformColour = 0;
    uniformDirectionalLight.uniformAmbientIntensity = 0;

    pointLightCount = 0;
    spotLightCount = 0;
}

void Shader::CreateFromString(const char *vertexCode, const char *fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char *vertexLocation, const char *fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    const char *vertexCode = vertexString.c_str();
    const char *fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char *fileLocation)
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

void Shader::SetDirectionalLight(DirectionalLight *dLight)
{
    dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity,
                     uniformDirectionalLight.uniformColour,
                     uniformDirectionalLight.uniformDiffuseIntensity,
                     uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight *pLight, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS)
    {
        lightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(uniformPointLightCount, lightCount); // int!

    for (size_t i = 0; i < lightCount; i++)
    {
        pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity,
                           uniformPointLight[i].uniformColour,
                           uniformPointLight[i].uniformDiffuseIntensity,
                           uniformPointLight[i].uniformPosition,
                           uniformPointLight[i].uniformConstant,
                           uniformPointLight[i].uniformLinear,
                           uniformPointLight[i].uniformExponent);
    }
}

void Shader::SetSpotLights(SpotLight *sLight, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS)
    {
        lightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(uniformSpotLightCount, lightCount); // int!

    for (size_t i = 0; i < lightCount; i++)
    {
        sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity,
                           uniformSpotLight[i].uniformColour,
                           uniformSpotLight[i].uniformDiffuseIntensity,
                           uniformSpotLight[i].uniformPosition,
                           uniformSpotLight[i].uniformDirection,
                           uniformSpotLight[i].uniformConstant,
                           uniformSpotLight[i].uniformLinear,
                           uniformSpotLight[i].uniformExponent,
                           uniformSpotLight[i].uniformEdge);
    }
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::GetShaderUniformsInfo()
{
    //glGetProgramInterface
    GLint uniformsNum = 0;
    glGetProgramiv(shaderID, GL_ACTIVE_UNIFORMS, &uniformsNum);
    if (uniformsNum > 0)
    {
        GLint maxLength;
        glGetProgramiv(shaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
        char *name = new char[maxLength];
        for (int i = 0; i < uniformsNum; ++i)
        {
            GLenum type;
            GLsizei nameLength;
            GLint size;
            glGetActiveUniform(shaderID, i, maxLength, &nameLength, &size, &type, name);

            float views[3] = {0.0f};
            glGetnUniformfv(shaderID, uniformView, sizeof(float), views);

            float projection[9] = {0.0f};
            glGetUniformfv(shaderID, uniformProjection, projection);

            float model[9] = {0.0f};
            glGetUniformfv(shaderID, uniformModel, model);
            printf("Name Length: %d, Size: %i, Type: %i, Name: %s", maxLength, size, type, name);
        }
        delete name;
    }
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID); // deletes program off graphics card
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode)
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

    uniformView = glGetUniformLocation(shaderID, "view");
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = {'\0'};
        snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].base.colour", i);
        uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = {'\0'};
        snprintf(locBuff, sizeof(locBuff), "spotLights[%ld].base.base.colour", i);
        uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%ld].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%ld].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%ld].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%ld].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%ld].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%ld].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%ld].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%ld].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }
}

void Shader::AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
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

Shader::~Shader()
{
    ClearShader();
}