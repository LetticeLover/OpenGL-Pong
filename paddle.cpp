#include "paddle.h"
#include "iostream"

void Paddle::update() {
	if (glfwGetKey(window, upKey) == GLFW_PRESS) {
		translate(glm::vec3(0.0f, 0.01f, 0.0f));
	}
	if (glfwGetKey(window, downKey) == GLFW_PRESS) {
		translate(glm::vec3(0.0f, -0.01f, 0.0f));
	}
	if (position.y > 1.0f - 0.20f) {
		translate(glm::vec3(0.0f, -0.01f, 0.0f));
	}
	else if (position.y < -1.0f + 0.20f) {
		translate(glm::vec3(0.0f, 0.01f, 0.0f));
	}
}