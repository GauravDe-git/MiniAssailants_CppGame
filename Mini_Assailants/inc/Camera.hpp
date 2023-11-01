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

	void scroll(float deltaTime, const glm::vec2& playerVelocity);

	void follow(const glm::vec2& playerPos);

	void enterArenaMode(const glm::vec2& centerPos);

	Math::AABB getScreenBounds() const;

	void update(float deltaTime, const glm::vec2& playerPos, const glm::vec2& playerVelocity,bool isPlayerAttacking);

	void setPosition(const glm::vec2& pos) { position = pos; }
	const glm::vec2& getPosition() const { return position; }
	glm::vec2 getViewPosition() const { return -position; }

private:
	glm::vec2 position;
	State state;
	Math::AABB screenBounds;
};