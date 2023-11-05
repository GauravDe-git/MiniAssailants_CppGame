#include "Entity.hpp"

Entity::Entity(const glm::vec2& pos, const Math::AABB& _aabb)
	: transform{ pos }, aabb{ _aabb }
{
}
