#pragma once

#include <Graphics/Image.hpp>
#include <Camera.hpp>

#include <filesystem>
#include <memory>

class Background
{
public:
	Background() = default;

	Background(const std::filesystem::path& path);

	void draw(Graphics::Image& image, const Camera& camera);

private:
	std::shared_ptr<Graphics::Image> background;
	glm::vec2 position;
};