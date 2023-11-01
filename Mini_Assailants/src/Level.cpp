#include <Level.hpp>
#include <Constants.hpp>

Level::Level() = default;

void Level::LoadLevelAssets()
{
	//Load level-specific assets
	background = Background(backgroundPath);
	player = Player{ {SCREEN_WIDTH / 2,(SCREEN_HEIGHT - 10)} };

	player.setTopEdgeCollision(topEdgeCollision);
	// Add other assets (eneimes, items etc.) later
}

void Level::SetLevel(int levelNumber)
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
	LoadLevelAssets();
}

void Level::Update(float deltaTime)
{
	// Update game logic here (eg. player, enemies)
	player.update(deltaTime);
}

void Level::Draw(Graphics::Image& image, const Camera& camera)
{
	// Draw level-specific elements (bg, player, enemies, etc.)
	background.draw(image, camera.getViewPosition());
	player.Draw(image, camera.getViewPosition());
}
