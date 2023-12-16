#pragma once

#include "Entity.hpp"
#include <Camera.hpp>
#include "UiBar.hpp"

#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>
#include <Math/Transform2D.hpp>

#include <glm/vec2.hpp>

#include <map>

class Player : public Entity
{
public:
	enum class State
	{
		None,
		Idle,
		Walking,
		LightAtk1,
		LightAtk2,
		Special1,
		Hurt
	};

	enum class AttackType
	{
		Light1,
		Light2,
		Heavy1,
		Heavy2,
		Special1,
		Special2
	};

	// Default constructor
	Player();

	explicit Player(const glm::vec2& pos);

	void setTopEdgeCollision(int top);

	virtual void update(float deltaTime) override;
	virtual void draw(Graphics::Image& image, const Camera& camera) override;

	const glm::vec2& getVelocity() const { return velocity; }
	bool isAttacking() const { return state == State::Special1 ||state == State::LightAtk1
							  || state == State::LightAtk2; }

	//---Combat related functions---//
	const Math::AABB getAABB() const { return  transform * aabb; }
	const Math::Circle& getAttackCircle() const { return attackCircle; }
	int getHP() const { return hp; }
	void reduceHP(int damage) { hp -= damage; }
	int getAtkDmg(AttackType type) const { return attackDmg.at(type); }
	AttackType getCurrentAtkType() const { return currentAtkType; }

	bool isHurt() const { return hitCounter > 0; }

	void setState(State newState);
	State getState() const { return state; }
private:
	
	void beginState(State newState);
	void endState(State oldState);

	void doMovement(float deltaTime);
	void doCombat();
	void doIdle(float deltaTime);
	void doWalk(float deltaTime);
	void doLightAtk1(float deltaTime);
	void doLightAtk2(float deltaTime);
	void doSpecial1(float deltaTime);
	void doHurt(float deltaTime);

	UiBar healthBar{};

	glm::vec2 velocity{ 0 };
	float speed{ 185.0f };
	int topEdgeCollision{ 0 };

	Math::AABB aabb;
	int hp{30};
	std::map<AttackType, int> attackDmg;
	Math::Circle attackCircle{};
	float timeSinceLastAtk{ 0.f };
	AttackType currentAtkType{};

	float hitCounter{0.f};

	State state = State::None;
	Graphics::SpriteAnim idleSprite;
	Graphics::SpriteAnim walkSprite;
	Graphics::SpriteAnim lightAtk1Sprite;
	Graphics::SpriteAnim lightAtk2Sprite;
	Graphics::SpriteAnim special1Sprite;
	Graphics::SpriteAnim hurtSprite;
};