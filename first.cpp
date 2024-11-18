#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

// Want to resize the viewport when the user resizes the window.
// This function gets called whenever the GLFW window changes in size.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
// Process user input.
void processInput(GLFWwindow* window) {
	// If the user presses Escape, the window should close.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
// Struct to parse and hold source text of a shader from file.
struct shaderReader
{
	shaderReader(std::string);
	std::string source;
};
// Load the text of a shader file.
shaderReader::shaderReader(std::string name)
{
	std::string line, allLines;
	std::ifstream theFile(name);
	if (theFile.is_open())
	{
		while (std::getline(theFile, line))
		{
			source = source + line + "\n";
		}

		theFile.close();
	}
	else
	{
		std::cout << "Unable to open file.";
	}
}

void compileShader(GLuint shader, const char* file) {
	// Read in the vertex shader.
	shaderReader vertShaderReader = shaderReader(file);
	// Convert the source text to the correct type.
	const GLchar* s1 = vertShaderReader.source.c_str();
	// Set the vertex shader object's source and compile it.
	glShaderSource(shader, 1, &s1, NULL);
	glCompileShader(shader);
}
// Triangle.
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
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

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate a Vertex Buffer Object and store its unique ID in unsigned int VBO.
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Bind the newly generated VBO to the current OpenGL Context.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // From this point on, any buffer function calls we make (targeting GL_ARRAY_BUFFER) will affect this VBO.
	// Send our vertices data to the GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Generate the unique ID of this shader.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, "shader.vert");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, "shader.frag");

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// Process input such as exit if Escape is pressed.
		processInput(window);

		// Clear the frame so that the previous frame doesn't bleed over.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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