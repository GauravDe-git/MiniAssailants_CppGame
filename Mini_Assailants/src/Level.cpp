#include "Level.hpp"
#include "Constants.hpp"
#include "Combat.hpp"
#include "ItemDrop.hpp"

#include "Graphics/Window.hpp"
#include <Graphics/ResourceManager.hpp>
#include "Graphics/Input.hpp"

#include <algorithm>

using namespace Graphics;
using namespace Math;

Level::Level(Window& _window)
	: window{ _window },
      gameState{GameState::Menu},
      camera{ glm::vec2{0,0} }, topEdgeCollision{0},
      tafelSans("assets/fonts/TafelSansPro-Bold.ttf", 20.0f)
{
    //Audio
    punch = Audio::Sound("assets/sounds/punch.wav");
    hurtSFX = Audio::Sound("assets/sounds/hurt.wav");
	hurtSFX.setVolume(0.5f);
	coinSFX = Audio::Sound("assets/sounds/coinpickup.wav");
    coinSFX.setVolume(0.1f);
    hpSFX = Audio::Sound("assets/sounds/hppickup.wav");
    hpSFX.setVolume(0.1f);
    mpSFX = Audio::Sound("assets/sounds/mppickup.wav");
    mpSFX.setVolume(0.1f);

    bgm1 = Audio::Sound("assets/sounds/stage1.ogg");
    bgm1.setVolume(0.1f);
    bgm1.setLooping(true);
    bgm2 = Audio::Sound("assets/sounds/stage2.ogg");
    bgm2.setVolume(0.1f);
    bgm2.setLooping(true);
    bgm3 = Audio::Sound("assets/sounds/stage3.ogg");
    bgm3.setVolume(0.1f);
    bgm3.setLooping(true);

    startScreen = ResourceManager::loadImage("assets/textures/startScreen.png");
	helpScreen = Sprite(ResourceManager::loadImage("assets/textures/helpScreen.png"), BlendMode::AlphaBlend);

    //Buttons
	SpriteSheet playBtnSheet{ "assets/textures/play_btn_sheet.png",95,36,0,0,BlendMode::AlphaBlend };
    playButton = Button{ playBtnSheet};

    SpriteSheet quitBtnSheet{ "assets/textures/quit_btn_sheet.png",95,36,0,0,BlendMode::AlphaBlend };
    quitButton = Button{ quitBtnSheet };

    SpriteSheet helpBtnSheet{ "assets/textures/help_btn_sheet.png",95,36,0,0,BlendMode::AlphaBlend };
    helpButton = Button{ helpBtnSheet };

    SpriteSheet backBtnSheet{ "assets/textures/back_btn_sheet.png",136,52,0,0,BlendMode::AlphaBlend };
    backButton = Button{ backBtnSheet };

    SpriteSheet lvl1BtnSheet{ "assets/textures/lvl1_btn_sheet.png",32,36,0,0,BlendMode::AlphaBlend };
    SpriteSheet lvl2BtnSheet{ "assets/textures/lvl2_btn_sheet.png",32,36,0,0,BlendMode::AlphaBlend };
    SpriteSheet lvl3BtnSheet{ "assets/textures/lvl3_btn_sheet.png",32,36,0,0,BlendMode::AlphaBlend };
	std::vector lvlBtnSheets{ lvl1BtnSheet, lvl2BtnSheet, lvl3BtnSheet }; /*Deduce type using CTAD C++17 */

    for (int i = 0; i < 3; ++i)
    {
        levelButtons.emplace_back(lvlBtnSheets[i], Transform2D{}, [this, i]{
            this->setLevel(i + 1);
            for (auto& lvlbtn : levelButtons)
                lvlbtn.setState(Button::State::Default);
            levelButtons[i].setState(Button::State::Selected);
            });
    }

    //Ui
    const auto coinSheet = ResourceManager::loadSpriteSheet("assets/textures/Coin_Sheet.png", 20, 20, 0, 0, BlendMode::AlphaBlend);
    coinUiAnim = SpriteAnim{ coinSheet, 10.0f };
}

