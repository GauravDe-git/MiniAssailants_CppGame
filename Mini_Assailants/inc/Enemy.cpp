#include "Enemy.hpp"

#include <Graphics/ResourceManager.hpp>

#include <map>

#include "Constants.hpp"

using namespace Graphics;

static std::map<Enemy::State, std::string> g_stateNames =
{
	{Enemy::State::None, "None"},
	{Enemy::State::Idle, "Idle"},
	{Enemy::State::Chase, "Chase"},
	{Enemy::State::Attack, "Attack"},
	{Enemy::State::Hurt, "Hurt"},
};

Enemy::Enemy(const glm::vec2& pos,Type _type)
	:Entity{ pos }
	,type{ _type }
{
	switch (type)
	{
		case Type::Goblin:
			aabbs[State::Idle] = { {65,30,0},{80,70,0} };
			aabbs[State::Attack] = { {59,33,0},{80,70,0} };
			aabbs[State::Dead] = { {0,0,0},{0,0,0} };
			aabbs[State::None] = { {0,0,0},{0,0,0} };
			attackDistance = 55.0f;
			speed = 90.0f;
			hp = 12;
			attackDmg = 1;
			attackFrame = 2;

			idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Idle.png", 123, 82, 0, 0, BlendMode::AlphaBlend), 7.f };
			chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Chase.png", 123, 82, 0, 0, BlendMode::AlphaBlend), 8.f };
			attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Atk.png", 123, 82, 0, 0, BlendMode::AlphaBlend), 10.f };
			hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Hurt.png", 123, 82, 0, 0, BlendMode::AlphaBlend), 7.f };
			deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Dead.png",123,82,0,0,BlendMode::AlphaBlend),4.f };

			state = State::Idle;
			transform.setAnchor(glm::vec2{ 71.0f,69.0f });
			break;
		case Type::Skeleton:
			aabbs[State::Idle] = { {41,46,0},{64,102,0} };
			aabbs[State::Dead] = { {0,0,0},{0,0,0} };
			aabbs[State::None] = { {0,0,0},{0,0,0} };
			attackDistance = 55.0f;
			speed = 90.0f;
			hp = 20;
			attackDmg = 1;
			attackFrame = 2;

			idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Idle.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 7.f };
			chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Chase.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 8.f };
			attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Atk.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 11.f };
			hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Hurt.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 7.f };
			deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Dead.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 4.f };

			state = State::Idle;
			transform.setAnchor(glm::vec2{ 55.0f,99.0f });
			break;
	case Type::Golem:
		aabbs[State::Idle] = { { 38,18,0 },{ 78,79,0 } };
		aabbs[State::Chase] = { { 44,16,0 },{ 81,79,0 } };
		aabbs[State::Attack] = { { 55,17,0 },{ 96,75,0 } };
		aabbs[State::Dead] = { {0,0,0},{0,0,0} };
		aabbs[State::None] = { {0,0,0},{0,0,0} };
		attackDistance = 50.0f;
		speed = 90.0f;
		hp = 30;
		attackDmg = 1;
		attackFrame = 6;

		idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Idle.png", 116, 80, 0, 0, BlendMode::AlphaBlend), 7.f };
		chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Chase.png", 116, 80, 0, 0, BlendMode::AlphaBlend), 8.f };
		attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Atk.png", 116, 80, 0, 0, BlendMode::AlphaBlend), 11.f };
		hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Hurt.png", 116, 80, 0, 0, BlendMode::AlphaBlend), 7.f };
		deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Dead.png", 116, 80, 0, 0, BlendMode::AlphaBlend), 5.f };

		state = State::Idle;
		transform.setAnchor(glm::vec2{ 67.0f,77.0f });
		break;
	case Type::Harpy:
		aabbs[State::Idle] = { { 17,16,0 },{ 41,57,0 } };
		aabbs[State::Dead] = { {0,0,0},{0,0,0} };
		aabbs[State::None] = { {0,0,0},{0,0,0} };
		attackDistance = 40.0f;
		speed = 90.0f;
		hp = 15;
		attackDmg = 1;
		attackFrame = 3;

		idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Harpy_IdleChase.png", 87, 78, 0, 0, BlendMode::AlphaBlend), 7.f };
		chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Harpy_IdleChase.png", 87, 78, 0, 0, BlendMode::AlphaBlend), 8.f };
		attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Harpy_Atk.png", 87, 78, 0, 0, BlendMode::AlphaBlend), 11.f };
		hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Harpy_Hurt.png", 87, 78, 0, 0, BlendMode::AlphaBlend), 7.f };
		deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Harpy_Dead.png", 87, 78, 0, 0, BlendMode::AlphaBlend), 5.f };

		state = State::Idle;
		transform.setAnchor(glm::vec2{ 27.0f,60.0f });
		break;
	case Type::Centaur:
		aabbs[State::Idle] = { { 39,7,0 },{ 71,57,0 } };
		aabbs[State::Dead] = { {0,0,0},{0,0,0} };
		aabbs[State::None] = { {0,0,0},{0,0,0} };
		attackDistance = 52.0f;
		speed = 90.0f;
		hp = 20;
		attackDmg = 2;
		attackFrame = 3;

		idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Centaur_Idle.png", 89, 59, 0, 0, BlendMode::AlphaBlend), 7.f };
		chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Centaur_Chase.png", 89, 59, 0, 0, BlendMode::AlphaBlend), 8.f };
		attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Centaur_Atk.png", 89, 59, 0, 0, BlendMode::AlphaBlend), 11.f };
		hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Centaur_Hurt.png", 89, 59, 0, 0, BlendMode::AlphaBlend), 7.f };
		deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Centaur_Dead.png", 89, 59, 0, 0, BlendMode::AlphaBlend), 5.f };

		state = State::Idle;
		transform.setAnchor(glm::vec2{ 57.0f,55.0f });
		break;
	case Type::Gargoyle:
		aabbs[State::Idle] = { { 35,55,0 },{ 67,104,0 } };
		aabbs[State::Dead] = { {0,0,0},{0,0,0} };
		aabbs[State::None] = { {0,0,0},{0,0,0} };
		attackDistance = 57.0f;
		speed = 90.0f;
		hp = 30;
		attackDmg = 2;
		attackFrame = 3;

		idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Gargoyle_Idle.png", 125, 115, 0, 0, BlendMode::AlphaBlend), 7.f };
		chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Gargoyle_Chase.png", 125, 115, 0, 0, BlendMode::AlphaBlend), 8.f };
		attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Gargoyle_Atk.png",125, 115, 0, 0, BlendMode::AlphaBlend), 11.f };
		hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Gargoyle_Hurt.png", 125, 115, 0, 0, BlendMode::AlphaBlend), 7.f };
		deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Gargoyle_Dead.png", 125, 115, 0, 0, BlendMode::AlphaBlend), 5.f };

		state = State::Idle;
		transform.setAnchor(glm::vec2{ 62.0f,102.0f });
		break;
	case Type::Cerberus:
		aabbs[State::Idle] = { { 18,19,0 },{ 63,57,0 } };
		aabbs[State::Dead] = { {0,0,0},{0,0,0} };
		aabbs[State::None] = { {0,0,0},{0,0,0} };
		attackDistance = 50.0f;
		speed = 90.0f;
		hp = 15;
		attackDmg = 3;
		attackFrame = 3;

		idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Cerberus_Idle.png", 96, 61, 0, 0, BlendMode::AlphaBlend), 7.f };
		chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Cerberus_Chase.png", 96, 61, 0, 0, BlendMode::AlphaBlend), 8.f };
		attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Cerberus_Atk.png",96, 61, 0, 0, BlendMode::AlphaBlend), 11.f };
		hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Cerberus_Hurt.png", 96, 61, 0, 0, BlendMode::AlphaBlend), 7.f };
		deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Cerberus_Dead.png", 96, 61, 0, 0, BlendMode::AlphaBlend), 5.f };

		state = State::Idle;
		transform.setAnchor(glm::vec2{ 49.0f,56.0f });
		break;
	case Type::FlyingEye:
		aabbs[State::Idle] = { { 32,39,0 },{ 59,84,0 } };
		aabbs[State::Dead] = { {0,0,0},{0,0,0} };
		aabbs[State::None] = { {0,0,0},{0,0,0} };
		attackDistance = 39.0f;
		speed = 90.0f;
		hp = 40;
		attackDmg = 3;
		attackFrame = 5;

		idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/FlyingEye_IdleChase.png", 108, 117, 0, 0, BlendMode::AlphaBlend), 7.f };
		chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/FlyingEye_IdleChase.png", 108, 117, 0, 0, BlendMode::AlphaBlend), 8.f };
		attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/FlyingEye_Atk.png",108, 117, 0, 0, BlendMode::AlphaBlend), 11.f };
		hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/FlyingEye_Hurt.png", 108, 117, 0, 0, BlendMode::AlphaBlend), 7.f };
		deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/FlyingEye_Dead.png", 108, 117, 0, 0, BlendMode::AlphaBlend), 5.f };

		state = State::Idle;
		transform.setAnchor(glm::vec2{ 43.0f,89.0f });
		break;
		//Handle Other enemy types
	}
}

