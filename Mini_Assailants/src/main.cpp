#include <Game.hpp>
#include <Constants.hpp>

#include "Graphics/Window.hpp"

using namespace Graphics;

int main()
{
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
