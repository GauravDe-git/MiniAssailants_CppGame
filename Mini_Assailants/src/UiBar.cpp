#include<UiBar.hpp>

UiBar::UiBar(float _width, float _height, const glm::vec2& _offset)
	:width{ _width }, height{ _height }, offset{ _offset }
{
	
}

void UiBar::Draw(Graphics::Image& image, float currentValue, float maxValue, const glm::vec2& position, const Graphics::Color& color) const
{
	const float valuePercent = currentValue / maxValue;
	Math::AABB barAABB;

	//Outline
	barAABB.min = glm::vec3{ position + offset - glm::vec2{1,1}, 0 };
	barAABB.max = glm::vec3{ position + offset + glm::vec2{ width + 2, height + 2 }, 0 };
	image.drawAABB(barAABB, Graphics::Color::Black);

	barAABB.min = glm::vec3{position + offset, 0};
	barAABB.max = glm::vec3{ position + offset + glm::vec2{ valuePercent * width, height }, 0 };
	image.drawAABB(barAABB, color );
}

