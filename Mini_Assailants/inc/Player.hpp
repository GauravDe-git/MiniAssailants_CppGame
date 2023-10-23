#pragma once

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
		Walking
	};

	// Default constructor
	Player();

	explicit Player(const glm::vec2& pos);

	void update(float deltaTime);

	std::string_view getState(const State& state);
	void Draw(Graphics::Image& image, const glm::vec2& offset);

	void setPosition(const glm::vec2& pos);
	const glm::vec2& getPosition() const;

	void translate(const glm::vec2& t);

	const Math::AABB getAABB() const;

private:
	void setState(State newState);

	State state = State::None;
	glm::vec2 position{ 0 };
	glm::vec2 velocity{ 0 };
	float speed{ 80.0f };
	Graphics::SpriteAnim idleSprite;
	Graphics::SpriteAnim walkSprite;
	Math::AABB aabb;
	Math::Transform2D transform;
};