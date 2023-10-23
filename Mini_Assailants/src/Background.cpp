#include "Background.hpp"

#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

Background::Background(const std::filesystem::path& path)
{
	background = ResourceManager::loadImage(path);
}

void Background::draw(Graphics::Image& image, const glm::vec2& offset)
{
	float w = background->getWidth();
	float left = position.x + offset.x;
	float right = position.x + offset.x + background->getWidth();

	if (left > 0)
	{
		image.copy(*background, glm::vec2(left - w, 0));
	}
	image.copy(*background,glm::vec2(left,0));
	if (right < image.getWidth())
	{
		image.copy(*background, glm::vec2(right, 0));
	}
}
