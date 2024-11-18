#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "shaders.h"
#include "drawn_object.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Rectangle.
std::vector<float> vertices = {
	 0.5f,  0.5f, 0.0f, //top right
	 0.5f, -0.5f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f  // top left
};
std::vector<unsigned int> indices = {
	0, 1, 3, //first tri
	1, 2, 3  //second tri
};
int main() {
	const int WIDTH = 800;
	const int HEIGHT = 600;
	const char* WINDOW_TITLE = "LearnOpenGL";

	// Initialize GLFW. Makes the GLFW functions work correctly.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); for MacOSX
	
	// Initialize the GLFW window.
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window." << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD to manage OpenGL function pointers.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	// Specifies the actual part of the window OpenGL renders to.
	// First two params set position of bottom left corner of window.
	// Last two set the width and height of the window.
	glViewport(0, 0, WIDTH, HEIGHT);

	// Register the window change callback.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Create the object to draw.
	drawn_object rectangle(vertices, indices, GL_STATIC_DRAW);
	// Set up the vertex attributes.
	rectangle.setVertexAttribPointer(0, 3, 3 * sizeof(float));
	// Unbind the vertex array (so we don't accidentally modify or anything).
	glBindVertexArray(0);
	// Create the shader program.
	shaderProgram program("shader.vert", "shader.frag");

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// Process input such as exit if Escape is pressed.
		processInput(window);

		// Clear the frame so that the previous frame doesn't bleed over.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program.ID);
		glBindVertexArray(rectangle.VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swaps front and back buffers.
		glfwSwapBuffers(window);
		// Checks if any events have been triggered (mouse movement; keyboard input; etc.).
		// Also updates the window state (such as window being resized).
		glfwPollEvents();
	}
	// Destroys remaining windows and frees allocated resources.
	// Also uninitializes GLFW.
	glfwTerminate();
	return 0;
}

// Process user input.
void processInput(GLFWwindow* window) {
	// If the user presses Escape, the window should close.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
// Want to resize the viewport when the user resizes the window.
// This function gets called whenever the GLFW window changes in size.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}