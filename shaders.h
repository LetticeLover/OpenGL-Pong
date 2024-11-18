#pragma once
#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>

struct shaderReader {
	shaderReader(std::string);
	std::string source;
};
struct shader {
	shader(GLenum type, const char* sourceFile);
	void compile(const char* file);
	void checkCompileStatus();
	GLenum type;
	std::string type_str;
	unsigned int ID;
};
struct shaderProgram {
	shaderProgram(const char* vertShaderSource, const char* fragShaderSource);
	void checkLinkingStatus();
	unsigned int ID;
};

#endif