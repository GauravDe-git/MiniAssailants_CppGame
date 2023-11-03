#include <Level.hpp>
#include <Constants.hpp>

Level::Level()
	:backgroundPath{}, topEdgeCollision{ 0 }, camera{ glm::vec2{0,0} }
{}

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
	camera.update(deltaTime, player.getPosition(), player.getVelocity(), player.isAttacking());

	player.update(deltaTime);
}

void Level::Draw(Graphics::Image& image)
{
	// Draw level-specific elements (bg, player, enemies, etc.)
	background.draw(image, camera.getViewPosition());
	player.Draw(image, camera.getViewPosition());
}


// call the camera in the level update, move the camera call from main to level
