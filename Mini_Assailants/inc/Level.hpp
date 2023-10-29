#pragma once

#include <Background.hpp>
#include <Camera.hpp>
#include <Player.hpp>

class Level {
public:
	Level();

	Level(const std::string& backgroundPath, int topEdgeCollision);

	void LoadLevelAssets();
	void SetLevel(int levelNumber);

	void Update(float deltaTime);
	void Draw(Graphics::Image& image, const glm::vec2& offset);

	Player& getPlayer() { return player; }

private:
	Background background;
	Player player;
	std::string backgroundPath;
	int topEdgeCollision;
};