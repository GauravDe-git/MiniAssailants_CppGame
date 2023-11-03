#pragma once

#include <Background.hpp>
#include <Camera.hpp>
#include <Player.hpp>

class Level {
public:
	Level();

	void LoadLevelAssets();
	void SetLevel(int levelNumber);

	void Update(float deltaTime);
	void Draw(Graphics::Image& image);

	Player& getPlayer() { return player; }

private:
	Background background;
	Player player;
	Camera camera;
	std::string backgroundPath;
	int topEdgeCollision;
};