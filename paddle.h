#pragma once
#ifndef PADDLE_H
#define PADDLE_H

#include "entity.h"

class Paddle : public Entity {
public:
	Paddle(glm::vec3 position, int upKey, int downKey) : Entity(position) {
		this->upKey = upKey;
		this->downKey = downKey;
	};
	void update();
	int upKey;
	int downKey;
};

#endif