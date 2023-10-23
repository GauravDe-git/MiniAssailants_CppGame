#pragma once

#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>

#include <glm/vec2.hpp>

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

	explicit Player(const glm::vec2& pos, const Graphics::SpriteAnim& _sprite);

	void update(float deltaTime);

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
	Graphics::SpriteAnim sprite;
	Math::AABB aabb;
};