void Level::loadLevelAssets()
{
    // Stop any music
    bgm1.stop();
	bgm2.stop();
	bgm3.stop();
    // Delete old enemies at start
    for (auto enemy : enemies)
    {
        delete enemy;
    }
    // Clear old entities and enemies
    entities.clear();
    enemies.clear();
    // Store the number of coins collected
    int coinsCollected = player.getCoins();

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

    player.setCoins(coinsCollected);
}

void Level::setLevel(int levelNumber)
{
	currentLevel = levelNumber;
    // Define different levels
    switch (levelNumber)
    {
    case 1:
        backgroundPath = "assets/textures/stage1.png";
        topEdgeCollision = 225;
        enemyInfos =  {
        {Enemy::Type::Goblin, {510,250}},
            {Enemy::Type::Goblin, {900,250}},
            {Enemy::Type::Goblin, {910,240}},
            {Enemy::Type::Goblin, {1300,250}},
            {Enemy::Type::Goblin, {1310,240}},
            {Enemy::Type::Skeleton, {1320,230}},
            {Enemy::Type::Golem, {1620,250}},
        };
        break;
    case 2:
        backgroundPath = "assets/textures/stage2.png";
        topEdgeCollision = 237;
        enemyInfos = {
        {Enemy::Type::Harpy, {510,250}},
            {Enemy::Type::Harpy, {900,250}},
            {Enemy::Type::Centaur, {910,240}},
            {Enemy::Type::Centaur, {1310,240}},
            {Enemy::Type::Skeleton, {1320,230}},
            {Enemy::Type::Gargoyle, {1620,250}},
        };
        break;
    case 3:
        backgroundPath = "assets/textures/stage3.png";
        topEdgeCollision = 210;
        enemyInfos = {
        {Enemy::Type::Cerberus, {530,250}},
            {Enemy::Type::Cerberus, {900,250}},
            {Enemy::Type::Cerberus, {910,240}},
            {Enemy::Type::Golem, {1310,240}},
            {Enemy::Type::Gargoyle, {1320,230}},
            {Enemy::Type::FlyingEye, {1620,250}},
        };
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
    case GameState::HelpScreen:
		doHelp();
		break;
    case GameState::Playing:
        updateEnemies(deltaTime);
        doPlaying(deltaTime);
        break;
    case GameState::Paused:
        doPaused(deltaTime);
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

void Level::draw(Image& image)
{
    // Draw level-specific elements (bg, player, enemies, etc.)
    switch (gameState)
    {
    case GameState::Menu:
        image.copy(*startScreen, 0, 0);
        playButton.draw(image);
		quitButton.draw(image);
        helpButton.draw(image);
        for (auto& lvlbtn : levelButtons)
        {
            lvlbtn.draw(image);
        }
        image.drawText(tafelSans, "LEVEL SELECT:", glm::vec2{ 103,238 }, Color::Black);
        image.drawText(tafelSans, "LEVEL SELECT:", glm::vec2{ 103,235 }, {230,157,107});
        break;
    case GameState::HelpScreen:
        image.drawSprite(helpScreen,{0,0});
		backButton.draw(image);
        break;
    case GameState::Playing:
        background.draw(image, camera);

        // Sorting order of drawing player/enemy based on their Y position
        // Solution using std::sort and lambda fn. suggested by Jeremiah van Oosten (@jpvanoosten)
        //** ranges algorithm suggested by resharper over the normal std::sort
        std::ranges::sort(entities, [](const Entity* a, const Entity* b) {return a->getPosition().y < b->getPosition().y; });
        for (auto entity : entities)
        {
            entity->draw(image, camera);
        }

        //GO text
        if (goTextTimer > 0.0f)
        {
            image.drawText(tafelSans, "GO->", glm::vec2{ SCREEN_WIDTH - 48, SCREEN_HEIGHT / 2 + 2 }, Color::Black);
            image.drawText(tafelSans, "GO->", glm::vec2{ SCREEN_WIDTH - 50, SCREEN_HEIGHT / 2 }, Color::Yellow);
        }

        // Ui Coin
        image.drawSprite(coinUiAnim, glm::vec2{SCREEN_WIDTH - 150.f, 1.f});
        image.drawText(Font::Default, std::to_string(player.getCoins()), glm::vec2{ SCREEN_WIDTH - 115.f, 5.f }, Color::Yellow);
        break;
	case GameState::Paused:
        background.draw(image, camera);
        player.draw(image, camera);
        for (auto& enemy : enemies)
        {
            enemy->draw(image, camera);
        }
        image.drawSprite(coinUiAnim, glm::vec2{ SCREEN_WIDTH - 150.f, 1.f });
        image.drawText(Font::Default, std::to_string(player.getCoins()), glm::vec2{ SCREEN_WIDTH - 115.f, 5.f }, Color::Yellow);

        image.drawText(tafelSans, "Game is Paused, press P to unpause", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2.3 - 1.5f }, Color::Black);
        image.drawText(tafelSans, "Game is Paused, press P to unpause", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2.3 + 3.5f }, Color::Black);
        image.drawText(tafelSans, "Game is Paused, press P to unpause", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2.3 }, Color::Yellow);

        image.drawText(tafelSans, "Press Q to go back to main menu", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 - 1.5f }, Color::Black);
        image.drawText(tafelSans, "Press Q to go back to main menu", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 3.5f }, Color::Black);
        image.drawText(tafelSans, "Press Q to go back to main menu", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 }, Color::Yellow);
        break;
    case GameState::GameOver:
        background.draw(image, camera);
        for(auto& enemy : enemies)
        {
            enemy->draw(image, camera);
        }
        image.drawText(tafelSans, "Game Over, press Enter to Continue", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 - 1.5f }, Color::Black);
        image.drawText(tafelSans, "Game Over, press Enter to Continue", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 3.5f }, Color::Black);
        image.drawText(tafelSans, "Game Over, press Enter to Continue", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 }, Color::Red);

        image.drawText(tafelSans, "Collected " + std::to_string(player.getCoins()) + " coins", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 23.f }, Color::Black);
        image.drawText(tafelSans, "Collected " + std::to_string(player.getCoins()) + " coins", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 20.f }, Color::Yellow);
        break;
    case GameState::Win:
        background.draw(image, camera);
        player.draw(image, camera);
        std::string winText = "Congrats on clearing game!";
		std::string winText2 = "Press Enter to go back";
        if (currentLevel < 3)
        {
            winText = "Congrats on clearing level " + std::to_string(currentLevel);
			winText2 = "Press Enter to go to next level";
        }
        image.drawText(tafelSans, winText, glm::vec2{ SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 3.5f }, Color::Black);
        image.drawText(tafelSans, winText, glm::vec2{ SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 }, Color::Green);

        image.drawText(tafelSans, winText2, glm::vec2{ SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 23.5f }, Color::Black);
        image.drawText(tafelSans, winText2, glm::vec2{ SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 20.f }, Color::Green);

        image.drawText(tafelSans, "Collected " + std::to_string(player.getCoins()) + " coins", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 43.f }, Color::Black);
        image.drawText(tafelSans, "Collected " + std::to_string(player.getCoins()) + " coins", glm::vec2{ SCREEN_WIDTH / 2 - 110, SCREEN_HEIGHT / 2 + 40.f }, Color::Yellow);
        break;
    }
}

