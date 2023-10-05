#include<Player.hpp>

#include"Graphics/Input.hpp"
#include"Graphics/Font.hpp"

using namespace Graphics;

Player::Player() = default;

Player::Player(const glm::vec2& pos, const SpriteAnim& _sprite)
	:position{ pos }
	, sprite{ _sprite }
	, aabb{ {9,65,0},{31,117,0} }
{

}

void Player::update(float deltaTime)
{
	auto initialPos = position;

	position.x += Input::getAxis("Horizontal") * speed * deltaTime;
	position.y -= Input::getAxis("Vertical") * speed * deltaTime;

	velocity = (position - initialPos) / deltaTime;

	if (glm::length(velocity) > 0)
	{
		setState(State::Walking);
	}
	else
	{
		setState(State::Idle);
	}

	sprite.update(deltaTime);
}

void Player::Draw(Image& image)
{
	//Conditionally draw sprite based on state-
	switch (state)
	{
	case State::Idle:
		image.drawSprite(sprite, position.x, position.y);
		break;
	case State::Walking:
		//Draw walking sprite anim
		break;
	}

	#if _DEBUG
		// Draw AABB
		image.drawAABB(getAABB(), Color::Yellow, {}, FillMode::WireFrame);
		image.drawText(Font::Default, "State:..?", position.x, position.y + 50, Color::Yellow);
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

void Player::translate(const glm::vec2& t)
{
	position += t;
}

const Math::AABB Player::getAABB() const
{
	return aabb + glm::vec3{ position,0 };
}

void Player::setState(State newState)
{
	if (newState != state)
	{
		switch (newState)
		{
		case State::Idle:
			break;
		case State::Walking:
			break;

		//Add remaining states here
		}
		state = newState;
	}
}
