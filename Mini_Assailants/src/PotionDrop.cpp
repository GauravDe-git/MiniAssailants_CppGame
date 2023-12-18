#include "PotionDrop.hpp"
#include "Camera.hpp"
#include "Graphics/ResourceManager.hpp"

PotionDrop::PotionDrop(const glm::vec2& pos, Type _type)
	:Entity{pos}, type{_type},
	aabb{{0,0,0},{19,25,0}}
{
	//Load the sprite based on type
	if(type == Type::HP)
	{
		sprite = Graphics::Sprite{ Graphics::ResourceManager::loadImage("assets/textures/hp_potion.png"),Graphics::BlendMode::AlphaBlend };
	}
	else if (type == Type::MP)
	{
		sprite = Graphics::Sprite{ Graphics::ResourceManager::loadImage("assets/textures/mp_potion.png"),Graphics::BlendMode::AlphaBlend };
	}

	//Set the value based on the type
	value = (type == Type::HP) ? 10 : 5;
}

void PotionDrop::draw(Graphics::Image& image, const Camera& camera)
{
	image.drawSprite(sprite, transform.getPosition() + camera.getViewPosition() + glm::vec2{0,-15});

	#if _DEBUG
	image.drawAABB(transform * aabb + glm::vec3{ camera.getViewPosition(), 0 } + glm::vec2{ 0,-15 }, Graphics::Color::Yellow, {}, Graphics::FillMode::WireFrame);
	#endif	
}



