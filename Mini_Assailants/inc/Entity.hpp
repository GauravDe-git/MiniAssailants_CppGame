#pragma once

//Description: Entity base class for any entity/gameobjects in the game.
//			   (Ideally the aabb should have been here too, but had to exclude it due to different way of using aabbs
//              in the enemy class.)

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