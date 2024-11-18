#include "drawn_object.h"
#include <iostream>

drawn_object::drawn_object(const std::vector<float> vertices, const std::vector<unsigned int> indices,GLenum usage) {
	// Generate the vertex array and buffers.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind VAO.
	glBindVertexArray(VAO);
	// Bind our buffers.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // From this point on, any buffer function calls we make (targeting GL_ARRAY_BUFFER) will affect this VBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Copy vertex data to VBO.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), usage);
	// Copy index data to EBO.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);
}
void drawn_object::setVertexAttribPointer(GLuint index, GLint size, GLsizei stride) {
	setVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride);
}
void drawn_object::setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride) {
	glVertexAttribPointer(index, size, type, normalized, stride, (void*)0);
	glEnableVertexAttribArray(index);
}