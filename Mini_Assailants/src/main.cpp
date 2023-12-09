#include <Constants.hpp>
#include <Level.hpp>

#include <Graphics/Window.hpp>
#include <Graphics/Image.hpp>
#include "Graphics/ResourceManager.hpp"
#include "Graphics/Timer.hpp"
#include "Graphics/Input.hpp"

#include <fmt/core.h>

#include <iostream>

using namespace Graphics;
using namespace Math;

int main()
{
    Image image{};
    Level level{};
    level.setLevel(1);

    //Initialization Settings:
    image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);
    Window window{ L"Mini Assailants", SCREEN_WIDTH, SCREEN_HEIGHT };
    window.show();
    window.setFullscreen(true);

    Timer timer;
    double      totalTime = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps = "FPS: 0";

    while (window)
    {
        timer.tick();

        //// Game Logic Here (Update Loop)  ////

        //Updating Input state
        Input::update();

    	level.update(timer.elapsedSeconds());

    	image.clear(Color::Black);

        // Draw Stuff Here (Render Loop) //

    	level.draw(image);
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
                }
            }
            }
        }

        ++frameCount;

        totalTime += timer.elapsedSeconds();
        if (totalTime > 1.0)
        {
            fps = fmt::format("FPS: {:.3f}", static_cast<double>(frameCount) / totalTime);

            std::cout << fps << '\n';

            frameCount = 0;
            totalTime = 0.0;
        }
    }

    return 0;
}