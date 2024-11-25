#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "shader.h"
#include "paddle.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

struct Vertex {
	GLfloat pos[3];
	GLubyte color[4];
	GLushort texcoord[2];
};
struct Rectangle {
	Vertex vertices[4];
};
const unsigned int loc_attrib = 0;
const unsigned int color_attrib = 1;
const unsigned int texcoord_attrib = 2;
// Both paddles vertices.
Rectangle paddleVertices[2] = {
	   // positions		       //colors			   //texcoords
 {{ {{ 0.05f,  0.20f,  0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //top right
	{{ 0.05f, -0.20f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // bottom right
	{{-0.05f, -0.20f,  0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // bottom left
	{{-0.05f,  0.20f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}} }}, // top left
	   // positions		       //colors			   //texcoords
 {{ {{ 0.05f,  0.20f,  0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //top right
	{{ 0.05f, -0.20f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // bottom right
	{{-0.05f, -0.20f,  0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // bottom left
	{{-0.05f,  0.20f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}} }} // top left
};
GLuint paddleIndices[] = {
	0, 1, 3, //first tri
	1, 2, 3  //second tri
};
GLint paddlesIndicesCounts[] = { 6, 6 };
GLvoid* paddlesBaseIndices[] = { (GLvoid*)0, (GLvoid*)0 };
GLint paddlesBaseVertices[] = { 0, 4 };


Paddle paddle1(glm::vec3(0.5f, 0.0f, 0.0f), GLFW_KEY_UP, GLFW_KEY_DOWN);
Paddle paddle2(glm::vec3(-0.5f, 0.0f, 0.0f), GLFW_KEY_W, GLFW_KEY_S);

int main() {
	const int WIDTH = 800;
	const int HEIGHT = 600;
	const char* WINDOW_TITLE = "LearnOpenGL";

	// Initialize GLFW. Makes the GLFW functions work correctly.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); for MacOSX
	
	// Initialize the GLFW window.
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window." << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	paddle1.setWindow(window);
	paddle2.setWindow(window);

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

	// Generate a new Vertex Buffer Object
	GLuint paddles;
	glCreateBuffers(1, &paddles);
	glNamedBufferData(paddles, sizeof(paddleVertices), paddleVertices, GL_STATIC_DRAW);

	GLuint paddleEBO;
	glCreateBuffers(1, &paddleEBO);
	glNamedBufferData(paddleEBO, sizeof(paddleIndices), paddleIndices, GL_STATIC_DRAW);
	
	// Generate a new Vertex Array Object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, paddleEBO);
	// Enable the attributes.
	glEnableVertexAttribArray(loc_attrib);
	glEnableVertexAttribArray(color_attrib);
	glEnableVertexAttribArray(texcoord_attrib);
	// Set the new VAO's attribute format.
	glVertexAttribFormat(loc_attrib, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
	glVertexAttribFormat(color_attrib, 4, GL_UNSIGNED_BYTE, GL_FALSE, offsetof(Vertex, color));
	glVertexAttribFormat(texcoord_attrib, 2, GL_UNSIGNED_SHORT, GL_FALSE, offsetof(Vertex, texcoord));
	// Bind this attribute format to a specific VBO binding index. 
	glVertexAttribBinding(loc_attrib, 0);
	glVertexAttribBinding(color_attrib, 0);
	glVertexAttribBinding(texcoord_attrib, 0);
	// Unbind the vertex array (so we don't accidentally modify or anything).
	glBindVertexArray(0);
	// Create the shader program.
	Shader shader1 = Shader("shader.vert", "shader.frag");

	// Set up shader uniforms.
	// Projection matrix transforms view/eye coordinates to clip coordinates and then to normalized coordinates [-1.0, 1.0].
	// Makes it so that things closer to you look closer and things farther look farther.
	glm::mat4 projection(1.0f);
	// View matrix transforms world space coordinates into camera coordinates.
	glm::mat4 view(1.0f);
	// Model matrix transforms model space coordinates into world space coordinates.
	//paddle1.transform;
	// We don't have any need for projection or view matrix so they are just identities.
	// MVP stands for Model * View * Projection matrix.
	glm::mat4 MVP;
	unsigned int MVPLoc = glGetUniformLocation(shader1.ID, "MVP");

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		// Process input such as exit if Escape is pressed.
		processInput(window);

		// Clear the frame so that the previous frame doesn't bleed over.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader1.use();
		glBindVertexArray(VAO);
		glBindVertexBuffer(0, paddles, 0, sizeof(Vertex));

		// Draw the paddles.
		MVP = projection * view * paddle1.transform;
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));
		glDrawElementsBaseVertex(GL_TRIANGLES, paddlesIndicesCounts[0], GL_UNSIGNED_INT, paddlesBaseIndices[0], paddlesBaseVertices[0]);
		
		MVP = projection * view * paddle2.transform;
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));
		glDrawElementsBaseVertex(GL_TRIANGLES, paddlesIndicesCounts[1], GL_UNSIGNED_INT, paddlesBaseIndices[1], paddlesBaseVertices[1]);

		//glMultiDrawElementsBaseVertex(GL_TRIANGLES, paddlesIndicesCounts, GL_UNSIGNED_INT, paddlesBaseIndices, 2, paddlesBaseVertices);
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
	paddle1.update();
	paddle2.update();
}
// Want to resize the viewport when the user resizes the window.
// This function gets called whenever the GLFW window changes in size.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}