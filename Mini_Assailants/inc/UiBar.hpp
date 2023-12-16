#pragma once

#include <glm/vec2.hpp>

#include "Graphics/Image.hpp"


class UiBar
{
public:
	UiBar() = default;

	UiBar(float _width, float _height, const glm::vec2& _offset);

	void Draw(Graphics::Image& image,float currentValue, float maxValue, const glm::vec2& position, const Graphics::Color& color) const;

private:

	float width{};
	float height{};
	glm::vec2 offset{};
};
