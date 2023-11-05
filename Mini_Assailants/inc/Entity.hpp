#pragma once

#include <Graphics/Image.hpp>
#include <Math/AABB.hpp>

class Entity
{
public:
	virtual ~Entity() = default;

	virtual void update(float deltaTime) = 0;
	virtual void draw(Graphics::Image& image /*camera*/) = 0;

protected:
	Math::AABB aabb;
	Math::Transform2D transform;
};