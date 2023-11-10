#pragma once

#include <Graphics/Image.hpp>

class StartMenu {
public:
    StartMenu();

    void update();
    void draw(Graphics::Image& image);

    bool isStartPressed() const { return startPressed; }
private:
    std::shared_ptr<Graphics::Image> startScreen;

    bool startPressed{ false };
};