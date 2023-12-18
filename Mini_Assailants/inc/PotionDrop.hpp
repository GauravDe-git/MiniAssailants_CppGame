#pragma once

#include <Entity.hpp>

#include "Graphics/Sprite.hpp"

class PotionDrop : public Entity
{
public:
	enum class Type {HP, MP};

	PotionDrop() = default;

	PotionDrop(const glm::vec2& pos, Type _type);

	void draw(Graphics::Image& image, const Camera& camera) override;
	void update(float deltaTime) override {}

	Type getType() const { return type; }
	int getValue() const { return value; }

private:
	Type type{};
	Graphics::Sprite sprite{};
	int value;

	Math::AABB aabb{};
};
