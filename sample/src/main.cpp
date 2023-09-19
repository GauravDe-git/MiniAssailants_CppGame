#include "Ball.hpp"

#include <Audio/Device.hpp>

#include <Graphics/Window.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/Timer.hpp>

#include <fmt/core.h>

#include <iostream>

using namespace Graphics;
using namespace Math;

int main(int argc, char* argv[])
{
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;

    Window window{ L"C++ for Games", WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  image{ WINDOW_WIDTH, WINDOW_HEIGHT };

    window.show();

    Ball ball{ { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } };
    ball.setVelocity(glm::vec2{ 1, -1 } *200.0f);

    // Set the audio listener to be in the center of the screen.
    Audio::Device::getListener().setPosition({ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 0.0f });

    // Load a one-shot bounce sound.
    Audio::Sound bounce{ "assets/sounds/8-bit-powerup.mp3" };

    // The default attenuation model is Inverse, but the linear attenuation model sounds better for this demo.
    bounce.setAttenuationModel(Audio::Sound::AttenuationModel::Linear);

    // Load some background music.
    Audio::Sound bgMusic{ "assets/sounds/Sweet Treats.ogg", Audio::Sound::Type::Music };
    bgMusic.setVolume(0.2f);  // It's too loud!

    bgMusic.play();

    Timer       timer;
    double      totalTime = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps = "FPS: 0";

    while (window)
    {
        ball.update(static_cast<float>(timer.elapsedSeconds()));
        // Check for collisions with the edges of the screen.
        Circle    c = ball.getCircle();
        glm::vec2 vel = ball.getVelocity();
        if (c.left() <= 0.0f)
        {
            c.center.x = c.radius;
            vel.x *= -1.0f;
            bounce.replay();
        }
        else if (c.right() >= static_cast<float>(image.getWidth()))
        {
            c.center.x = static_cast<float>(image.getWidth()) - c.radius;
            vel.x *= -1.0f;
            bounce.replay();
        }
        if (c.top() <= 0.0f)
        {
            c.center.y = c.radius;
            vel.y *= -1.0f;
            bounce.replay();
        }
        else if (c.bottom() >= static_cast<float>(image.getHeight()))
        {
            c.center.y = static_cast<float>(image.getHeight()) - c.radius;
            vel.y *= -1.0f;
            bounce.replay();
        }
        ball.setCircle(c);
        ball.setVelocity(vel);

        // Update the sound position.
        bounce.setPosition({ c.center.x, c.center.y, 0 });

        image.clear(Color::Black);
        ball.draw(image);
        image.drawText(Font::Default, fps, 10, 10, Color::White);

        window.clear(Color::Blue);
        window.present(image);

        Event e;
        while (window.popEvent(e))
        {
            switch (e.type)
            {
            case Event::Close:
                window.destroy();
                break;
            case Event::KeyPressed:
                switch (e.key.code)
                {
                case KeyCode::Escape:
                    window.destroy();
                    break;
                case KeyCode::V:
                    window.setVSync(!window.isVSync());
                    std::cout << "Vsync: " << window.isVSync() << std::endl;
                    break;
                case KeyCode::Enter:
                    if (e.key.alt)
                    {
                        [[fallthrough]];
                case KeyCode::F11:
                    window.toggleFullscreen();
                    }
                    break;
                }
                break;
            }
        }

        timer.tick();
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