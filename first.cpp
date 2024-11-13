#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Want to resize the viewport when the user resizes the window.
// This function gets called whenever the GLFW window changes in size.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

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

	// Render loop
	while (!glfwWindowShouldClose(window)) {
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