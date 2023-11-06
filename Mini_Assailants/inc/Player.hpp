#pragma once

#include "Entity.hpp"
#include <Camera.hpp>

#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>
#include <Math/Transform2D.hpp>

#include <glm/vec2.hpp>

#include <string>

class Player : public Entity
{
public:
	enum class State
	{
		None,
		Idle,
		Walking,
		Special1
	};

	// Default constructor
	Player();

	explicit Player(const glm::vec2& pos);

	void setTopEdgeCollision(int top);

	virtual void update(float deltaTime) override;
	virtual void draw(Graphics::Image& image, const Camera& camera) override;

	const glm::vec2& getVelocity() const { return velocity; }
	bool isAttacking() const { return state == State::Special1; }

	void setScreenBounds(const Math::AABB& _bounds) { bounds = _bounds; }

private:
	void setState(State newState);
	void beginState(State newState);
	void endState(State oldState);

	void doMovement(float deltaTime);
	void doIdle(float deltaTime);
	void doWalk(float deltaTime);
	void doSpecial1(float deltaTime);

	glm::vec2 velocity{ 0 };
	float speed{ 80.0f };
	int topEdgeCollision{ 0 };

	State state = State::None;
	Graphics::SpriteAnim idleSprite;
	Graphics::SpriteAnim walkSprite;
	Graphics::SpriteAnim special1Sprite;

	Math::AABB bounds;
};