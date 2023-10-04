#include<Player.hpp>

#include"Graphics/Input.hpp"

using namespace Graphics;

Player::Player() = default;

Player::Player(const glm::vec2& pos, const SpriteAnim& _sprite)
	:position{ pos }
	, sprite{ _sprite }
	, aabb{ {0,0,0},{16,16,0} }
{

}

void Player::update(float deltaTime)
{
	position.x += Input::getAxis("Horizontal") * speed * deltaTime;
	position.y -= Input::getAxis("Vertical") * speed * deltaTime;

	sprite.update(deltaTime);
}

void Player::Draw(Image& image)
{
	image.drawSprite(sprite, position.x, position.y);

	#if _DEBUG
		// Draw AABB
		image.drawAABB(aabb, Color::Yellow, {}, FillMode::WireFrame);
	#endif
}

void Player::setPosition(const glm::vec2& pos)
{
	position = pos;
}

const glm::vec2& Player::getPosition() const
{
	return position;
}

const Math::AABB Player::getAABB() const
{
	return aabb + glm::vec3{ position,0 };
}
