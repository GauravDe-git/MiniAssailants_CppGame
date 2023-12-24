#pragma once

#include <Level.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Image.hpp>

class Game
{
public:

	Game(int width, int height);

	void play();
	void processEvent(const Graphics::Event& e);

	const Graphics::Image& getBufferImage() const { return image; }
private:
	Graphics::Image image{};
	Graphics::Timer timer{};

	Level level{};
};
