#pragma once

#include <Camera.hpp>

#include <Graphics/Image.hpp>

class Entity
{
public:
	virtual ~Entity() = default;

	virtual void update(float deltaTime) = 0;
	virtual void draw(Graphics::Image& image,const Camera& camera) = 0;

	void setPosition(const glm::vec2& pos) { transform.setPosition(pos); }
	const glm::vec2& getPosition() const { return transform.getPosition(); }
	void translate(const glm::vec2& t) { transform.translate(t); }

protected:
	Entity() = default;
	Entity(const glm::vec2& pos);

	Math::Transform2D transform;
};