#pragma once

#include "Entity.hpp"
#include "Constants.hpp"

#include <Graphics/SpriteAnim.hpp>
#include <glm/vec2.hpp>

class Enemy : public Entity
{
public:
	enum class Type
	{
		Goblin
	};

	enum class State
	{
		None,
		Idle,
		Chase,
		Attack,
		Hurt
	};

	Enemy();
	explicit Enemy(const glm::vec2& pos, Type type);

	virtual void update(float deltaTime) override;
	virtual void draw(Graphics::Image& image, const Camera& camera) override;

	void setTarget(Entity* _target) { target = _target; }
	const Math::AABB getAABB() const { return  transform * aabb; }

private:
	void setState(State newState);
	void beginState(State newState);
	void endState(State oldState);

	void doMovement(float deltaTime);
	void doIdle(float deltaTime);
	void doChase(float deltaTime);
	void doAttack(float deltaTime);

	Entity* target = nullptr;

	glm::vec2 velocity{ 0 };
	float attackDistance;
	float speed;
	Math::AABB aabb;

	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim chaseAnim;
	Graphics::SpriteAnim attackAnim;
	Graphics::SpriteAnim hurtAnim;

	Type type;
	State state = State::None;
};