void Level::setState(GameState newState)
{
    if (newState != gameState)
    {
        beginState(newState, gameState);
        endState(gameState, newState);
        gameState = newState;
    }
}

// Got help to implement processEvents(),onMouseMoved(), onResized() from~
// Source: https://github.com/jpvanoosten/SoftwareRasterizer/blob/main/samples/07-PixelAdventure/src/Game.cpp

void Level::processEvents(const Event& e)
{
    // Copy the event so you can modify it.
    Event event = e;

    switch (event.type)
    {
    case Event::MouseMoved:
        onMouseMoved(event.mouseMove);
        break;
    case Event::Resize:
        onResized(event.resize);
        break;
    }

    if (gameState == GameState::Menu)
    {
        playButton.processEvents(event);
        quitButton.processEvents(event);
        helpButton.processEvents(event);
        for (auto& lvlbtn : levelButtons)
        {
            lvlbtn.processEvents(event);
        }
    }
    else if(gameState == GameState::HelpScreen)
        backButton.processEvents(event);
}

void Level::onMouseMoved(MouseMovedEventArgs& args)
{
    // Compute the mouse position relative to the game screen.
    const glm::vec2 scale{
        static_cast<float>(SCREEN_WIDTH) / static_cast<float>(gameRect.width),
        static_cast<float>(SCREEN_HEIGHT) / static_cast<float>(gameRect.height)
    };
    const glm::vec2 offset{ gameRect.topLeft() };

    args.x = std::lround((static_cast<float>(args.x) - offset.x) * scale.x);
    args.y = std::lround((static_cast<float>(args.y) - offset.y) * scale.y);

    mousePos = { args.x, args.y };
}

