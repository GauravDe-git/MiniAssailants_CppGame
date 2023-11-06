#include <Level.hpp>
#include <Constants.hpp>

Level::Level()
	:backgroundPath{}, topEdgeCollision{ 0 }, camera{ glm::vec2{0,0} }
{}

void Level::loadLevelAssets()
{
	//Load level-specific assets
	background = Background(backgroundPath);
	enemy = Enemy{ {SCREEN_WIDTH / 2 + 100,SCREEN_HEIGHT - 30} };
	player = Player{ {SCREEN_WIDTH / 2,(SCREEN_HEIGHT - 10)} };

	player.setTopEdgeCollision(topEdgeCollision);
	// Add other assets (eneimes, items etc.) later
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
	// Update game logic here (eg. player, enemies)
	camera.update(deltaTime, player.getPosition(), player.getVelocity(), player.isAttacking());

	player.update(deltaTime);
	enemy.update(deltaTime);
}

void Level::draw(Graphics::Image& image)
{
	// Draw level-specific elements (bg, player, enemies, etc.)
	background.draw(image, camera);
	enemy.draw(image, camera);
	player.draw(image, camera);
}
