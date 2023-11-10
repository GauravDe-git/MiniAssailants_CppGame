#include "StartMenu.hpp"

#include <Graphics/ResourceManager.hpp>
#include <Graphics/Input.hpp>

StartMenu::StartMenu()
{
	startScreen = Graphics::ResourceManager::loadImage("assets/textures/startScreen.png");
}

void StartMenu::update()
{
	if (Graphics::Input::getKeyDown(Graphics::KeyCode::Enter))
	{
		startPressed = true;
	}
}

void StartMenu::draw(Graphics::Image& image)
{
	image.copy(*startScreen, glm::vec2{0,0});
}
