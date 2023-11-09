#include <Player.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>

#include <iostream>

using namespace Graphics;

static std::map<Player::State, std::string> g_stateNames =
{
	{Player::State::None, "None"},
	{Player::State::Idle, "Idle"},
	{Player::State::Walking, "Walking"},
	{Player::State::LightAtk1, "LightAtk1"},
	{Player::State::LightAtk2, "LightAtk2"},
	{Player::State::Special1, "Special1"},
	{Player::State::Hurt, "Hurt"},
};

Player::Player() = default;

Player::Player(const glm::vec2& pos)
	:Entity{ pos } ,aabb{ {9,65,0},{31,117,0} } 
{
	auto idleSheet = ResourceManager::loadSpriteSheet("assets/textures/Idle_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	idleSprite = SpriteAnim{ idleSheet, 10.0f };

	auto walkSheet = ResourceManager::loadSpriteSheet("assets/textures/Walking_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	walkSprite = SpriteAnim{ walkSheet, 10.0f };

	// 2 anims for Light Attack (On pressing H key)
	auto lightAtk1Sheet = ResourceManager::loadSpriteSheet("assets/textures/LightAtk1_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	lightAtk1Sprite = SpriteAnim{ lightAtk1Sheet, 12.0f };
	auto lightAtk2Sheet = ResourceManager::loadSpriteSheet("assets/textures/LightAtk2_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	lightAtk2Sprite = SpriteAnim{ lightAtk2Sheet, 14.0f };

	auto special1Sheet = ResourceManager::loadSpriteSheet("assets/textures/Special1_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	special1Sprite = SpriteAnim{ special1Sheet, 12.0f };

	auto hurtSheet = ResourceManager::loadSpriteSheet("assets/textures/Hurt_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	hurtSprite = SpriteAnim{ hurtSheet, 12.0f };

	attackDmg[AttackType::Light1] = 1;
	attackDmg[AttackType::Light2] = 2;
	attackDmg[AttackType::Special1] = 5;

	transform.setAnchor(glm::vec2{ 21.0f,116.0f });
	setState(State::Idle);
}

void Player::setTopEdgeCollision(int top)
{
	topEdgeCollision = top;
}

void Player::update(float deltaTime)
{
	if (isAttacking())
	{
		timeSinceLastAtk += deltaTime;
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
	case State::LightAtk1:
		doLightAtk1(deltaTime);
		break;
	case State::LightAtk2:
		doLightAtk2(deltaTime);
		break;
	case State::Special1:
		doSpecial1(deltaTime);
		break;
	case State::Hurt:
		doHurt(deltaTime);
		break;
	}
}

void Player::draw(Image& image, const Camera& camera)
{
	// Set the position of the transform.
	Math::Transform2D tempTransform = transform;
	tempTransform.translate(camera.getViewPosition());
	// Draw the sprite with the transform.
	switch (state)
	{
	case State::Idle:
		image.drawSprite(idleSprite, tempTransform);
		break;
	case State::Walking:
		image.drawSprite(walkSprite, tempTransform);
		break;
	case State::LightAtk1:
		image.drawSprite(lightAtk1Sprite, tempTransform);
		break;
	case State::LightAtk2:
		image.drawSprite(lightAtk2Sprite, tempTransform);
		break;
	case State::Special1:
		image.drawSprite(special1Sprite, tempTransform);
		break;
	case State::Hurt:
		image.drawSprite(hurtSprite, tempTransform);
		break;
	}

#if _DEBUG
	image.drawAABB(getAABB() + glm::vec3{ camera.getViewPosition(), 0}, Color::Yellow, {}, FillMode::WireFrame);
	image.drawText(Font::Default, g_stateNames[state], transform.getPosition() + camera.getViewPosition() + glm::vec2{-20, -65}, Color::Yellow);
	image.drawText(Font::Default,"HP: " + std::to_string(hp), transform.getPosition() + camera.getViewPosition() + glm::vec2{-17, -78}, Color::Green);
	if (isAttacking())
	{
		image.drawCircle(attackCircle.center + camera.getViewPosition(), attackCircle.radius, Color::Red, {}, FillMode::WireFrame);
	}
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
	case State::LightAtk1:
		currentAtkType = AttackType::Light1;
		lightAtk1Sprite.reset();
		break;
	case State::LightAtk2:
		currentAtkType = AttackType::Light2;
		lightAtk2Sprite.reset();
		break;
	case State::Special1:
		currentAtkType = AttackType::Special1;
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
	case State::LightAtk1:
		break;
	case State::LightAtk2:
		break;
	case State::Special1:
		{
		//Displacement of player pos upon landing
		glm::vec2 displacement{26.f, 0};
		// If the player is facing left, negate the displacement
		transform.translate(displacement * transform.getScale());
		}
	break;
	}
	//Reset the attack circle at end of each state
	attackCircle = {};
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

void Player::doCombat()
{
	if (Input::getKeyDown(KeyCode::H))
	{
		setState(State::LightAtk1);
		timeSinceLastAtk = 0.f;
	}
	else if (Input::getKeyDown(KeyCode::Y))
	{
		setState(State::Special1);
	}
}

void Player::doIdle(float deltaTime)
{
	doMovement(deltaTime);
	doCombat();
	if (glm::length(velocity) > 0)
	{
		setState(State::Walking);
	}

	idleSprite.update(deltaTime);
}

void Player::doWalk(float deltaTime)
{
	doMovement(deltaTime);
	doCombat();
	if (glm::length(velocity) == 0)
	{
		setState(State::Idle);
	}
	walkSprite.update(deltaTime);
}

void Player::doLightAtk1(float deltaTime)
{
	if (Input::getKeyDown(KeyCode::H) && timeSinceLastAtk < 2.f)
	{
		setState(State::LightAtk2);
	}
	else
	{
		lightAtk1Sprite.update(deltaTime);

		if (lightAtk1Sprite.getCurrentFrame() >= 3)
		{
			attackCircle = { transform.getPosition() + glm::vec2{ 32.f, -30.f } *transform.getScale(),10.f };
		}

		if (lightAtk1Sprite.isDone())
		{
			setState(State::Idle);
		}
	}
}

void Player::doLightAtk2(float deltaTime)
{
	lightAtk2Sprite.update(deltaTime);

	if (lightAtk2Sprite.getCurrentFrame() >= 3)
		attackCircle = { transform.getPosition() + glm::vec2{ 32.f, -30.f } *transform.getScale(),11.f };

	if (lightAtk2Sprite.isDone())
	{
		setState(State::Idle);
	}
}

void Player::doSpecial1(float deltaTime)
{
	special1Sprite.update(deltaTime);

	if (special1Sprite.getCurrentFrame() >= 4)
	{
		attackCircle = { transform.getPosition() + glm::vec2{ 30.f, -40.f } *transform.getScale(),15.f };
	}

	if (special1Sprite.isDone())
	{
		setState(State::Idle);
	}
}

void Player::doHurt(float deltaTime)
{
	hurtSprite.update(deltaTime);
	if (hurtSprite.isDone())
	{
		setState(State::Idle);
	}
}
