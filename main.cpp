#define STB_IMAGE_IMPLEMENTATION

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
#include "UCamera.h"
#include "Texture.h"

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh *> meshList;
std::vector<JPShader> shaderList;
GLWindow mainWindow;
UCamera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brinkTexture;
Texture dirtyTexture;

void FramebufferResize(GLFWwindow *window, float height, float width);

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
		//   x    y     z    u      v
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom Left
		0.0f, -1.0f, 1.0f, 1.0f, 0.0f,	// Bottom Back (Middle)
		1.0f, -1.0f, 0.0f, 2.0f, 0.0f,	// Bottom Right
		0.0f, 1.0f, 0.0f, 1.0f, 2.0f};	// Top

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj2);
}

// Create a cube
void CreateCube()
{
	unsigned int indices[] = {
		0, 1, 2, // Front - tstartUpop triangle
		1, 2, 3, // Front
		4, 5, 6, // Back - top triangle
		5, 6, 7, // Back
		1, 5, 3, // Right Side
		3, 5, 7, // Right Side Back
		0, 4, 6, // Left
		0, 6, 2, // Left
		0, 4, 5, // startUp
		0, 1, 5, // Top
		2, 6, 7, // Bottom
		2, 3, 7	 // Bottom
	};

	GLfloat vertices[] = {
		-1.0f, 1.0f, 0.0f,	// 0 Left Top Front
		1.0f, 1.0f, 0.0f,	// 1 Right Top Front
		-1.0f, -1.0f, 0.0f, // 2 Left Bottom Front
		1.0f, -1.0f, 0.0f,	// 3 Right Bottom Front
		-1.0f, 1.0f, 1.0f,	// 4 Left Top Back
		1.0f, 1.0f, 1.0f,	// 5 Right Top Back
		-1.0f, -1.0f, 1.0f, // 6 Left Bottom Back
		1.0f, -1.0f, 1.0f}; // 7 Right Bottom Back*/

	Mesh *cube1 = new Mesh();
	cube1->CreateMesh(vertices, indices, 24, 36);
	meshList.push_back(cube1);
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
	//CreateCube();
	CreateShaders();

	camera = UCamera(glm::vec3(0.0f, 2.0f, 0.0f),
					 glm::vec3(0.0f, 1.0f, 0.0f),
					 -90.0f,
					 0.0f,
					 5.0f,
					 0.5f);

	brinkTexture = Texture((char *)"textures/brick_red.jpg");
	brinkTexture.LoadTexture();

	dirtyTexture = Texture((char *)"textures/concrete_dirty.jpg");
	dirtyTexture.LoadTexture();

	dirtyTexture.UseTexture();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// main loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat currentTime = glfwGetTime(); // SDL_GetPErformanceCounter() <- Need to convert to seconds
		deltaTime = currentTime - lastTime;	 // (currentTime-lastTime)*1000 / SDL_GetPerformanceFrequency() converts it to seconds
		lastTime = currentTime;

		//clear the windowglm::value_ptr(projection)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		shaderList[0].UseShader();

		glUniformMatrix4fv(shaderList[0].GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
		//float projectionVal[16] = {0.0f};
		//glGetUniformfv(shaderList[0].GetShaderID(), shaderList[0].GetProjectionLocation(), projectionVal);

		glUniformMatrix4fv(shaderList[0].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//float viewVal[16] = {0.0f};
		//glGetUniformfv(shaderList[0].GetShaderID(), shaderList[0].GetViewLocation(), viewVal);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel = shaderList[0].GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		//float modelVal[16] = {0.0f};
		//glGetUniformfv(shaderList[0].GetShaderID(), shaderList[0].GetModelLocation(), modelVal);

		brinkTexture.UseTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dirtyTexture.UseTexture();
		meshList[1]->RenderMesh();

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[2]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

		// get and handle user input events
		glfwPollEvents();
	}

	return 0;
}