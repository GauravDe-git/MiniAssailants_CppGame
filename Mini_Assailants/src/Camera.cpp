#include "Camera.hpp"
#include <Constants.hpp>

Camera::Camera(const glm::vec2& pos)
	: position{pos}, state{State::scrolling}
{
}

void Camera::update(float deltaTime, const glm::vec2& playerPos, const glm::vec2& playerVelocity,bool isPlayerAttacking)
{
	if (state == State::scrolling && !isPlayerAttacking)
	{
		position.x += playerVelocity.x * deltaTime;
	}

	// Prevent bg from scrolling off screen on left side (temporary fix)
	if (playerPos.x <= SCREEN_WIDTH / 2) {
		state = State::arena;
	}
	else if (playerPos.x > SCREEN_WIDTH / 2) {
		state = State::scrolling;
	}
}
