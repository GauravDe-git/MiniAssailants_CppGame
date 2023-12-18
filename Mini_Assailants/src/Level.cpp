#include "Level.hpp"
#include "Constants.hpp"
#include "Combat.hpp"
#include "PotionDrop.hpp"

#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include "Graphics/Input.hpp"

#include <algorithm>
#include <random>

Level::Level()
    : gameState{GameState::Menu},
      camera{ glm::vec2{0,0} }, topEdgeCollision{0}
{
    punch = Audio::Sound("assets/sounds/punch.wav");
    swordSlash = Audio::Sound("assets/sounds/swordSlash.wav");
    startScreen = Graphics::ResourceManager::loadImage("assets/textures/startScreen.png");
}

void Level::loadLevelAssets()
{
    //Load level-specific assets
    background = Background(backgroundPath);
    player = Player{ {SCREEN_WIDTH / 2 -200,(SCREEN_HEIGHT - 10)} };

    player.setTopEdgeCollision(topEdgeCollision);
    entities.push_back(&player);
    
    for (const auto& enemyInfo : enemyInfos)
    {
        enemies.emplace_back(enemyInfo.position,enemyInfo.type);
    }

    for (auto& enemy: enemies)
    {
        entities.push_back(&enemy);
    }
}

void Level::setLevel(int levelNumber)
{
    // Define different levels
    switch (levelNumber)
    {
    case 1:
        backgroundPath = "assets/textures/stage1.png";
        topEdgeCollision = 225;
        enemyInfos =  { {Enemy::Type::Goblin, {420,250}},
                        {Enemy::Type::Skeleton, {820,250}},
        {Enemy::Type::Goblin, {1020,250}},};
        break;
        
    // Add more cases for different levels
    }

    //Load level-specific assets based on updated props
    loadLevelAssets();
}

void Level::updateEnemies(float deltaTime)
{
	for (auto& enemy: enemies)
	{
		enemy.update(deltaTime);
	}
}

void Level::update(float deltaTime)
{
    switch (gameState)
    {
    case GameState::Menu:
        doMenu();
        break;
    case GameState::Playing:
        updateEnemies(deltaTime);
        doPlaying(deltaTime);
        break;
    case GameState::GameOver:
        updateEnemies(deltaTime);
        doGameOver();
        break;
    case GameState::Win:
        doWin();
        break;
    }
}

void Level::draw(Graphics::Image& image)
{
    // Draw level-specific elements (bg, player, enemies, etc.)
    background.draw(image, camera);

    switch (gameState)
    {
    case GameState::Menu:
        image.copy(*startScreen, 0, 0);
        break;
    case GameState::Playing:
        //sorting order of drawing player/enemy based on their Y position
        // Solution using std::sort and lambda fn. suggested by Jeremiah
        //** ranges algorithm suggested by resharper over the normal std::sort
        std::ranges::sort(entities, [](const Entity* a, const Entity* b) {return a->getPosition().y < b->getPosition().y; });
        for (const auto entity : entities)
        {
            entity->draw(image, camera);
        }
        break;
    case GameState::GameOver:
        for(auto& enemy : enemies)
        {
            enemy.draw(image, camera);
        }
        image.drawText(Graphics::Font::Default, "Game Over", glm::vec2{ SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 + 1.5f }, Graphics::Color::Black);
        image.drawText(Graphics::Font::Default, "Game Over", glm::vec2{ SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 }, Graphics::Color::Red);
        break;
    case GameState::Win:
        player.draw(image, camera);
        image.drawText(Graphics::Font::Default, "You Win", glm::vec2{ SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 + 1.5f }, Graphics::Color::Black);
        image.drawText(Graphics::Font::Default, "You Win", glm::vec2{ SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 }, Graphics::Color::Green);
        break;
    }
}

void Level::setState(GameState newState)
{
    if (newState != gameState)
    {
        beginState(newState);
        endState(gameState);
        gameState = newState;
    }
}

void Level::beginState(GameState newState)
{
    switch (newState)
    {
    case GameState::Menu:
        break;
    case GameState::Playing:
        if (!isFirstLoad)
        {
            loadLevelAssets();
            camera.setPosition(glm::vec2{ 0,0 });
        }
        break;
    case GameState::GameOver:
        isFirstLoad = false;
        break;
    case GameState::Win:
        isFirstLoad = false;
        break;
    }
}

