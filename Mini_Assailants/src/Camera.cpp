#include "Camera.hpp"
#include <Constants.hpp>

#include <Graphics/Keyboard.hpp>

Camera::Camera(const glm::vec2& pos)
	: position{pos}, state{State::scrolling}
	, screenBounds{ {0,0,0}, {SCREEN_WIDTH, SCREEN_HEIGHT,0} }
{
}

Math::AABB Camera::getScreenBounds() const
{
	return screenBounds + position;
}

void Camera::update(float deltaTime, const glm::vec2& playerPos, const glm::vec2& playerVelocity,bool isPlayerAttacking)
{
	switch (state)
	{
	case State::scrolling:
		if (!isPlayerAttacking)
		{
			doScrolling(deltaTime, playerVelocity, playerPos);
		}
		break;
	case State::arena:
		doArena(playerPos);
		break;
	}
}

void Camera::setState(State newState)
{
	if (newState != state)
	{
		switch (newState)
		{
		case State::scrolling:
			break;
		case State::arena:
			break;
		}
		state = newState;
	}
}

void Camera::doScrolling(float deltaTime, const glm::vec2& playerVelocity, const glm::vec2& playerPos)
{	
	position.x += playerVelocity.x * deltaTime;

	///// Go to arena if player reaches edge of screen//////
	if (playerPos.x <= SCREEN_WIDTH / 2) //worldBounds.left() 
	{
		setState(State::arena);
	}

}

void Camera::doArena(const glm::vec2& playerPos)
{
	////Logic for going to arena mode//////
	// no logic, essentially position.x remains unchanged

	//// Go to scrolling if player goes away from edge of screen////
	if (playerPos.x > SCREEN_WIDTH / 2)
	{
		setState(State::scrolling);
	}
}