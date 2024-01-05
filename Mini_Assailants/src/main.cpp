// Credits for the framework: C++ For Games by Jeremiah van Oosten (@jpvanoosten)
// Source: https://github.com/BredaUniversityGames/cppforgames-template
//		   https://github.com/jpvanoosten/SoftwareRasterizer

// Framework Includes:
// 2D software rasterizer in the Image class.
// Support for gamepads(currently using XInput on Windows)
// Spatialized audio library with support for wav, mp3, ogg, flac audio file sources.
// Create your own waveforms for your game using the Waveform class.
// A few math helpers in the math library(AABB, Camera2D, Transform2D, etc...)

#include <Game.hpp>
#include <Constants.hpp>

#include "Graphics/Window.hpp"

#include <iostream>

using namespace Graphics;

int main()
{
	std::cout << "...Loading Game" << '\n'; 

	Window window{ L"Mini Assailants", SCREEN_WIDTH, SCREEN_HEIGHT };

	Game game{ SCREEN_WIDTH, SCREEN_HEIGHT, window };

	window.show();
	window.setFullscreen(true);

	while (window)
	{
		window.clear(Color::Black);

		game.play();

		window.present(game.getBufferImage());

		//Handle events
		Event e;
		while (window.popEvent(e))
		{
			game.processEvent(e);

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
					window.toggleVSync();
					break;
				case KeyCode::F11:
					window.toggleFullscreen();
					break;
				}
				break;
			}
		}
	}

	return 0;
}
