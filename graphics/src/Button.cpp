#include "Button.hpp"

using namespace Graphics;
using namespace Math;

Transform2D operator+(Transform2D t, const glm::vec2& v)
{
    t.translate(v);
    return t;
}

Transform2D operator-(Transform2D t, const glm::vec2& v)
{
    t.translate(-v);
    return t;
}

Button::Button(const Graphics::SpriteSheet& sheet, const Math::Transform2D& transform, const std::function<void()>& onClick)
    : spriteSheet{ sheet }
    , transform{ transform }
    , onClick{ onClick }
{
    aabb = AABB::fromRect(spriteSheet[0].getRect());
    aabb *= transform;

}

Button::Button(const std::string& label, const Math::Transform2D& transform, const std::function<void()>& onClick)
    : label{ label }
    , transform{ transform }
    , onClick{ onClick }
{
    aabb = AABB::fromRect(spriteSheet[0].getRect());
    aabb *= transform;
}

void Button::processEvents(const Graphics::Event& event)
{
    if (!enabled)
        return;

    switch (event.type)
    {
    case Event::MouseMoved:
        if (aabb.contains({ event.mouseMove.x, event.mouseMove.y, 0 }))
            setState(State::Hover);
        else
            setState(State::Default);
        break;

    case Event::MouseButtonPressed:
        if (state == State::Hover)
            setState(State::Pressed);
        break;

    case Event::MouseButtonReleased:
        if (state == State::Pressed)
            if (onClick)
                onClick();
        break;
    }
}

void Button::draw(Graphics::Image& image)
{
    if (!enabled)
        return;

    const Sprite* spriteToDraw = &spriteSheet[0];

    switch (state)
    {
    case State::Hover:
        if (spriteSheet.getNumSprites() > 1)
            spriteToDraw = &spriteSheet[1];
        break;
    case State::Pressed:
        if (spriteSheet.getNumSprites() > 2)
            spriteToDraw = &spriteSheet[2];
        break;
    }

    // image.drawAABB( aabb, Color::Red, {}, FillMode::WireFrame );
}

void Button::setState(State newState)
{
    if (state != newState)
    {
        endState(state);
        state = newState;
        startState(state);
    }
}

void Button::endState(State oldState) {}

void Button::startState(State newState)
{
    switch (newState)
    {
    case State::Hover:
    case State::Pressed:
        animTimer.reset();
        break;
    }
}
