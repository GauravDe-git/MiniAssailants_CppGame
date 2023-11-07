#pragma once

#include <Background.hpp>
#include <Camera.hpp>
#include <Player.hpp>
#include <Enemy.hpp>

#include <vector>

class Level {
public:
	Level();

	void loadLevelAssets();
	void setLevel(int levelNumber);

	void update(float deltaTime);
	void draw(Graphics::Image& image);

	Player& getPlayer() { return player; }

private:
	Background background;
	Player player;
	Enemy enemy;
	Camera camera;
	std::vector<Entity*> entities;

	std::string backgroundPath;
	int topEdgeCollision;
};