void Level::endState(GameState oldState)
{
    switch (oldState)
    {
    case GameState::Menu:
        break;
    case GameState::Playing:
        entities.clear();
        enemies.clear();
        break;
    case GameState::GameOver:
        break;
    case GameState::Win:
        break;
    }
}

void Level::doMenu()
{
    if (Graphics::Input::getKeyDown(Graphics::KeyCode::Enter))
        setState(GameState::Playing);
}

void Level::enemySteerAi(std::vector<Enemy>::value_type& enemy) const
{
	//Enemy steering behaviour when colliding with itself
	for (const auto& otherEnemy : enemies)
	{
		if(&otherEnemy != &enemy && enemy.getCollisionCircle().intersect(otherEnemy.getCollisionCircle()))
		{
			auto direction = glm::normalize(otherEnemy.getPosition() - enemy.getPosition());

			//set velocity to move in opposite direction
			enemy.setVelocity(-direction * enemy.getSpeed());

			//immediately resolve the collision by slightly moving the enemy
			const float displacement = enemy.getCollisionCircle().radius + otherEnemy.getCollisionCircle().radius - glm::distance(enemy.getPosition(), otherEnemy.getPosition());
			enemy.setPosition(enemy.getPosition() - direction * displacement);
		}
	}
	// Set the enemy's facing direction based on the player's position
	const glm::vec2 directionToPlayer = glm::normalize(player.getPosition() - enemy.getPosition());
	enemy.setFacingDirection(directionToPlayer);
}

void Level::doPlaying(float deltaTime)
{
    bool allEnemiesDefeated{true};
    camera.update(deltaTime, player.getPosition(), player.getVelocity(), player.isAttacking());
    player.update(deltaTime);

    for (auto& enemy : enemies)
    {
        enemy.setTarget(&player);

        enemySteerAi(enemy);

        // Check if the enemy is dead
        if (enemy.getState() != Enemy::State::None)
        {
            allEnemiesDefeated = false;
        }
        else
        {
            // Remove the dead enemy from the entities vector
            //Bug: Not able to properly erase the enemy like this
            // (Punch effect still playing after the enemy is removed)
            std::erase(entities, &enemy);
        }

        //Enemy Potion Drop Logic
        if (enemy.getState() == Enemy::State::JustDefeated)
        {
            std::mt19937 randGen{ std::random_device{}() };
            std::bernoulli_distribution randDist(0.5);
            if (randDist(randGen))
            {
	            const PotionDrop::Type type = randDist(randGen) ? PotionDrop::Type::HP : PotionDrop::Type::MP;
				PotionDrop* potion = new PotionDrop{ enemy.getPosition(), type };
				entities.push_back(potion);
            }
            enemy.setState(Enemy::State::None);
        }

        //Player and Enemy Combat Interaction Logic
        if (player.getHP() <= 0)
        {
            setState(GameState::GameOver);
            std::erase(entities, &player);
            enemy.setTarget(nullptr);
        }
        else {
            if (player.isAttacking() && enemy.getState() != Enemy::State::Hurt
                && enemy.getAABB().intersect(player.getAttackCircle()))
            {
                Combat::attack(player, enemy, player.getCurrentAtkType());
                punch.play();
            }
            if (enemy.isAttacking() && player.getState() != Player::State::Hurt
                && player.getAABB().intersect(enemy.getAttackCircle()))
            {
                Combat::attack(enemy, player);
                swordSlash.play();
            }
        }
    }
    if (allEnemiesDefeated)
    {
        setState(GameState::Win);
    }
}

void Level::doGameOver()
{
    // Any logic that needs to happen when the game is over
    if (Graphics::Input::getKeyDown(Graphics::KeyCode::Enter))
        setState(GameState::Menu);
}

void Level::doWin()
{
    // Any logic that needs to happen when the player wins the level
    if (Graphics::Input::getKeyDown(Graphics::KeyCode::Enter))
        setState(GameState::Menu);
}
