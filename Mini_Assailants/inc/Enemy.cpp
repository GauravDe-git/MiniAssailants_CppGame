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
			aabb = { {75,62,0},{94,102,0} };
			attackDistance = 62.0f;
			speed = 78.0f;
			hp = 10;
			attackDmg = 1;

			idleAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Idle.png", 150, 125, 0, 0, BlendMode::AlphaBlend), 7.f };
			chaseAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Chase.png", 150, 125, 0, 0, BlendMode::AlphaBlend), 8.f };
			attackAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Atk.png", 150, 125, 0, 0, BlendMode::AlphaBlend), 7.f };
			hurtAnim = SpriteAnim{ ResourceManager::loadSpriteSheet("assets/textures/Goblin_Hurt.png", 150, 125, 0, 0, BlendMode::AlphaBlend), 7.f };

			state = State::Idle;
			transform.setAnchor(glm::vec2{ 84.0f,103.0f });
			break;
		//Handle Other enemy types
	}
}

void Enemy::update(float deltaTime)
{
	if (velocity.x < 0) {
		transform.setScale(glm::vec2(1.0f, 1.0f));
	}
	else if (velocity.x > 0) {
		transform.setScale(glm::vec2(-1.0f, 1.0f));
	}

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
	}
}

void Enemy::draw(Graphics::Image& image, const Camera& camera)
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
	}

#if _DEBUG
	// Draw AABB
	image.drawAABB(getAABB() + glm::vec3{ camera.getViewPosition(), 0 }, Color::Yellow, {}, FillMode::WireFrame);
	image.drawText(Font::Default, g_stateNames[state], transform.getPosition() + camera.getViewPosition() + glm::vec2{ -18, -58 }, Color::Yellow);
	image.drawText(Font::Default, std::to_string(hp), transform.getPosition() + camera.getViewPosition() + glm::vec2{-16, -70}, Color::Red);
#endif
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
	}
}

void Enemy::doMovement(float deltaTime)
{
	auto initialPos = transform.getPosition();
	auto targetPos = target ? target->getPosition() : initialPos;

	auto direction = targetPos - initialPos;

	direction = glm::length(direction) > 0 ? glm::normalize(direction) : direction;
	velocity = direction * speed;
	initialPos += velocity * deltaTime;

	transform.setPosition(initialPos);
}

void Enemy::doIdle(float deltaTime)
{
	if (!(target && glm::distance(transform.getPosition(), target->getPosition()) < attackDistance))
	{
		doMovement(deltaTime);
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
		if (attackAnim.isDone())
		{
			attackAnim.reset();
			setState(State::Idle);
		}
	}
}

void Enemy::doHurt(float deltaTime)
{
	hurtAnim.update(deltaTime);
	glm::vec2 knockBack{ 50.f * transform.getScale().x,0.f };

	transform.translate(knockBack * deltaTime);
	
	
	if (hurtAnim.isDone())
	{
		hurtAnim.reset();
		setState(State::Idle);
	}
}
