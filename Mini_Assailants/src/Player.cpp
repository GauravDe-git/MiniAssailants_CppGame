#include <Player.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>

#include <iostream>
#include <map>

using namespace Graphics;

static std::map<Player::State, std::string> g_stateNames =
{
	{Player::State::None, "None"},
	{Player::State::Idle, "Idle"},
	{Player::State::Walking, "Walking"},
	{Player::State::Special1, "Special1"},
};

Player::Player() = default;

Player::Player(const glm::vec2& pos)
	:transform{ pos }
	, aabb{ {9,65,0},{31,117,0} }
{
	auto idleSheet = ResourceManager::loadSpriteSheet("assets/textures/Idle_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	idleSprite = SpriteAnim{ idleSheet, 10.0f };

	auto walkSheet = ResourceManager::loadSpriteSheet("assets/textures/Walking_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	walkSprite = SpriteAnim{ walkSheet, 10.0f };

	auto special1Sheet = ResourceManager::loadSpriteSheet("assets/textures/Special1_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	special1Sprite = SpriteAnim{ special1Sheet, 12.0f };

	transform.setAnchor(glm::vec2{ 21.0f,116.0f });
	setState(State::Idle);
}

void Player::setTopEdgeCollision(int top)
{
	topEdgeCollision = top;
}

void Player::update(float deltaTime)
{
	if (Input::getKeyDown(KeyCode::Y))
	{
		setState(State::Special1);
	}

	// Check the direction of movement and flip the sprite.
	if (velocity.x < 0) {
		transform.setScale(glm::vec2(-1.0f, 1.0f));
	}
	else if (velocity.x > 0) {
		transform.setScale(glm::vec2(1.0f, 1.0f));
	}

	switch (state)
	{
	case State::Idle:
		doIdle(deltaTime);
		break;
	case State::Walking:
		doWalk(deltaTime);
		break;
	case State::Special1:
		doSpecial1(deltaTime);
		break;
	}
}

void Player::Draw(Image& image, const glm::vec2& offset)
{
	// Set the position of the transform.
	Math::Transform2D tempTransform = transform;
	tempTransform.translate(offset);
	// Draw the sprite with the transform.
	switch (state)
	{
	case State::Idle:
		image.drawSprite(idleSprite, tempTransform);
		break;
	case State::Walking:
		image.drawSprite(walkSprite, tempTransform);
		break;
	case State::Special1:
		image.drawSprite(special1Sprite, tempTransform);
		break;
	}

#if _DEBUG
	// Draw AABB
	image.drawAABB(getAABB() + glm::vec3{ offset, 0 }, Color::Yellow, {}, FillMode::WireFrame);
	image.drawText(Font::Default, g_stateNames[state], transform.getPosition() + offset + glm::vec2{ -20, -70 }, Color::Yellow);
#endif
}

void Player::setState(State newState)
{
	if (newState != state)
	{
		beginState(newState);
		endState(state);
		state = newState;
	}
}

void Player::beginState(State newState)
{
	switch (newState)
	{
	case State::Idle:
		break;
	case State::Walking:
		break;
	case State::Special1:
		special1Sprite.reset();
		break;
		//Add remaining states here
	}
}

void Player::endState(State oldState)
{
	switch (oldState)
	{
	case State::Idle:
		break;
	case State::Walking:
		break;
	case State::Special1:

	{//Displacement of player pos upon landing
		glm::vec2 displacement(26.f, 0);

		// If the player is facing left, negate the displacement
		if (transform.getScale().x < 0)
		{
			displacement.x = -displacement.x;
		}

		transform.translate(displacement);
	}
	break;
	//Add remaining states here
	}
}

void Player::doMovement(float deltaTime)
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
	if (position.y < topEdgeCollision) // top edge collision
	{
		position.y = topEdgeCollision;
	}
	if (position.x < 0) // left edge collision
	{
		position.x = 0;
	}

	velocity = (position - initialPos) / deltaTime;

	transform.setPosition(position);
}

void Player::doIdle(float deltaTime)
{
	doMovement(deltaTime);
	if (glm::length(velocity) > 0)
	{
		setState(State::Walking);
	}

	idleSprite.update(deltaTime);
}

void Player::doWalk(float deltaTime)
{
	doMovement(deltaTime);
	if (glm::length(velocity) == 0)
	{
		setState(State::Idle);
	}
	walkSprite.update(deltaTime);
}

void Player::doSpecial1(float deltaTime)
{
	// Implement the special attack logic here
	special1Sprite.update(deltaTime);

	// Check if the special attack animation is done
	if (special1Sprite.isDone())
	{
		// Transition back to Idle state or another appropriate state
		setState(State::Idle);
	}
}