void Enemy::update(float deltaTime)
{
	collisionCircle.center = transform.getPosition();

	switch (state)
	{
	case State::Idle:
		doIdle(deltaTime);
		break;
	case State::Chase:
		doChase(deltaTime);
		break;
	case State::Attack:
		doAttack(deltaTime);
		break;
	case State::Reposition:
		doReposition(deltaTime);
		break;
	case State::Hurt:
		doHurt(deltaTime);
		break;
	case State::Dead:
		doDead(deltaTime);
		break;
	}
}

void Enemy::draw(Image& image, const Camera& camera)
{
	Math::Transform2D tempTransform = transform;
	tempTransform.translate(camera.getViewPosition());

	switch (state)
	{
	case State::Idle:
		image.drawSprite(idleAnim, tempTransform);
		break;
	case State::Chase:
		image.drawSprite(chaseAnim, tempTransform);
		break;
	case State::Attack:
		image.drawSprite(attackAnim, tempTransform);
		break;
	case State::Reposition:
		image.drawSprite(chaseAnim, tempTransform);
		break;
	case State::Hurt:
		image.drawSprite(hurtAnim, tempTransform);
		break;
	case State::Dead:
		image.drawSprite(deadAnim, tempTransform);
		break;
	}

#if _DEBUG
	// Draw AABB
	image.drawAABB(getAABB() + glm::vec3{ camera.getViewPosition(), 0 }, Color::Yellow, {}, FillMode::WireFrame);
	image.drawText(Font::Default, g_stateNames[state], transform.getPosition() + camera.getViewPosition() + glm::vec2{ -18, -58 }, Color::Yellow);
	image.drawText(Font::Default,"HP: " + std::to_string(hp), transform.getPosition() + camera.getViewPosition() + glm::vec2{-16, -70}, Color::Red);
	if (state == State::Attack)
	{
		image.drawCircle(attackCircle.center + camera.getViewPosition(), attackCircle.radius, Color::Cyan, {}, FillMode::WireFrame);
	}
	image.drawCircle(collisionCircle.center + camera.getViewPosition(), collisionCircle.radius, Color::Yellow, {}, FillMode::WireFrame);
#endif
}

