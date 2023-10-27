#pragma once
#include <glm/vec2.hpp>

class Camera
{
public:
	Camera() = default;

	explicit Camera(const glm::vec2& pos)
		: position{pos}
	{}

	void setPosition(const glm::vec2& pos) { position = pos; }

	const glm::vec2& getPosition() const { return position; }

	glm::vec2 getViewPosition() const { return -position; }

private:
	glm::vec2 position;
};