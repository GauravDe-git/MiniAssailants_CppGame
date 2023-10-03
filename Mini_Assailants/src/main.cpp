#include "../Player.hpp"

#include"Graphics/Window.hpp"
#include"Graphics/Image.hpp"
#include"Graphics/Sprite.hpp"
#include"Graphics/ResourceManager.hpp" //It includes font.hpp   *Transitive Includes*
#include"Graphics/SpriteAnim.hpp"      //It includes spritesheet.hpp
#include"Graphics/Timer.hpp"
//#include"Graphics/Keyboard.hpp" for Simple Keyboard mapping
#include"Graphics/Input.hpp" // For pre-configured mapping

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
	//SpriteAnim playerIdleAnim(idleSprites,10.0f);
	player = Player{ {SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 },SpriteAnim{idleSprites,10.0f} };

	auto backgroundStage1 = ResourceManager::loadImage("assets/textures/stage1.png");
	Sprite bg1Sprite(backgroundStage1);

	//Initialization Settings:
	image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);

	window.create(L"Mini Assailants", SCREEN_WIDTH,SCREEN_HEIGHT);
	window.show();
	//window.setVSync(false);

	Timer timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";

	while (window)
	{
		timer.tick();

		// Game Logic Here (Update Loop)

		Input::update();

		player.update(timer.elapsedSeconds());

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