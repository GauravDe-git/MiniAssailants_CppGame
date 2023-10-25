#include <Player.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

Player::Player() = default;

Player::Player(const glm::vec2& pos)
	:transform{ pos }
	, aabb{ {9,65,0},{31,117,0} }
{
	auto idleSheet = ResourceManager::loadSpriteSheet("assets/textures/Idle_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	idleSprite = SpriteAnim{ idleSheet, 10.0f };

	auto walkSheet = ResourceManager::loadSpriteSheet("assets/textures/Walking_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	walkSprite = SpriteAnim{ walkSheet, 10.0f };

	transform.setAnchor(glm::vec2{21.0f,116.0f});
}

void Player::update(float deltaTime)
{
	auto initialPos = transform.getPosition();
	auto position = initialPos;

	position.x += Input::getAxis("Horizontal") * speed * deltaTime;
	position.y -= Input::getAxis("Vertical") * speed * deltaTime;

	//edge collision checks (temporary,need to change to a fn.)
	if (position.y > 270) // bottom edge collision
	{
		position.y = 270;
	}
	if (position.y < 225)
	{
		position.y = 225;
	}

	velocity = (position - initialPos) / deltaTime;

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
	transform.setPosition(position);
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
	Math::Transform2D tempTransform = transform;
	tempTransform.translate(offset); //glm::vec2{offset.x,115.f}
	// Draw the sprite with the transform.
	switch (state)
	{
	case State::Idle:
		image.drawSprite(idleSprite, tempTransform);
		break;
	case State::Walking:
		image.drawSprite(walkSprite, tempTransform);
		break;
	}

	#if _DEBUG
		// Draw AABB
	image.drawAABB(getAABB() + glm::vec3{offset, 0}, Color::Yellow, {}, FillMode::WireFrame);
	image.drawText(Font::Default, getState(state), transform.getPosition() + offset + glm::vec2{0, 50}, Color::Yellow);
	#endif
}

void Player::setPosition(const glm::vec2& pos)
{
	transform.setPosition(pos);
}

const glm::vec2& Player::getPosition() const
{
	return transform.getPosition();
}

void Player::translate(const glm::vec2& t)
{
	transform.translate(t);
}

const Math::AABB Player::getAABB() const
{
	return  transform * aabb;
}

void Player::setScreenBounds(const Math::AABB& _bounds)
{
	bounds = _bounds;
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
