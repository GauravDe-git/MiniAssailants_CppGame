#pragma once

//Description: Responsible for loading and drawing a background image


#include <Graphics/Image.hpp>
#include <Camera.hpp>

#include <filesystem>
#include <memory>

class Background
{
public:
	Background() = default;

	Background(const std::filesystem::path& path);

	void draw(Graphics::Image& image, const Camera& camera) const;

private:
	std::shared_ptr<Graphics::Image> background;
};