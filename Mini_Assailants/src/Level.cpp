#include "Level.hpp"
#include "Constants.hpp"
#include "Combat.hpp"

#include <algorithm>
#include <iostream>

Level::Level()
	:backgroundPath{}, topEdgeCollision{ 0 }, camera{ glm::vec2{0,0} }
{}

void Level::loadLevelAssets()
{
	//Load level-specific assets
	background = Background(backgroundPath);
	player = Player{ {SCREEN_WIDTH / 2,(SCREEN_HEIGHT - 10)} };

	player.setTopEdgeCollision(topEdgeCollision);
	// Add other assets (eneimes, items etc.) later
	enemy = Enemy{ {SCREEN_WIDTH / 2 + 100,SCREEN_HEIGHT - 30},Enemy::Type::Skeleton };
	entities.clear();
	entities.push_back(&player);
	entities.push_back(&enemy);
}

void Level::setLevel(int levelNumber)
{
	// Define different levels
	switch (levelNumber)
	{
	case 1:
		backgroundPath = "assets/textures/stage1.png";
		topEdgeCollision = 225;
		break;

	// Add more cases for different levels
	}

	//Load level-specific assets based on updated props
	loadLevelAssets();
}

void Level::update(float deltaTime)
{
	// Update game logic here (eg. player, enemies)
	camera.update(deltaTime, player.getPosition(), player.getVelocity(), player.isAttacking());

	player.update(deltaTime);
	enemy.update(deltaTime);
	enemy.setTarget(&player);
	if (player.isAttacking() && enemy.getState() != Enemy::State::Hurt && enemy.getAABB().intersect(player.getAttackCircle()))
	{
		Combat::attack(player, enemy, player.getCurrentAtkType());
		std::cout << enemy.getHp() << "\n";
	}
	if (enemy.getState() == Enemy::State::Attack && player.getState() != Player::State::Hurt && player.getAABB().intersect(enemy.getAttackCircle()))
	{
		Combat::attack(enemy, player);
	}
}

void Level::draw(Graphics::Image& image)
{
	// Draw level-specific elements (bg, player, enemies, etc.)
	background.draw(image, camera);

	//sorting order of drawing player/enemy based on their Y position
	std::sort(entities.begin(), entities.end(), [](const Entity* a, const Entity* b) {return a->getPosition().y < b->getPosition().y; });
	for (auto entity : entities)
	{
		entity->draw(image, camera);
	}
	
}
