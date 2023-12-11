#include "Background.hpp"

#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

Background::Background(const std::filesystem::path& path)
{
	background = ResourceManager::loadImage(path);
}

void Background::draw(Graphics::Image& image, const Camera& camera) const
{
    float w = background->getWidth();

    // Calculate the current position of the background.
    float bgX = camera.getViewPosition().x;

    // Draw the background repeatedly to the right as the player moves.
    while (bgX < image.getWidth()) {
        image.copy(*background, glm::vec2(bgX, 0));
        bgX += w;
    }
}
