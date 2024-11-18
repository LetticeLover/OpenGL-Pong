#include "shaders.h"

// Load the text of a shader file.
shaderReader::shaderReader(std::string name) {
	std::string line, allLines;
	std::ifstream theFile(name);
	if (theFile.is_open()) {
		while (std::getline(theFile, line)) {
			source = source + line + "\n";
		}

		theFile.close();
	}
	else {
		std::cout << "Unable to open file.";
	}
}

// Compile a shader.
void shader::compile(const char* file) {
	// Read in the vertex shader.
	shaderReader vertShaderReader = shaderReader(file);
	// Convert the source text to the correct type.
	const GLchar* s1 = vertShaderReader.source.c_str();
	// Set the vertex shader object's source and compile it.
	glShaderSource(ID, 1, &s1, NULL);
	glCompileShader(ID);
}
void shader::checkCompileStatus() {
	int success;
	char infoLog[512];
	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" + type_str + "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}
shader::shader(GLenum shaderType, const char* sourceFile) {
	type = shaderType;
	ID = glCreateShader(type);
	if (type == GL_VERTEX_SHADER) {
		type_str = "VERTEX";
	}
	else if (type == GL_FRAGMENT_SHADER) {
		type_str = "FRAGMENT";
	}
	compile(sourceFile);
	checkCompileStatus();
};
void shaderProgram::checkLinkingStatus() {
	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}
// Link a shader program.
shaderProgram::shaderProgram(const char* vertShaderSource, const char* fragShaderSource) {
	shader vertShader(GL_VERTEX_SHADER, vertShaderSource);
	shader fragShader(GL_FRAGMENT_SHADER, fragShaderSource);
	ID = glCreateProgram();
	glAttachShader(ID, vertShader.ID);
	glAttachShader(ID, fragShader.ID);
	glLinkProgram(ID);
	checkLinkingStatus();
	glDeleteShader(vertShader.ID);
	glDeleteShader(fragShader.ID);
}