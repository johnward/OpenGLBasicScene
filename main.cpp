#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Mesh.h"

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh *> meshList;

GLuint shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.02f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char *vShader = "						\n\
#version 330										\n\
													\n\
layout (location = 0) in vec3 pos;					\n\
                									\n\
out vec4 vCol;										\n\
																\n\
uniform mat4 model;												\n\
uniform mat4 projection;										\n\
            													\n\
void main()														\n\
{																\n\
	gl_Position = projection * model * vec4(pos, 1.0);			\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);					\n\
}";

// Fragment Shader
static const char *fShader = "						\n\
#version 330										\n\
                                                	\n\
in vec4 vCol;										\n\
                                                  	\n\
out vec4 colour;									\n\
                                                  	\n\
void main()											\n\
{													\n\
	colour = vCol;									\n\
	//colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);		\n\
                                                  	\n\
}";

void CreateTriangle()
{
	unsigned int indices[] = {
		0, 3, 1,  // Side
		1, 3, 2,  // Side
		2, 3, 0,  // Front
		0, 1, 2}; // base

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, // Bottom Left
		0.0f, -1.0f, 1.0f,	// Bottom Back (Middle)
		1.0f, -1.0f, 0.0f,	// Bottom Right
		0.0f, 1.0f, 0.0f};	// Top

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
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

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Error creating shader program");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error Linking Program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error Validate Program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjection = glGetUniformLocation(shader, "projection");
}

int main()
{
	if (!glfwInit())
	{
		printf("GLFW Failed!");
		glfwTerminate();
		return 1;
	}

	// setup glfw window properties 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Profile - Core profile is not backwards compatable
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatability
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(
		WIDTH,
		HEIGHT,
		"Test Window",
		NULL,  // Monitor
		NULL); // If it is shared accross systems

	if (!mainWindow)
	{
		printf("GLFW Window Creation failed");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information in the middle of window
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set the context for glew to use
	// THis window is where OpenGL should draw to
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extensions features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Glew Initialisation failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Set up our view port size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	// main loop until window closed

	while (!glfwWindowShouldClose(mainWindow))
	{
		// get and handle user input events
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxoffset)
		{
			direction = !direction;
		}

		curAngle += 0.1f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		if (sizeDirection)
		{
			curSize += 0.001f;
		}
		else
		{
			curSize -= 0.001f;
		}

		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		//clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		// sets "model" in the shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->RenderMesh();

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
