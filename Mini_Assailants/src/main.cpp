#include <Player.hpp>
#include <Background.hpp>
#include <Constants.hpp>
#include <Level.hpp>

#include <Graphics/Window.hpp>
#include <Graphics/Image.hpp>
#include "Graphics/Sprite.hpp"
#include "Graphics/ResourceManager.hpp"
#include "Graphics/SpriteAnim.hpp"      
#include "Graphics/Timer.hpp"
#include "Graphics/Input.hpp"

#include <Camera.hpp>

#include <fmt/core.h>

#include <iostream>

using namespace Graphics;
using namespace Math;

Window window;
Image image;

Camera camera;
//Player player;
Level level;

int main()
{
    level =  Level("assets/textures/stage1.png", 225);
    level.LoadLevelAssets();
    //player = Player{ {SCREEN_WIDTH / 2,(SCREEN_HEIGHT - 10)}};
    bool isScrolling{ true };
    //auto backgroundStage1 = Background("assets/textures/stage1.png");

    //Initialization Settings:
    image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);

    window.create(L"Mini Assailants", SCREEN_WIDTH, SCREEN_HEIGHT);
    window.show();
    window.setFullscreen(true);

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

        if(isScrolling)
        camera.setPosition(camera.getPosition() + glm::vec2(Input::getAxis("Horizontal"), 0) * 80.0f * static_cast<float>(timer.elapsedSeconds()));

        //player.update(timer.elapsedSeconds());
		level.Update(timer.elapsedSeconds());

		// Prevent bg from scrolling off screen on left side (temporary fix)
        /*if (player.getPosition().x <= SCREEN_WIDTH/2)
        {
            isScrolling = false;
        }
        else if (player.getPosition().x > SCREEN_WIDTH/2)
        {
            isScrolling = true;
        }*/
        if (level.getPlayer().getPosition().x <= SCREEN_WIDTH / 2)
        {
            isScrolling = false;
        }
        else if (level.getPlayer().getPosition().x > SCREEN_WIDTH / 2)
        {
            isScrolling = true;
        }

        image.clear(Color::Black);

        // Draw Sprites here (Render Loop)
		//backgroundStage1.draw(image, camera.getViewPosition());

        // Do we want the player moving on the screen?
        //player.Draw(image, camera.getViewPosition());
        level.Draw(image, camera.getViewPosition());

        image.drawText(Font::Default, fps, 10, 10, Color::Blue);

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
                case KeyCode::F11:
                    window.toggleFullscreen();
                    break;
                case KeyCode::T:  //DEBUG
                    isScrolling = !isScrolling;
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