#pragma once

#include <Background.hpp>
#include <Camera.hpp>
#include <Player.hpp>
#include <Enemy.hpp>

#include <Audio/Sound.hpp>

#include <vector>

//Level class: Responsible for both loading/changing levels
//			   and changing between Game States.

class Level {
public:
	enum class GameState
	{
		Menu,
		Playing,
		Win,
		GameOver
	};
	Level();

	void loadLevelAssets();
	void setLevel(int levelNumber);

	void update(float deltaTime);
	void draw(Graphics::Image& image);

	GameState getGameState() const { return gameState; }
	void setState(GameState newState);
private:
	void beginState(GameState newState);
	void endState(GameState oldState);

	void doMenu();
	void doPlaying(float deltaTime);
	void doGameOver();
	void doWin();

	GameState gameState{};
	Background background{};
	Player player{};
	Enemy enemy{};
	Camera camera{};
	std::vector<Entity*> entities;

	Audio::Sound punch;
	Audio::Sound swordSlash;
	std::shared_ptr<Graphics::Image> startScreen{};

	std::string backgroundPath;
	int topEdgeCollision;
	bool isFirstLoad{ true };
};