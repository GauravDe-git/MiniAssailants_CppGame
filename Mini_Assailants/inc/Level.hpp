#pragma once

#include <Background.hpp>
#include <Camera.hpp>
#include <Player.hpp>
#include <Enemy.hpp>

#include <Button.hpp>
#include <Graphics/Font.hpp>
#include <Audio/Sound.hpp>

#include <vector>
#include <random>

//Level class: Responsible for both loading/changing levels
//			   and changing between Game States.

namespace Graphics
{
	class Window; //Forward declaration to get reference to window
}

class Level {
public:
	enum class GameState
	{
		Menu,
		Playing,
		Win,
		GameOver
	};
	Level(Graphics::Window& _window);

	void loadLevelAssets();
	void setLevel(int levelNumber);

	void update(float deltaTime);
	void draw(Graphics::Image& image);

	GameState getGameState() const { return gameState; }
	void setState(GameState newState);

	void processEvents(const Graphics::Event& e);
	void onMouseMoved(Graphics::MouseMovedEventArgs& args);
	void onResized(Graphics::ResizeEventArgs& args);
private:
	struct EnemyInfo
	{
		Enemy::Type type;
		glm::vec2 position;
	};

	void updateEnemies(float deltaTime) const;
	void beginState(GameState newState);
	void endState(GameState oldState);

	void doMenu();
	void enemySteerAi(Enemy* enemy) const;
	void doPlaying(float deltaTime);
	void doGameOver();
	void doWin();

	Graphics::Window& window;
	GameState gameState{};
	Background background{};
	Player player{};
	Camera camera{};
	std::vector<Enemy*> enemies;
	std::vector<Entity*> entities;
	std::vector<EnemyInfo> enemyInfos;

	Audio::Sound punch;
	Audio::Sound swordSlash;
	std::shared_ptr<Graphics::Image> startScreen{};

	std::string backgroundPath;
	int topEdgeCollision;
	bool isFirstLoad{ true };

	std::mt19937 randGen{ std::random_device{}() };
	std::bernoulli_distribution randDist{0.5};

	Graphics::Font tafelSans;
	// The game rectangle in the Window's coordinate frame.
	// Used for translating mouse coordinates.
	Math::RectI gameRect;

	// Translated mouse position.
	glm::ivec2 mousePos;

	//buttons
	Button playButton{};
	Button quitButton{};
	Button changelvlButton{};
	std::vector<Button> levelButtons{};

	int currentLevel = 1;
};
