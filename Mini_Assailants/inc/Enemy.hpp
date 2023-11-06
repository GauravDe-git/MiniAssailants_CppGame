#pragma once

#include "Entity.hpp"

#include <Graphics/SpriteAnim.hpp>
#include <glm/vec2.hpp>

class Enemy : public Entity
{
public:

	Enemy();
	explicit Enemy(const glm::vec2& pos);

	virtual void update(float deltaTime) override;
	virtual void draw(Graphics::Image& image, const Camera& camera) override;

private:

	Graphics::SpriteAnim goblinIdle;

};
