#pragma once

#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>

#include <glm/vec2.hpp>

class Player
{
public:
	// Default constructor
	Player();

	explicit Player(const glm::vec2& pos, const Graphics::SpriteAnim& _sprite);

	void update(float deltaTime);

	void Draw(Graphics::Image& image);

	void setPosition(const glm::vec2& pos);
	const glm::vec2& getPosition() const;

	const Math::AABB getAABB() const;

private:
	glm::vec2 position{ 0 };
	float speed{ 60.0f };
	Graphics::SpriteAnim sprite;
	Math::AABB aabb;
};