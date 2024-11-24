#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Entity {
public:
	Entity(glm::vec3 position);
	void setWindow(GLFWwindow* window);
	void render();
	void update();
	void translate(glm::vec3 trans);
	GLFWwindow* window;
	glm::mat4 transform;
	glm::vec3 position;
};

#endif