#include "Camera.hpp"
#include <Constants.hpp>

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
	//Calculate the center of the screen
	const float screenCenterX = position.x + SCREEN_WIDTH / 2;

	//Calculate player's distance from center of the screen
	const float distanceFromCenter = playerPos.x - screenCenterX;

	//Now calculate the scrollSpeed factor based on distanceFromCenter
	float scrollSpeed = 1 + std::abs(distanceFromCenter) / (SCREEN_WIDTH/2);

	//adjustments for direction
	if (playerVelocity.x < 0 && distanceFromCenter > 0)
	{
		scrollSpeed *= -1;
	}
	else if (playerVelocity.x > 0 && distanceFromCenter < 0)
	{
		scrollSpeed *= -1;
	}

	position.x += playerVelocity.x * deltaTime * scrollSpeed;

	///// Go to arena if player reaches edge of screen//////
	if (position.x <= 0) //worldBounds.left() 
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