#pragma once

//Description: Responsible for loading and changing levels.. and ... basically most gameplay logic.
//				(Most of the code here should have been part of game class,
//              but some wrong technical design choices by me led them to here)


#include <Background.hpp>
#include <Camera.hpp>
#include <Player.hpp>
#include <Enemy.hpp>

#include <Button.hpp>
#include <Graphics/Font.hpp>
#include <Audio/Sound.hpp>

#include <vector>
#include <random>


namespace Graphics
{
	class Window; //Forward declaration to get reference to window
}

class Level {
public:
	enum class GameState
	{
		Menu,
		HelpScreen,
		Playing,
		Paused,
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

// Got help to implement processEvents(),onMouseMoved(), onResized() from~
// Source: https://github.com/jpvanoosten/SoftwareRasterizer/blob/main/samples/07-PixelAdventure/src/Game.cpp

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
	void beginState(GameState newState,GameState oldState);
	void endState(GameState oldState,GameState newState);

	void doMenu();
	void doHelp();
	void enemySteerAi(Enemy* enemy) const;
	void doPlaying(float deltaTime);
	void doPaused(float deltaTime);
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

	std::shared_ptr<Graphics::Image> startScreen{};
	Graphics::Sprite helpScreen{};
	Graphics::SpriteAnim coinUiAnim{};

	Audio::Sound punch;
	Audio::Sound hurtSFX;
	Audio::Sound coinSFX;
	Audio::Sound hpSFX;
	Audio::Sound mpSFX;

	Audio::Sound bgm1;
	Audio::Sound bgm2;
	Audio::Sound bgm3;

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
	Button helpButton{};
	std::vector<Button> levelButtons{};
	Button backButton{};

	int currentLevel = 1;

	//variables for 'Go' Text indicator
	float goTextTimer = 0.f;
	bool wasArena = false;
};