Math::AABB Enemy::getAABB() const
{
	const auto it = aabbs.find(state);
	if (it != aabbs.end())
	{
		// If there's an AABB for the current state, use it
		return transform * it->second;
	}
	// Otherwise, use the Idle state's AABB as the default
	return transform * aabbs.at(State::Idle);
}

Math::Circle Enemy::getAttackCircle() const
{
	switch (type)
	{
	case Type::Goblin:
		return {{transform.getPosition() + glm::vec2{ 44.f, 30.f } * -transform.getScale() }, 11.f};
	case Type::Skeleton:
		return {{transform.getPosition() + glm::vec2{ 34.f, 30.f } * -transform.getScale() }, 12.f};
	case Type::Golem:
		return { {transform.getPosition() + glm::vec2{ 34.f, 5.f } *-transform.getScale() }, 13.f };
	case Type::Harpy:
		return { {transform.getPosition() + glm::vec2{ 30.f, 7.f } *-transform.getScale() }, 8.f };
	case Type::Centaur:
		return { {transform.getPosition() + glm::vec2{ 35.f, 17.f } *-transform.getScale() }, 9.f };
	case Type::Gargoyle:
		return { {transform.getPosition() + glm::vec2{ 40.f, 17.f } *-transform.getScale() }, 9.f };
	case Type::Cerberus:
		return { {transform.getPosition() + glm::vec2{ 40.f, 17.f } *-transform.getScale() }, 9.f };
	case Type::FlyingEye:
		return { {transform.getPosition() + glm::vec2{ 35.f, 45.f } *-transform.getScale() }, 11.f };
	}
	return {};
}

void Enemy::setState(State newState)
{
	if (newState != state)
	{
		beginState(newState);
		endState(state);
		state = newState;
	}
}

void Enemy::beginState(State newState)
{
	switch (newState)
	{
	case State::Idle:
		break;
	case State::Chase:
		break;
	case State::Attack:
		attackAnim.reset();
		break;
	case State::Hurt:
		break;
	case State::Dead:
		break;
	}
}

