#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

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
		NULL, // Monitor 
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

	// Set up our view port size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// main loop until window closed

	while (!glfwWindowShouldClose(mainWindow))
	{
		// get and handle user input events
		glfwPollEvents();

		//clear the window 
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);	
	}


	return 0;
}
