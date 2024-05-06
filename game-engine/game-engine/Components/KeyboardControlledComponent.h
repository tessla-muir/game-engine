#ifndef KEYBOARDCONTROLLEDCOMPONENT_H
#define KEYBOARDCONTROLLEDCOMPONENT_H

#include <glm.hpp>

struct KeyboardControlledComponent {
	glm::vec2 upVelocity;
	glm::vec2 rightVelocity;
	glm::vec2 leftVelocity;
	glm::vec2 downVelocity;

	// Set a consistent velocity for all directions
	KeyboardControlledComponent(float velocity) {
		this->upVelocity = glm::vec2(0, -velocity);
		this->downVelocity = glm::vec2(0, velocity);
		this->rightVelocity = glm::vec2(velocity, 0);
		this->leftVelocity = glm::vec2(-velocity, 0);
	}

	// Customize velocity for different directions
	// Lets you adjust x and y for each - can create interesting effects
	KeyboardControlledComponent(glm::vec2 upVelocity = glm::vec2(0), glm::vec2 rightVelocity = glm::vec2(0), glm::vec2 downVelocity = glm::vec2(0), glm::vec2 leftVelocity = glm::vec2(0)) {
		this->upVelocity = upVelocity;
		this->rightVelocity = rightVelocity;
		this->leftVelocity = leftVelocity;
		this->downVelocity = downVelocity;
	}
};

#endif