#include"Player.hpp"

#include"Graphics/Input.hpp"

using namespace Graphics;

Player::Player() = default;

Player::Player(const glm::vec2& pos,const SpriteAnim& _sprite)
	:position{pos}
	,sprite{ _sprite }
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
}

void Player::setPosition(const glm::vec2& pos)
{
	position = pos;
}

const glm::vec2& Player::getPosition() const
{
	return position;
}