void Level::onResized(ResizeEventArgs& args)
{
    // Implement the logic to update gameRect based on the new window size.
    const float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    const float scaleWidth = static_cast<float>(args.width) / static_cast<float>(SCREEN_WIDTH);
    const float scaleHeight = static_cast<float>(args.height) / static_cast<float>(SCREEN_HEIGHT);

    int width;
    int height;

    //logic for updating width and height based on aspect ratio and scaling.
    if (scaleWidth < scaleHeight)
    {
        width = args.width;
        height = static_cast<int>(static_cast<float>(width) / aspectRatio);
    }
    else
    {
        height = args.height;
        width = static_cast<int>(static_cast<float>(height) * aspectRatio);
    }

    gameRect = {
        (args.width - width) / 2,
        (args.height - height) / 2,
        width, height
    };

    // Update any UI elements or positions that depend on the game rectangle.
    
    playButton.setTransform(Transform2D{ { 120, 170 } });
    helpButton.setTransform(Transform2D{ { 220, 170 } });
    quitButton.setTransform(Transform2D{ { 320, 170 } });
    for (int i = 0; i < 3; ++i)
    {
        levelButtons[i].setTransform(Transform2D{ { 230 + i * 35, 215} });
    }
    backButton.setTransform(Transform2D{ { 375, 220 },{0.7f,0.7f} });
}

