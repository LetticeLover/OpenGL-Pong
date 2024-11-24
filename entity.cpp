#include "entity.h"

Entity::Entity(glm::vec3 pos) {
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, pos);
	position = pos;
	window = NULL;
}
void Entity::setWindow(GLFWwindow* win) {
	window = win;
}
void Entity::render() {

}
void Entity::update() {

}
void Entity::translate(glm::vec3 trans) {
	transform = glm::translate(transform, trans);
	position += trans;
}