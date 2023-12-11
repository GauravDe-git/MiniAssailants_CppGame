#include "Level.hpp"
#include "Constants.hpp"
#include "Combat.hpp"

#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>

#include <algorithm>

#include "Graphics/Input.hpp"

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
    // Add other assets (enemies, items etc.) later
    enemy = Enemy{ {SCREEN_WIDTH / 2 + 180,SCREEN_HEIGHT - 30},Enemy::Type::Skeleton };
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

    switch (gameState)
    {
    case GameState::Menu:
        doMenu();
        break;
    case GameState::Playing:
        doPlaying(deltaTime);
        break;
    case GameState::GameOver:
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
        //** ranges algorithm suggested by resharper over the normal std::sort
        std::ranges::sort(entities, [](const Entity* a, const Entity* b) {return a->getPosition().y < b->getPosition().y; });
        for (const auto entity : entities)
        {
            entity->draw(image, camera);
        }
        break;
    case GameState::GameOver:
        enemy.draw(image, camera);
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

void Level::doPlaying(float deltaTime)
{
    camera.update(deltaTime, player.getPosition(), player.getVelocity(), player.isAttacking());
    player.update(deltaTime);
    enemy.setTarget(&player);

    // Check if the enemy is dead
    if (enemy.getState() == Enemy::State::None) {
        // Remove the dead enemy from the entities vector
        std::erase(entities, &enemy);
        setState(GameState::Win);
    }

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
