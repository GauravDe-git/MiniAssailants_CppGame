#pragma once
#include <glm/vec2.hpp>
#include <Math/AABB.hpp>

class Camera
{
public:
	enum class State {
		scrolling,
		arena
	};

	Camera() = default;

	explicit Camera(const glm::vec2& pos);

	Math::AABB getScreenBounds() const;

	void update(float deltaTime, const glm::vec2& playerPos, const glm::vec2& playerVelocity,bool isPlayerAttacking);

	void setPosition(const glm::vec2& pos) { position = pos; }
	const glm::vec2& getPosition() const { return position; }
	glm::vec2 getViewPosition() const { return -position; }

	void translate(const glm::vec2& t) { position += t; }

private:
	void setState(State newState);

	void doScrolling(float deltaTime, const glm::vec2& playerVelocity, const glm::vec2& playerPos);
	void doArena(const glm::vec2& playerPos);

	glm::vec2 position;
	State state;
	Math::AABB screenBounds;
};