void Level::beginState(GameState newState, GameState oldState)
{
    switch (newState)
    {
    case GameState::Menu:
        // Stop any music
        bgm1.stop();
        bgm2.stop();
        bgm3.stop();
        //Reset coins
        player.setCoins(0);
        break;
    case GameState::Playing:
        if (!isFirstLoad && oldState != GameState::Paused)
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

void Level::endState(GameState oldState,GameState newState)
{
    switch (oldState)
    {
    case GameState::Menu:
        break;
    case GameState::Playing:
        if (newState != GameState::Paused)
        {
            entities.clear();
        }
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
	playButton.setCallback([this] {setState(GameState::Playing); });
	helpButton.setCallback([this] {setState(GameState::HelpScreen); });
    quitButton.setCallback([this] {window.destroy(); });
}

void Level::doHelp()
{
	backButton.setCallback([this] {setState(GameState::Menu); });
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
    switch (currentLevel)
    {
    case 1:
        if(!bgm1.isPlaying())
			bgm1.play();
        break;
    case 2:
        if (!bgm2.isPlaying())
            bgm2.play();
        break;
    case 3:
        if (!bgm3.isPlaying())
            bgm3.play();
        break;
    }

    camera.update(deltaTime, player.getPosition(), player.getVelocity(), player.isAttacking());
    player.update(deltaTime);
    coinUiAnim.update(deltaTime);

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
            //Always drop a coin when an enemy is defeated
                ItemDrop* coin = new ItemDrop{ enemy->getPosition() + glm::vec2{0, -20}, ItemDrop::Type::Coin };  // Drop the coin a bit upward
            entities.push_back(coin);

            //Random chance to drop a potion
            if (randDist(randGen))
            {
                const ItemDrop::Type type = randDist(randGen) ? ItemDrop::Type::HP : ItemDrop::Type::MP;
                ItemDrop* potion = new ItemDrop{ enemy->getPosition(), type };
                entities.push_back(potion);
            }
            enemy->setState(Enemy::State::None);
        }

        //Player and Enemy Combat Interaction Logic
        if (player.getHP() <= 0)
        {
            setState(GameState::GameOver);
            std::erase(entities, &player);
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
                hurtSFX.play();
            }
        }
    }

    //Switch Camera state based on Enemy aggro
	if (isEnemyAggroing)
	{
		camera.setState(Camera::State::arena);
        wasArena = true;
	}
	else if (player.getPosition().x > 240.f)
	{
		camera.setState(Camera::State::scrolling);

        // If the camera just switched from the arena mode to the scrolling state
        if (wasArena)
        {
            goTextTimer = 3.0f;
        }
        wasArena = false;
	}
    // Decrease the 'Go' Text timer
    if (goTextTimer > 0.0f)
    {
        goTextTimer -= deltaTime;
    }

    // Picking up the Items dropped by Enemies
  
    // Adding the items to remove in a vector, and delete them later.
    // Deleting them within the iteration loop led to undefined behaviour.
    std::vector<ItemDrop*> itemsToRemove; 

    for (auto& entity : entities) {
        if (ItemDrop* item = dynamic_cast<ItemDrop*>(entity)) {
            item->update(deltaTime);

            if (item->canPickUp() && player.getAABB().intersect(item->getAABB())) {
                switch (item->getType())
                {
                case ItemDrop::Type::HP:
                    hpSFX.play();
                    player.setHP(std::min(player.getHP() + item->getValue(), player.getMaxHP()));
                    break;
                case ItemDrop::Type::MP:
                    mpSFX.play();
                    player.setMP(std::min(player.getMP() + item->getValue(), player.getMaxMP()));
                    break;
                case ItemDrop::Type::Coin:
                    coinSFX.play();
                    player.setCoins(player.getCoins() + item->getValue());
                    break;
                }
                // Add to the list of items to remove
                itemsToRemove.push_back(item);
            }
        }
    }

    // Remove items after iterating
    for (ItemDrop* item : itemsToRemove) {
        // Erase and delete the item
        std::erase(entities, item);
        delete item;
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
    if (Input::getKeyDown(KeyCode::P))
    {
        setState(GameState::Paused);
    }

    if (enemies.empty())
    {
    	setState(GameState::Win);
    }
}

void Level::doPaused(float deltaTime)
{
	// Any logic that needs to happen when the game is paused
	if (Input::getKeyDown(KeyCode::P))
		setState(GameState::Playing);

    if (Input::getKeyDown(KeyCode::Q))
        setState(GameState::Menu);

}

void Level::doGameOver()
{
    // Any logic that needs to happen when the game is over
    for (const auto enemy : enemies)
    {
        enemy->setTarget(nullptr);
    }

    if (Input::getKeyDown(KeyCode::Enter))
        setState(GameState::Menu);
}

void Level::doWin()
{
    // Any logic that needs to happen when the player wins the level
    if (Input::getKeyDown(KeyCode::Enter))
    {
        if(currentLevel >= 3)
        {
            setState(GameState::Menu);
        }
        else
        {
            setLevel(++currentLevel);
            setState(GameState::Playing);
        }
    }
}
