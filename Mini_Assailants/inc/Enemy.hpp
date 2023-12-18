#pragma once

#include "Entity.hpp"

#include <Graphics/SpriteAnim.hpp>
#include <glm/vec2.hpp>

#include <map>

class Enemy : public Entity
{
public:
	enum class Type
	{
		Goblin,
		Skeleton
	};

	enum class State
	{
		None,
		Idle,
		Chase,
		Attack,
		Hurt,
		Dead,
		JustDefeated
	};

	Enemy();
	explicit Enemy(const glm::vec2& pos, Type type);

	virtual void update(float deltaTime) override;
	virtual void draw(Graphics::Image& image, const Camera& camera) override;

	void setFacingDirection(const glm::vec2& direction);
	void setTarget(Entity* _target) { target = _target; }
	Math::AABB getAABB() const;

	//---------Getters/Setters-------------// 
	Math::Circle getAttackCircle() const;
	int getHp() const { return hp; }
	void setHp(int hp) { this->hp = hp; }
	void reduceHP(int damage) { hp -= damage; }
	int getAtkDmg() const { return attackDmg; }
	Math::Circle getCollisionCircle() const { return collisionCircle; }
	void setVelocity(glm::vec2 _velocity) { velocity = _velocity; }
	float getSpeed() const { return speed; }

	bool isAttacking() const {
		return state == State::Attack && attackAnim.getCurrentFrame() >= attackFrame;
	}

	void setState(State newState);
	State getState() const { return state; }
private:
	void beginState(State newState);
	void endState(State oldState);

	void doMovement(float deltaTime);
	void doIdle(float deltaTime);
	void doChase(float deltaTime);
	void doAttack(float deltaTime);
	void doHurt(float deltaTime);
	void doDead(float deltaTime);

	Entity* target = nullptr;

	glm::vec2 velocity{ 0 };
	float chaseDistance{ 230.f };
	float attackDistance{};
	float speed{};
	int hp{};
	int attackDmg{};
	std::map<State, Math::AABB> aabbs;
	Math::Circle attackCircle{};
	Math::Circle collisionCircle{{},10.f};
	int attackFrame{};

	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim chaseAnim;
	Graphics::SpriteAnim attackAnim;
	Graphics::SpriteAnim hurtAnim;
	Graphics::SpriteAnim deadAnim;

	Type type;
	State state = State::None;
};
