#pragma once

//Description: Contains all info related to the player character.

#include "Entity.hpp"
#include <Camera.hpp>
#include <UiBar.hpp>

#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>
#include <Math/Transform2D.hpp>
#include <Audio/Sound.hpp>

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
		HeavyAtk1,
		HeavyAtk2,
		Special1,
		Special2,
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
							  || state == State::LightAtk2 || state == State::HeavyAtk1 || state == State::HeavyAtk2 || state == State::Special2;}

	//---Combat related functions---//
	const Math::AABB getAABB() const { return  transform * aabb; }
	const Math::Circle& getAttackCircle() const { return attackCircle; }
	int getHP() const { return hp; }
	int getMaxHP() const { return maxHp; }
	void setHP(int hp) { this->hp = hp; }
	int getMP() const { return mp; }
	int getMaxMP() const { return maxMp; }
	void setMP(int mp) { this->mp = mp; }
	int getAtkDmg(AttackType type) const { return attackDmg.at(type); }
	AttackType getCurrentAtkType() const { return currentAtkType; }

	int getCoins() const { return coinsCollected; }
	void setCoins(int coins) { coinsCollected = coins; }

	bool isHurt() const { return hitCounter > 0; }

	void setState(State newState);
	State getState() const { return state; }
	void setCamera(Camera* camera) { this->camera = camera; }
private:
	void beginState(State newState);
	void endState(State oldState);

	void doMovement(float deltaTime);
	void doCombat();
	void doIdle(float deltaTime);
	void doWalk(float deltaTime);
	void doLightAtk1(float deltaTime);
	void doLightAtk2(float deltaTime);
	void doHeavyAtk1(float deltaTime);
	void doHeavyAtk2(float deltaTime);
	void doSpecial1(float deltaTime);
	void doSpecial2(float deltaTime);
	void doHurt(float deltaTime);

	UiBar healthBar{ 100, 12, {0, -20} };
	UiBar mpBar{ 80, 12, {0, -20} };

	Camera* camera;

	glm::vec2 velocity{ 0 };
	float speed{ 100.0f };
	int topEdgeCollision{ 0 };

	Math::AABB aabb;
	int hp{15};
	int maxHp = hp;
	int mp{ 20 };
	int maxMp = mp;
	int coinsCollected{0};
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
	Graphics::SpriteAnim heavyAtk1Sprite;
	Graphics::SpriteAnim heavyAtk2Sprite;
	Graphics::SpriteAnim special1Sprite;
	Graphics::SpriteAnim special2Sprite;
	Graphics::SpriteAnim hurtSprite;

	Audio::Sound lightAtk1SFX{};
	Audio::Sound lightAtk2SFX{};
	Audio::Sound heavyAtk1SFX{};
	Audio::Sound heavyAtk2SFX{};
	Audio::Sound specialAtk1SFX{};
	Audio::Sound specialAtk2SFX{};
};
