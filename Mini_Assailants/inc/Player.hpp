#pragma once

#include "Entity.hpp"

#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>
#include <Math/Transform2D.hpp>

#include <glm/vec2.hpp>

#include <string>

class Player
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

	void update(float deltaTime);

	void Draw(Graphics::Image& image, const glm::vec2& offset);

	void setPosition(const glm::vec2& pos);
	const glm::vec2& getPosition() const;

	void translate(const glm::vec2& t);

	const Math::AABB getAABB() const;

	void setScreenBounds(const Math::AABB& bounds);

private:
	void setState(State newState);

	void doMovement(float deltaTime);
	void doIdle(float deltaTime);
	void doWalk(float deltaTime);
	void doSpecial1(float deltaTime);

	State state = State::None;
	glm::vec2 velocity{ 0 };
	float speed{ 80.0f };
	int topEdgeCollision{ 0 };

	Graphics::SpriteAnim idleSprite;
	Graphics::SpriteAnim walkSprite;
	Graphics::SpriteAnim special1Sprite;

	Math::AABB aabb;
	Math::Transform2D transform;
	Math::AABB bounds;
};