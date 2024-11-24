#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// 1. Retrieve the vertex/fragment source code.
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// Ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// Open the files:
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read file buffer contents into the streams:
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// Close the ifstreams:
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string:
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. Compile Shaders.
	unsigned int vertex, fragment;

	// Vertex Shader:
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print errors if any:
	checkCompileErrors(ID, "VERTEX");
	// Fragment Shader:
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print errors if any:
	checkCompileErrors(ID, "FRAGMENT");

	// 3. Link Shaders.
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// Print errors if any:
	checkCompileErrors(ID, "PROGRAM");

	// Delete the unnecessary shaders as they are now linked:
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
// Activate the shader
void Shader::use() {
	glUseProgram(ID);
}
// Uniform functions
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// Utility function for checking shader compilation/linking errors.
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << type << "COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << type << "::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
}