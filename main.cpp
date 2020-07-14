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

#include "CommonValues.h"
#include "common/Mesh.h"
#include "common/Shader.h"
#include "common/GLWindow.h"
#include "common/Camera.h"
#include "common/Texture.h"
#include "common/DirectionalLight.h"
#include "common/PointLight.h"
#include "common/SpotLight.h"
#include "common/Material.h"
#include "common/Model.h"

#include <assimp/Importer.hpp>

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;
GLWindow mainWindow;
Camera camera;

Material shinyMaterial;
Material dullMaterial;

Model enterprise;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brinkTexture;
Texture dirtyTexture;
Texture plainTexture;
Texture brick2Texture;
Texture roadTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

void FramebufferResize(GLFWwindow *window, float height, float width);

// Vertex Shader
static const char *vShader = "shaders/shader.vert";

// Fragment Shader
static const char *fShader = "shaders/shader.frag";

void calcAverageNormals(unsigned int *indices,
						unsigned int indiceCount,
						GLfloat *vertices,
						unsigned int verticesCount,
						unsigned int vLength,
						unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0],
					 vertices[in1 + 1] - vertices[in0 + 1],
					 vertices[in1 + 2] - vertices[in0 + 2]);

		glm::vec3 v2(vertices[in2] - vertices[in0],
					 vertices[in2 + 1] - vertices[in0 + 1],
					 vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,  // Side
		1, 3, 2,  // Side
		2, 3, 0,  // Front
		0, 1, 2}; // base

	GLfloat vertices[] = {
		//   x    y     z    u      v   nx    ny    nz
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom Left
		0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // Bottom Back (Middle)
		1.0f, -1.0f, -0.6f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom Right
		0.0f, 1.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 0.0f};   // Top

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // back left
		10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f, // back right
		-10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f, // front left
		10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f, // front right
	};

	unsigned int floorIndices[] = {
		0, 2, 1, // tri 1
		1, 2, 3	 // tri 2
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
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
		//  x    y     z    u      v   nx    ny    nz
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // 0 Left Top Front
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,	  // 1 Right Top Front
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 2 Left Bottom Front
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 3 Right Bottom Front
		-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // 4 Left Top Back
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,	  // 5 Right Top Back
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 6 Left Bottom Back
		1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // 7 Right Bottom Back*/

	Mesh *cube1 = new Mesh();
	cube1->CreateMesh(vertices, indices, 64, 36);
	meshList.push_back(cube1);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = GLWindow(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateCube();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f),
					-90.0f,
					0.0f,
					5.0f,
					0.5f);

	brinkTexture = Texture((char *)"textures/brick.png");
	brinkTexture.LoadTextureA();

	dirtyTexture = Texture((char *)"textures/dirt.png");
	dirtyTexture.LoadTextureA();

	plainTexture = Texture((char *)"textures/plain.png");
	plainTexture.LoadTextureA();

	roadTexture = Texture((char *)"textures/road.jpg");
	roadTexture.LoadTexture();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	enterprise = Model();
	enterprise.LoadModel("models/enterprise1701d.obj");

	brick2Texture = Texture((char *)"textures/wall.jpg");
	brick2Texture.LoadTexture();

	dirtyTexture.UseTexture();

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,	 // colour and
								 0.3f, 0.6f,		 // 0.3, 0.4 Ambient intensity and Intensity of light for diffuse
								 0.0f, 0.0f, -1.0f); // Position of light

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								0.3f, 0.1f,
								4.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);

	pointLightCount++;

	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.3f, 0.1f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);

	pointLightCount++;

	unsigned int spotLightCount = 0;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
							  0.0f, 2.0f,
							  1.0f, 0.0f, 0.0f,
							  0.0f, -1.0f, 0.0f,
							  1.0f, 0.0f, 0.0f,
							  20.0f);

	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
							  0.0f, 1.0f,
							  0.0f, -1.5f, 0.0f,
							  -100.0f, -1.0f, 0.0f,
							  1.0f, 0.0f, 0.0f,
							  20.0f);

	spotLightCount++;

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

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		shaderList[0].UseShader();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f; // More realistic position of flash light (or say a game)
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(shaderList[0].GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
		//float projectionVal[16] = {0.0f};
		//glGetUniformfv(shaderList[0].GetShaderID(), shaderList[0].GetProjectionLocation(), projectionVal);
		glUniformMatrix4fv(shaderList[0].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//float viewVal[16] = {0.0f};
		//glGetUniformfv(shaderList[0].GetShaderID(), shaderList[0].GetViewLocation(), viewVal);
		glUniform3f(shaderList[0].GetEyePositionLocation(), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// Triangle 1
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		glUniformMatrix4fv(uniformModel = shaderList[0].GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		//float modelVal[16] = {0.0f};
		//glGetUniformfv(shaderList[0].GetShaderID(), shaderList[0].GetModelLocation(), modelVal);
		brinkTexture.UseTexture();
		shinyMaterial.UseMaterial(shaderList[0].GetSpecularIntensityLocation(), shaderList[0].GetShininess());
		meshList[0]->RenderMesh();

		// Triangle 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtyTexture.UseTexture();
		dullMaterial.UseMaterial(shaderList[0].GetSpecularIntensityLocation(), shaderList[0].GetShininess());
		meshList[1]->RenderMesh();

		//Floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		roadTexture.UseTexture();
		shinyMaterial.UseMaterial(shaderList[0].GetSpecularIntensityLocation(), shaderList[0].GetShininess());
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(shaderList[0].GetSpecularIntensityLocation(), shaderList[0].GetShininess());
		enterprise.RenderModel();

		// Cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, -3.5f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(shaderList[0].GetSpecularIntensityLocation(), shaderList[0].GetShininess());
		brick2Texture.UseTexture();
		meshList[3]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

		// get and handle user input events
		glfwPollEvents();
	}

	return 0;
}