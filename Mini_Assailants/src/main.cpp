#include"Graphics/Window.hpp"

Graphics::Window window;

int main()
{
	window.create(L"Mini Assailants", 800, 600);

	window.show();

	while (window)
	{
		// Game Logic Here

		Graphics::Event event;
		while (window.popEvent(event))
		{
			switch (event.type)
			{
			case Graphics::Event::Close:
				window.destroy();
				break;
			case Graphics::Event::KeyPressed:
			{
				switch (event.key.code)
				{
				case Graphics::KeyCode::Escape:
					window.destroy();
					break;
				}
			}
			}
		}
	}

	return 0;
}