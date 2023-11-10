#include "Level.hpp"
#include "Constants.hpp"
#include "Combat.hpp"

#include <Graphics/Font.hpp>

#include <algorithm>
#include <iostream>

Level::Level()
	:backgroundPath{}, topEdgeCollision{ 0 }, 
	camera{ glm::vec2{0,0} }, gameState{ GameState::Playing }
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
	enemy.update(deltaTime);
	if (gameState == GameState::Playing) {
		camera.update(deltaTime, player.getPosition(), player.getVelocity(), player.isAttacking());
		player.update(deltaTime);
		enemy.setTarget(&player);

		// Check if the enemy is dead
		if (enemy.getState() == Enemy::State::None) {
			// Remove the dead enemy from the entities vector
			entities.erase(std::remove(entities.begin(), entities.end(), &enemy), entities.end());
			gameState = GameState::Won;
		}

		if (player.getHP() <= 0)
		{
			gameState = GameState::GameOver;
			entities.erase(std::remove(entities.begin(), entities.end(), &player), entities.end());
			enemy.setTarget(nullptr);
		}
		else{
			if (player.isAttacking() && enemy.getState() != Enemy::State::Hurt && enemy.getAABB().intersect(player.getAttackCircle()))
			{
				Combat::attack(player, enemy, player.getCurrentAtkType());
				std::cout << enemy.getHp() << "\n";
			}
			if (enemy.isAttacking() && player.getState() != Player::State::Hurt && player.getAABB().intersect(enemy.getAttackCircle()))
			{
				Combat::attack(enemy, player);
			}
		}
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

	if (gameState == GameState::GameOver)
	{
		image.drawText(Graphics::Font::Default, "Game Over", glm::vec2{ SCREEN_WIDTH/2 - 70, SCREEN_HEIGHT/2 }, Graphics::Color::Red);
	}
	else if (gameState == GameState::Won)
	{
		image.drawText(Graphics::Font::Default, "You Won", glm::vec2{ SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 }, Graphics::Color::Green);
	}
}
