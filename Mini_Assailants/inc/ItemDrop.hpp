#pragma once

#include <Entity.hpp>

#include <Graphics/Sprite.hpp>
#include <Graphics/Timer.hpp>

class ItemDrop : public Entity
{
public:
	enum class Type {HP, MP, Coin};

	ItemDrop() = default;

	ItemDrop(const glm::vec2& pos, Type _type);

	void draw(Graphics::Image& image, const Camera& camera) override;
	void update(float deltaTime) override;

	Type getType() const { return type; }
	int getValue() const { return value; }
	Math::AABB getAABB() const { return  transform * aabb; }

	bool canPickUp() const;
private:
	Type type{};
	Graphics::Sprite sprite{};
	int value;

	Math::AABB aabb{};
	Graphics::Timer pickupDelay{};
};
