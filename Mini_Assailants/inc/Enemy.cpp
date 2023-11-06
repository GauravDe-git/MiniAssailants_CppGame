#include "Enemy.hpp"

#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

Enemy::Enemy() = default;

Enemy::Enemy(const glm::vec2& pos)
	:Entity{ { pos } ,{ {34,63,0},{94,103,0} } }
{
	auto goblinIdleSheet = ResourceManager::loadSpriteSheet("assets/textures/Goblin_Idle.png", 150, 125, 0, 0);
	goblinIdle = SpriteAnim{ goblinIdleSheet, 7.f};


	transform.setAnchor(glm::vec2{ 84.0f,103.0f });
}

void Enemy::update(float deltaTime)
{
	goblinIdle.update(deltaTime);
}

void Enemy::draw(Graphics::Image& image, const Camera& camera)
{
	Math::Transform2D tempTransform = transform;
	tempTransform.translate(camera.getViewPosition());

	//drawing idle goblin anim-
	image.drawSprite(goblinIdle, tempTransform);
}
