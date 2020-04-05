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

#include "JPShader.h"
#include "GLWindow.h"

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh *> meshList;
std::vector<JPShader> shaderList;
GLWindow mainWindow;

// Vertex Shader
static const char *vShader = "shaders/shader.vert";

// Fragment Shader
static const char *fShader = "shaders/shader.frag";

void CreateObjects()
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

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	JPShader *shader1 = new JPShader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = GLWindow(800, 600);
	mainWindow.initialise();
	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// main loop until window closed

	while (!mainWindow.getShouldClose())
	{
		// get and handle user input events
		glfwPollEvents();

		//clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		// sets "model" in the shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &model[0][0]);
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
