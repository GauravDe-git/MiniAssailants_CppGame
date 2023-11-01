#pragma once
#include <glm/vec2.hpp>

class Camera
{
public:
	enum class State {
		scrolling,
		arena
	};

	Camera() = default;

	explicit Camera(const glm::vec2& pos);

	void update(float deltaTime, const glm::vec2& playerPos, const glm::vec2& playerVelocity,bool isPlayerAttacking);

	void setPosition(const glm::vec2& pos) { position = pos; }
	const glm::vec2& getPosition() const { return position; }
	glm::vec2 getViewPosition() const { return -position; }

private:
	glm::vec2 position;
	State state;
};