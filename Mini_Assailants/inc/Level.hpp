#pragma once

#include <Background.hpp>
#include <Camera.hpp>
#include <Player.hpp>
#include <Enemy.hpp>

#include <Audio/Sound.hpp>

#include <vector>

class Level {
public:
	enum class GameState
	{
		Playing,
		Won,
		GameOver
	};
	Level();

	void loadLevelAssets();
	void setLevel(int levelNumber);

	void update(float deltaTime);
	void draw(Graphics::Image& image);

	GameState getGameState() const { return gameState; }
private:
	GameState gameState{};
	Background background{};
	Player player{};
	Enemy enemy{};
	Camera camera{};
	std::vector<Entity*> entities;

	Audio::Sound punch;
	Audio::Sound swordSlash;

	std::string backgroundPath;
	int topEdgeCollision;
};