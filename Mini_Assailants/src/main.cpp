#include<Player.hpp>

#include"Graphics/Window.hpp"
#include"Graphics/Image.hpp"
#include"Graphics/Sprite.hpp"
#include"Graphics/ResourceManager.hpp"
#include"Graphics/SpriteAnim.hpp"      
#include"Graphics/Timer.hpp"
#include"Graphics/Input.hpp" 

#include <fmt/core.h>

#include <iostream>

using namespace Graphics;

Window window;
Image image;

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 270;

Player player;

int main()
{
	auto idleSprites = ResourceManager::loadSpriteSheet("assets/textures/Idle_Sheet.png", 153, 127, 0, 0, BlendMode::AlphaBlend);
	player = Player{ {SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 },SpriteAnim{idleSprites,10.0f} };

	auto backgroundStage1 = ResourceManager::loadImage("assets/textures/stage1.png");
	Sprite bg1Sprite(backgroundStage1);

	//Initialization Settings:
	image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);

	window.create(L"Mini Assailants", SCREEN_WIDTH,SCREEN_HEIGHT);
	window.show();

	Timer timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";

	while (window)
	{
		timer.tick();

		// Game Logic Here (Update Loop)

		//Updating Input state
		Input::update(); 
		player.update(timer.elapsedSeconds());

		//Check collision with player (screen-space)
		{
			auto aabb = player.getAABB();
			glm::vec2 correction{ 0 };
			if (aabb.min.x < 0)
			{
				correction.x = -aabb.min.x;
			}
			if (aabb.min.y < 0)
			{
				correction.y = -aabb.min.y;
			}
			if (aabb.max.x >= image.getWidth())
			{
				correction.x = image.getWidth() - aabb.max.x;
			}
			if (aabb.max.y >= image.getHeight())
			{
				correction.y = image.getHeight() - aabb.max.y;
			}
			//Apply correction
			player.translate(correction);
		}

		image.clear(Color::White);

		// Draw Sprites here (Render Loop)

		image.copy(*backgroundStage1, 0, 0);

		player.Draw(image);

		image.drawText(Font::Default, fps, 10, 10, Color::White);

		window.present(image);

		Event event;
		while (window.popEvent(event))
		{
			switch (event.type)
			{
				case Event::Close:
					window.destroy();
					break;
				case Event::KeyPressed:
				{
					switch (event.key.code)
					{
					case KeyCode::Escape:
						window.destroy();
						break;
					case KeyCode::V:
						window.toggleVSync();
						break;
					}
				}
			}
		}

		++frameCount;

		totalTime += timer.elapsedSeconds();
		if (totalTime > 1.0)
		{
			fps = fmt::format("FPS: {:.3f}", static_cast<double>(frameCount) / totalTime);

			std::cout << fps << std::endl;

			frameCount = 0;
			totalTime = 0.0;
		}
	}

	return 0;
}