void Enemy::endState(State oldState)
{
	switch (oldState)
	{
	case State::Idle:
		break;
	case State::Chase:
		break;
	case State::Attack:
		break;
	case State::Hurt:
		break;
	case State::Dead:
		break;
	}
	attackCircle = {};
}

void Enemy::doMovement(float deltaTime)
{
	glm::vec2 initialPos = transform.getPosition();
	const auto targetPos = target ? target->getPosition() : initialPos;

	auto direction = targetPos - initialPos;

	direction = glm::length(direction) > 0 ? glm::normalize(direction) : direction;
	velocity = direction * speed;

	initialPos += velocity * deltaTime;

	transform.setPosition(initialPos);

	if (transform.getPosition().y > SCREEN_HEIGHT)
	{
		transform.setPosition({ transform.getPosition().x, SCREEN_HEIGHT });
	}
}

void Enemy::doIdle(float deltaTime)
{
	if (target != nullptr) {
		float distanceToPlayer = glm::distance(transform.getPosition(), target->getPosition());

		if (distanceToPlayer > attackDistance && distanceToPlayer < chaseDistance)
		{
			doMovement(deltaTime);
		}
	}
	if (glm::length(velocity) > 0)
	{
		setState(State::Chase);
	}

	idleAnim.update(deltaTime);
}


void Enemy::doChase(float deltaTime)
{
	if (target && glm::distance(transform.getPosition(), target->getPosition()) < attackDistance)
	{
		if(state != State::Attack)
		setState(State::Attack);
	}
	else
	{
		doMovement(deltaTime);
		if (glm::length(velocity) == 0.f)
		{
			setState(State::Idle);
		}
	}
	chaseAnim.update(deltaTime);
}

void Enemy::doAttack(float deltaTime)
{
	if (!target)
	{
		setState(State::Idle);
	}
	glm::vec2 targetPos = target ? target->getPosition() : transform.getPosition();
	float xDifference = std::abs(transform.getPosition().x - targetPos.x);
	float yDifference = std::abs(transform.getPosition().y - targetPos.y);

	// If the target is too close in the x direction and y direction, move away
	if (target && xDifference < 5.f && yDifference < attackDistance)
	{
		setState(State::Reposition);
	}
	// If the target is at the right distance, attack
	else if (target && glm::distance(transform.getPosition(), targetPos) < attackDistance)
	{
		attackAnim.update(deltaTime);
		if (attackAnim.getCurrentFrame() >= attackFrame)
		{
			attackCircle = getAttackCircle();
		}
		if (attackAnim.isDone())
		{
			attackAnim.reset();
			if (!target)
				setState(State::Idle);
		}
	}
	// If the target is too far, chase
	else if (target && glm::distance(transform.getPosition(), targetPos) > attackDistance)
	{
		setState(State::Chase);
	}
}

void Enemy::doReposition(float deltaTime)
{
	if (!target)
	{
		setState(State::Idle);
	}
	chaseAnim.update(deltaTime);
	glm::vec2 targetPos = target ? target->getPosition() : transform.getPosition();
	float xDifference = std::abs(transform.getPosition().x - targetPos.x);
	float yDifference = std::abs(transform.getPosition().y - targetPos.y);

	// Calculate the reposition point
	glm::vec2 repositionPoint = targetPos + glm::vec2((targetPos.x < transform.getPosition().x) ? attackDistance : -attackDistance, 0);

	// Move towards the reposition point
	glm::vec2 direction = glm::normalize(repositionPoint - transform.getPosition());
	transform.translate(direction * speed * deltaTime);

	// If the target is at the right distance, go back to attacking
	if (glm::distance(transform.getPosition(), targetPos) >= attackDistance)
	{
		setState(State::Attack);
	}
}


void Enemy::doHurt(float deltaTime)
{
	hurtAnim.update(deltaTime);
	const glm::vec2 knockBack{ 80.f * transform.getScale().x,0.f };

	transform.translate(knockBack * deltaTime);
	
	
	if (hurtAnim.isDone())
	{
		hurtAnim.reset();
		setState(State::Idle);
	}
}

void Enemy::doDead(float deltaTime)
{
	deadAnim.update(deltaTime);
	if (deadAnim.isDone())
	{
		deadAnim.reset();
		setState(State::JustDefeated);
	}
}

void Enemy::setFacingDirection(const glm::vec2& direction)
{
	if (getState() != State::Dead)
	{
		if (direction.x < 0) {
			transform.setScale(glm::vec2(1.0f, 1.0f));
		}
		else if (direction.x > 0) {
			transform.setScale(glm::vec2(-1.0f, 1.0f));
		}
	}
}

