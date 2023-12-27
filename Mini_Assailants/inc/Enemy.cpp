#include "Enemy.hpp"
#include "Enemy.hpp"

#include <Graphics/ResourceManager.hpp>

#include <map>

using namespace Graphics;

static std::map<Enemy::State, std::string> g_stateNames =
{
	{Enemy::State::None, "None"},
	{Enemy::State::Idle, "Idle"},
	{Enemy::State::Chase, "Chase"},
	{Enemy::State::Attack, "Attack"},
	{Enemy::State::Hurt, "Hurt"},
};

Enemy::Enemy() = default;

Enemy::Enemy(const glm::vec2& pos,Type _type)
	:Entity{ pos }
	,type{ _type }
{
	switch (type)
	{
		case Type::Goblin:
			aabbs[State::Idle] = { {75,62,0},{94,102,0} };
			aabbs[State::Attack] = { {65,62,0},{94,102,0} };
			attackDistance = 55.0f;
			speed = 90.0f;
			hp = 10;
			attackDmg = 1;
			attackFrame = 2;

			idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Idle.png", 150, 125, 0, 0, BlendMode::AlphaBlend), 7.f };
			chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Chase.png", 150, 125, 0, 0, BlendMode::AlphaBlend), 8.f };
			attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Atk.png", 150, 125, 0, 0, BlendMode::AlphaBlend), 10.f };
			hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Hurt.png", 150, 125, 0, 0, BlendMode::AlphaBlend), 7.f };
			deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Dead.png",150,125,0,0,BlendMode::AlphaBlend),6.5f };

			state = State::Idle;
			transform.setAnchor(glm::vec2{ 84.0f,103.0f });
			break;
		case Type::Skeleton:
			aabbs[State::Idle] = { {41,46,0},{64,102,0} };
			attackDistance = 55.0f;
			speed = 90.0f;
			hp = 20;
			attackDmg = 1;
			attackFrame = 2;

			idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Idle.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 7.f };
			chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Chase.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 8.f };
			attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Atk.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 11.f };
			hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Hurt.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 7.f };
			deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Skeleton_Dead.png", 110, 120, 0, 0, BlendMode::AlphaBlend), 7.f };

			state = State::Idle;
			transform.setAnchor(glm::vec2{ 55.0f,99.0f });
			break;
	case Type::Golem:
		aabbs[State::Idle] = { { 33,13,0 },{ 81,78,0 } };
		aabbs[State::Chase] = { { 37,16,0 },{ 85,79,0 } };
		aabbs[State::Attack] = { { 44,16,0 },{ 85,79,0 } };
		attackDistance = 50.0f;
		speed = 90.0f;
		hp = 30;
		attackDmg = 2;
		attackFrame = 6;

		idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Idle.png", 111, 80, 0, 0, BlendMode::AlphaBlend), 7.f };
		chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Chase.png", 111, 80, 0, 0, BlendMode::AlphaBlend), 8.f };
		attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Atk.png", 111, 80, 0, 0, BlendMode::AlphaBlend), 11.f };
		hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Hurt.png", 111, 80, 0, 0, BlendMode::AlphaBlend), 7.f };
		deadAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Golem_Dead.png", 111, 80, 0, 0, BlendMode::AlphaBlend), 7.f };

		state = State::Idle;
		transform.setAnchor(glm::vec2{ 57.0f,77.0f });
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
		return { {transform.getPosition() + glm::vec2{ 34.f, 15.f } *-transform.getScale() }, 13.f };
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
	auto initialPos = transform.getPosition();
	const auto targetPos = target ? target->getPosition() : initialPos;

	auto direction = targetPos - initialPos;

	direction = glm::length(direction) > 0 ? glm::normalize(direction) : direction;
	velocity = direction * speed;

	initialPos += velocity * deltaTime;

	transform.setPosition(initialPos);
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
	if (target && glm::distance(transform.getPosition(), target->getPosition()) > attackDistance)
	{
		setState(State::Chase);
	}
	else
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
		//setState(State::None);
		setState(State::JustDefeated);
	}
}

void Enemy::setFacingDirection(const glm::vec2& direction)
{
	if (direction.x < 0) {
		transform.setScale(glm::vec2(1.0f, 1.0f));
	}
	else if (direction.x > 0) {
		transform.setScale(glm::vec2(-1.0f, 1.0f));
	}
}

