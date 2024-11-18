#pragma once
#ifndef DRAWN_OBJECT_H
#define DRAWN_OBJECT_H

#include <glad/glad.h>
#include <vector>

struct drawn_object {
	drawn_object(std::vector<float> vertices, std::vector<unsigned int> indices, GLenum usage);
	void setVertexAttribPointer(GLuint index, GLint size, GLsizei stride);
	void setVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

#endif