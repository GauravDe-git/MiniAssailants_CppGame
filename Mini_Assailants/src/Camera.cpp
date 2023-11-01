#include "Camera.hpp"
#include <Constants.hpp>

Camera::Camera(const glm::vec2& pos)
	: position{pos}, state{State::scrolling}
{
}

void Camera::scroll(float deltaTime, const glm::vec2& playerVelocity)
{
	if (state == State::scrolling)
	{
		position.x += playerVelocity.x * deltaTime;
	}
}

void Camera::follow(const glm::vec2& playerPos)
{
	if (state == State::arena)
	{
		position.x = playerPos.x - SCREEN_WIDTH / 2;
	}
}

void Camera::enterArenaMode(const glm::vec2& centerPos)
{
	state = State::arena;
	position.x = centerPos.x - SCREEN_WIDTH / 2;
}

Math::AABB Camera::getScreenBounds() const
{
	return screenBounds + position;
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

// state machine, aabb- getScreenBounds, pass the camera to player for stuff
