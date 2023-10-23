#include <Player.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

Player::Player() = default;

Player::Player(const glm::vec2& pos)
	:position{ pos }
	, aabb{ {9,65,0},{31,117,0} }
{
	auto idleSheet = ResourceManager::loadSpriteSheet("assets/textures/Idle_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	idleSprite = SpriteAnim{ idleSheet, 10.0f };

	auto walkSheet = ResourceManager::loadSpriteSheet("assets/textures/Walking_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	walkSprite = SpriteAnim{ walkSheet, 10.0f };
}

void Player::update(float deltaTime)
{
	auto initialPos = position;

	position.x += Input::getAxis("Horizontal") * speed * deltaTime;
	position.y -= Input::getAxis("Vertical") * speed * deltaTime;

	velocity = (position - initialPos) / deltaTime;
	transform.setAnchor(glm::vec2(76.5f, 63.5f));

	// Check the direction of movement and flip the sprite.
	if (velocity.x < 0) {
		transform.setScale(glm::vec2(-1.0f, 1.0f));
	}
	else if (velocity.x > 0) {
		transform.setScale(glm::vec2(1.0f, 1.0f));
	}

	if (glm::length(velocity) > 0)
	{
		setState(State::Walking);
		walkSprite.update(deltaTime);
	}
	else
	{
		setState(State::Idle);
		idleSprite.update(deltaTime);
	}
}

std::string_view Player::getState(const State& state)
{
	switch (state)
	{
	case State::Idle:
		return "Idle";
	case State::Walking:
		return "Walking";
	}
}

void Player::Draw(Image& image, const glm::vec2& offset)
{
	// Set the position of the transform.
	transform.setPosition(position + offset);

	// Draw the sprite with the transform.
	switch (state)
	{
	case State::Idle:
		image.drawSprite(idleSprite, transform);
		break;
	case State::Walking:
		image.drawSprite(walkSprite, transform);
		break;
	}

	#if _DEBUG
		// Draw AABB
	image.drawAABB(getAABB() + glm::vec3{offset, 0}, Color::Yellow, {}, FillMode::WireFrame);
	image.drawText(Font::Default, getState(state), position + offset + glm::vec2{0, 50}, Color::Yellow);
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
