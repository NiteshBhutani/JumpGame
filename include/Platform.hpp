#pragma once

#include <SFML/Graphics.hpp>
#include "Camera.hpp"

class Platform {
public:
    Platform(float width, float y, float x);
    void update(const sf::Time& delta);
    void draw(sf::RenderTarget& target, Camera2D& camera);
    float getPlatformYPosition() const;
    //[LeftX, RightX]
    std::pair<float,float> getPlatformXPosition() const;
    bool checkPlatformStillInFocus(Camera2D& cam);

private:
    sf::Vector2f mSize;
    sf::Vector2f mVelocity;
    sf::RectangleShape mSprite;
};
