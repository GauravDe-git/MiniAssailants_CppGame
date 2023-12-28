#include <Player.hpp>
#include <UiBar.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

static std::map<Player::State, std::string> g_stateNames =
{
	{Player::State::None, "None"},
	{Player::State::Idle, "Idle"},
	{Player::State::Walking, "Walking"},
	{Player::State::LightAtk1, "LightAtk1"},
	{Player::State::LightAtk2, "LightAtk2"},
	{Player::State::HeavyAtk1, "HeavyAtk1"},
	{Player::State::HeavyAtk2, "HeavyAtk2"},
	{Player::State::Special1, "Special1"},
	{Player::State::Special2, "Special2"},
	{Player::State::Hurt, "Hurt"},
};

Player::Player() = default;

Player::Player(const glm::vec2& pos)
	:Entity{ pos } ,
	 aabb{{9, 65, 0}, {31, 117, 0}}
{
	const auto idleSheet = ResourceManager::loadSpriteSheet("assets/textures/Idle_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	idleSprite = SpriteAnim{ idleSheet, 10.0f };

	const auto walkSheet = ResourceManager::loadSpriteSheet("assets/textures/Walking_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	walkSprite = SpriteAnim{ walkSheet, 10.0f };

	// 2 anims for Light Attack (On pressing H key)
	const auto lightAtk1Sheet = ResourceManager::loadSpriteSheet("assets/textures/LightAtk1_Sheet.png", 153,			                                                                  127, 0, 0, BlendMode::AlphaBlend);
	lightAtk1Sprite = SpriteAnim{ lightAtk1Sheet, 13.5f };

	const auto lightAtk2Sheet = ResourceManager::loadSpriteSheet("assets/textures/LightAtk2_Sheet.png", 153,                                                                        127, 0, 0, BlendMode::AlphaBlend);
	lightAtk2Sprite = SpriteAnim{ lightAtk2Sheet, 15.0f };

	//2 anims for Heavy Attack (On pressing J key)
	const auto heavyAtk1Sheet = ResourceManager::loadSpriteSheet("assets/textures/HeavyAtk1_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	heavyAtk1Sprite = SpriteAnim{ heavyAtk1Sheet, 13.5f };

	const auto heavyAtk2Sheet = ResourceManager::loadSpriteSheet("assets/textures/HeavyAtk2_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	heavyAtk2Sprite = SpriteAnim{ heavyAtk2Sheet, 15.0f };

	const auto special1Sheet = ResourceManager::loadSpriteSheet("assets/textures/Special1_Sheet.png", 153,                                                                                 127, 0, 0, BlendMode::AlphaBlend);
	special1Sprite = SpriteAnim{ special1Sheet, 15.0f };

	const auto special2Sheet = ResourceManager::loadSpriteSheet("assets/textures/Special2_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	special2Sprite = SpriteAnim{ special2Sheet, 13.0f };

	attackDmg[AttackType::Light1] = 1;
	attackDmg[AttackType::Light2] = 2;
	attackDmg[AttackType::Heavy1] = 3;
	attackDmg[AttackType::Heavy2] = 4;
	attackDmg[AttackType::Special1] = 5;
	attackDmg[AttackType::Special2] = 5;

	transform.setAnchor(glm::vec2{ 21.0f,116.0f });
	setState(State::Idle);
}

void Player::setTopEdgeCollision(int top)
{
	topEdgeCollision = top;
}

void Player::update(float deltaTime)
{
	//Logic for light/heavy attack 2 
	hitCounter -= deltaTime;
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
	case State::HeavyAtk1:
		doHeavyAtk1(deltaTime);
		break;
	case State::HeavyAtk2:
		doHeavyAtk2(deltaTime);
		break;
	case State::Special1:
		doSpecial1(deltaTime);
		break;
	case State::Special2:
		doSpecial2(deltaTime);
		break;
	case State::Hurt:
		doHurt(deltaTime);
		break;
	}
}

void Player::draw(Image& image, const Camera& camera)
{
	//Draw the health bar
	const float healthPercent = static_cast<float>(hp) / maxHp;
	const Color healthColor = healthPercent > 0.5f ? Color::Green : (healthPercent > 0.25f ? Color::Yellow : Color::Red);
	healthBar.Draw(image, hp, maxHp, glm::vec2{10,25}, healthColor);

	//Draw the magic bar
	const float magicPercent = static_cast<float>(mp) / maxMp;
	const Color magicColor = magicPercent > 0.5f ? Color::Blue : Color::Cyan;
	mpBar.Draw(image, mp, maxMp, glm::vec2{ 10, 40 }, magicColor);

	//Logic to flash the player sprite upon damage
	//Solution using Sin wave shown by Jeremiah
	Color color = Color::White;
	if (hitCounter > 0.f)
	{
		const float alpha = (std::sin(hitCounter * 20.f) + 1.f) / 2.f;
		color = alpha * Color::White + (1.f - alpha) * Color::Red;
	}

	// Set the position of the transform.
	Math::Transform2D tempTransform = transform;
	tempTransform.translate(camera.getViewPosition());
	// Draw the sprite with the transform.
	switch (state)
	{
	case State::Idle:
		image.drawSprite(idleSprite, tempTransform, color);
		break;
	case State::Walking:
		image.drawSprite(walkSprite, tempTransform, color);
		break;
	case State::LightAtk1:
		image.drawSprite(lightAtk1Sprite, tempTransform, color);
		break;
	case State::LightAtk2:
		image.drawSprite(lightAtk2Sprite, tempTransform, color);
		break;
	case State::HeavyAtk1:
		image.drawSprite(heavyAtk1Sprite, tempTransform, color);
		break;
	case State::HeavyAtk2:
		image.drawSprite(heavyAtk2Sprite, tempTransform, color);
		break;
	case State::Special1:
		image.drawSprite(special1Sprite, tempTransform, color);
		break;
	case State::Special2:
		image.drawSprite(special2Sprite, tempTransform, color);
		break;
	case State::Hurt:
		image.drawSprite(idleSprite, tempTransform, color);
		break;
	}

#if _DEBUG
	image.drawAABB(getAABB() + glm::vec3{ camera.getViewPosition(), 0}, Color::Yellow, {}, FillMode::WireFrame);
	image.drawText(Font::Default, g_stateNames[state], transform.getPosition() + camera.getViewPosition() + glm::vec2{-20, -65}, Color::Yellow);
	image.drawText(Font::Default,"HP: " + std::to_string(hp), transform.getPosition() + camera.getViewPosition() + glm::vec2{-17, -78}, Color::Green);
	image.drawText(Font::Default, "MP: " + std::to_string(mp), transform.getPosition() + camera.getViewPosition() + glm::vec2{ -17, -90 }, Color::Blue);
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
	case State::HeavyAtk1:
		currentAtkType = AttackType::Heavy1;
		mp -= 1;
		heavyAtk1Sprite.reset();
		break;
	case State::HeavyAtk2:
		currentAtkType = AttackType::Heavy2;
		mp -= 1;
		heavyAtk2Sprite.reset();
		break;
	case State::Special1:
		currentAtkType = AttackType::Special1;
		special1Sprite.reset();
		break;
	case State::Special2:
		currentAtkType = AttackType::Special2;
		special2Sprite.reset();
		break;
	case State::Hurt:
		idleSprite.reset();
		hitCounter = 0.5f;
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
	case State::HeavyAtk1:
		break;
	case State::HeavyAtk2:
		break;
	case State::Special1:
		{
		//Displacement of player pos upon landing
		constexpr glm::vec2 displacement{26.f, 0};
		// If the player is facing left, negate the displacement
		transform.translate(displacement * transform.getScale());
		}
	break;
	case State::Special2:
	{
		//Displacement of player pos upon landing
		constexpr glm::vec2 displacement{ 82.f, 0 };
		// If the player is facing left, negate the displacement
		transform.translate(displacement * transform.getScale());
	}
	break;
	case State::Hurt:
		break;
	}
	//Reset the attack circle at end of each state
	attackCircle = {};
}

void Player::doMovement(float deltaTime)
{
	const auto initialPos = transform.getPosition();
	auto position = initialPos;

	position.x += Input::getAxis("Horizontal") * speed * deltaTime;
	position.y -= Input::getAxis("Vertical") * speed * deltaTime;

	//edge collision checks
	if (position.y > 270) // bottom edge collision
	{
		position.y = 270;
	}
	if (position.y < topEdgeCollision) // top edge collision
	{
		position.y = topEdgeCollision;
	}
	if (position.x < camera->getScreenBounds().left()) // left edge collision
	{
		position.x = camera->getScreenBounds().left();
	}
	if (position.x > camera->getScreenBounds().right()) // right edge collision
	{
		position.x = camera->getScreenBounds().right();
	}

	velocity = (position - initialPos) / deltaTime;

	transform.setPosition(position);
}

void Player::doCombat()
{
	if (mp < 0)
		mp = 0;

	if (Input::getKeyDown(KeyCode::H))
	{
		setState(State::LightAtk1);
		timeSinceLastAtk = 0.f;
	}
	else if (Input::getKeyDown(KeyCode::J) && mp>= 1 && !isAttacking()) //need to modify this more
	{
		setState(State::HeavyAtk1);
		timeSinceLastAtk = 0.f;
	}
	else if (Input::getKeyDown(KeyCode::Y) && mp >= 5 && !isAttacking())
	{
		setState(State::Special1);
		mp -= 5;
	}
	else if (Input::getKeyDown(KeyCode::U) && mp >= 5 && !isAttacking())
	{
		setState(State::Special2);
		mp -= 5;
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
			attackCircle = { transform.getPosition() + glm::vec2{ 32.f, -25.f } *transform.getScale(),8.5f };
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

	if (lightAtk2Sprite.getCurrentFrame() >= 3 && lightAtk2Sprite.getCurrentFrame() <= 4)
		attackCircle = { transform.getPosition() + glm::vec2{ 32.f, -30.f } *transform.getScale(),11.f };

	if (lightAtk2Sprite.isDone())
	{
		setState(State::Idle);
	}
}

void Player::doHeavyAtk1(float deltaTime)
{
		if (Input::getKeyDown(KeyCode::J) && mp >= 2 && timeSinceLastAtk < 2.f)
		{
			setState(State::HeavyAtk2);
		}
		else
		{
			heavyAtk1Sprite.update(deltaTime);

			if (heavyAtk1Sprite.getCurrentFrame() >= 3)
			{
				//need to fix this attack circle
				attackCircle = { transform.getPosition() + glm::vec2{ 42.f, -30.f } *transform.getScale(),12.f };
			}

			if (heavyAtk1Sprite.isDone())
			{
				setState(State::Idle);
			}
		}
	
}

void Player::doHeavyAtk2(float deltaTime)
{
	heavyAtk2Sprite.update(deltaTime);

	if (heavyAtk2Sprite.getCurrentFrame() >= 3 && heavyAtk2Sprite.getCurrentFrame() <= 4) //need to fix this frame too
		attackCircle = { transform.getPosition() + glm::vec2{ 52.f, -30.f } *transform.getScale(),13.f };

	if (heavyAtk2Sprite.isDone())
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

void Player::doSpecial2(float deltaTime)
{
	special2Sprite.update(deltaTime);

	if (special2Sprite.getCurrentFrame() >= 9) //fix this
	{
		attackCircle = { transform.getPosition() + glm::vec2{ 80.f, -10.f } *transform.getScale(),15.f };
	}
	else if (special2Sprite.getCurrentFrame() >= 4)
	{
		attackCircle = { transform.getPosition() + glm::vec2{ 40.f, -40.f } *transform.getScale(),15.f };
	}

	if (special2Sprite.isDone())
	{
		setState(State::Idle);
	}
}

void Player::doHurt(float deltaTime)
{
	setState(State::Idle);
}
