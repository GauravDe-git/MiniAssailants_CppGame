#include <Game.hpp>

#include "fmt/format.h"
#include "Graphics/Font.hpp"

using namespace Graphics;
using namespace Math;

Game::Game(int width, int height)
{
	level.setLevel(1);
	image.resize(width, height);
}

void Game::play()
{
    static double totalTime = 0.0;
    static uint64_t frameCount = 0;
    static std::string fps = "FPS: 0";

    timer.tick();
    ++frameCount;
    totalTime += timer.elapsedSeconds();
    if (totalTime > 1.0)
    {
        fps = fmt::format("FPS:{:.2f}", static_cast<double>(frameCount) / totalTime);

        //std::cout << fps << '\n';

        frameCount = 0;
        totalTime = 0.0;
    }

    Input::update();

    level.update(timer.elapsedSeconds());
    level.draw(image);
    image.drawText(Font::Default, fps, 407, 5, Color::Yellow);
}

void Game::processEvent(const Event& e) {
    // Handle game-specific events here
    level.processEvents(e);
}

