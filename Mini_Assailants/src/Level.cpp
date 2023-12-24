#include "Level.hpp"
#include "Constants.hpp"
#include "Combat.hpp"
#include "PotionDrop.hpp"

#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include "Graphics/Input.hpp"

#include <algorithm>

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
    player = Player{ {SCREEN_WIDTH / 2 -200,(SCREEN_HEIGHT - 10)}};
    player.setCamera(&camera);

    player.setTopEdgeCollision(topEdgeCollision);
    entities.push_back(&player);
    
    for (const auto& enemyInfo : enemyInfos)
    {
        enemies.push_back(new Enemy(enemyInfo.position,enemyInfo.type));
    }

    for (const auto& enemy: enemies)
    {
        entities.push_back(enemy);
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

void Level::updateEnemies(float deltaTime) const
{
	for (auto& enemy: enemies)
	{
		enemy->update(deltaTime);
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
            enemy->draw(image, camera);
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
        //enemies.clear();
        break;
    case GameState::GameOver:
		enemies.clear();
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

void Level::enemySteerAi(Enemy* enemy) const
{
	//Enemy steering behaviour when colliding with itself
	for (const auto& otherEnemy : enemies)
	{
		if(otherEnemy != enemy && enemy->getCollisionCircle().intersect(otherEnemy->getCollisionCircle()))
		{
			auto direction = glm::normalize(otherEnemy->getPosition() - enemy->getPosition());

			//set velocity to move in opposite direction
			enemy->setVelocity(-direction * enemy->getSpeed());

			//immediately resolve the collision by slightly moving the enemy
			const float displacement = enemy->getCollisionCircle().radius + otherEnemy->getCollisionCircle().radius - glm::distance(enemy->getPosition(), otherEnemy->getPosition());
			enemy->setPosition(enemy->getPosition() - direction * displacement);
		}
	}
	// Set the enemy's facing direction based on the player's position
	const glm::vec2 directionToPlayer = glm::normalize(player.getPosition() - enemy->getPosition());
	enemy->setFacingDirection(directionToPlayer);
}

void Level::doPlaying(float deltaTime)
{
    camera.update(deltaTime, player.getPosition(), player.getVelocity(), player.isAttacking());
    player.update(deltaTime);

    bool isEnemyAggroing = false;

    // Using iterator based loop, instead of range based loop due to some bug
    // That was preventing proper deletion of enemies 
    for (auto enemy_iter = enemies.begin(); enemy_iter != enemies.end(); ++enemy_iter)
    {
	    const auto& enemy = *enemy_iter;

	    const float distanceToPlayer = distance(player.getPosition(), enemy->getPosition());
        if (distanceToPlayer <= 250.f)
        {
            enemy->setTarget(&player);
            isEnemyAggroing = true;
        }
        else
        {
            enemy->setTarget(nullptr);
        }

        enemySteerAi(enemy);

        //Enemy Potion Drop Logic
        if (enemy->getState() == Enemy::State::JustDefeated)
        {
            if (randDist(randGen))
            {
                const PotionDrop::Type type = randDist(randGen) ? PotionDrop::Type::HP : PotionDrop::Type::MP;
                PotionDrop* potion = new PotionDrop{ enemy->getPosition(), type };
                entities.push_back(potion);
            }
            enemy->setState(Enemy::State::None);
        }

        //Player and Enemy Combat Interaction Logic
        if (player.getHP() <= 0)
        {
            setState(GameState::GameOver);
            std::erase(entities, &player);
            //enemy->setTarget(nullptr);
        }
        else {
            if (player.isAttacking() && enemy->getState() != Enemy::State::Hurt
                && enemy->getAABB().intersect(player.getAttackCircle()))
            {
                Combat::attack(player, *enemy, player.getCurrentAtkType());
                punch.play();
            }
            if (enemy->isAttacking() && player.getState() != Player::State::Hurt
                && player.getAABB().intersect(enemy->getAttackCircle()))
            {
                Combat::attack(*enemy, player);
                swordSlash.play();
            }
        }
    }

    //Switch Camera state based on Enemy aggro
	if (isEnemyAggroing)
	{
		camera.setState(Camera::State::arena);
	}
	else if (player.getPosition().x > 240.f)
	{
		camera.setState(Camera::State::scrolling);
	}

    // Picking up the Potions dropped by Enemies
    for (const auto entity : entities)
    {
		// Check if the entity is a potion
		if (PotionDrop* potion = dynamic_cast<PotionDrop*>(entity))
		{
            potion->update(deltaTime);

			if (potion->canPickUp() && player.getAABB().intersect(potion->getAABB()))
			{
				switch (potion->getType())
				{
				case PotionDrop::Type::HP:
                    player.setHP(std::min(player.getHP() + potion->getValue(), player.getMaxHP()));
					break;
				case PotionDrop::Type::MP:
					player.setMP(std::min(player.getMP() + potion->getValue(), player.getMaxMP()));
					break;
				}
                std::erase(entities, potion);
                delete potion;
			}
		}
    }

    // Removing Enemies when they are in none state.
    for (auto iter = enemies.begin(); iter != enemies.end();)
    {
        auto enemy = *iter;
        if (enemy->getState() == Enemy::State::None)
        {
            iter = enemies.erase(iter);
            std::erase(entities, enemy);
            delete enemy;
        }
        else
        {
            ++iter;
        }
    }

    if (enemies.empty())
    {
        setState(GameState::Win);
    }
}

void Level::doGameOver()
{
    // Any logic that needs to happen when the game is over
    for (const auto enemy : enemies)
    {
        enemy->setTarget(nullptr);
    }

    if (Graphics::Input::getKeyDown(Graphics::KeyCode::Enter))
        setState(GameState::Menu);
}

void Level::doWin()
{
    // Any logic that needs to happen when the player wins the level
    if (Graphics::Input::getKeyDown(Graphics::KeyCode::Enter))
        setState(GameState::Menu);
}
