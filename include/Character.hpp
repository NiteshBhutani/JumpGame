#pragma once

#include "Camera.hpp"
#include "Platform.hpp"

class Character {
public:
    Character(const sf::Vector2f& pos, Platform* p);

    void update(const sf::Time& delta);
    void draw(sf::RenderTarget& window, Camera2D& camera);
    
    bool checkCollision(Platform* p);
    void updateRestingPlatform(Platform* p);
    bool shouldCheckForCollision();

    bool outOfGame(Camera2D& camera);

public:
    static const float speedRate;
    static const float gravityRate;
    static const float gravity;

private:
    sf::RectangleShape mSprite;
    sf::Vector2f mDisplacement;
    sf::Vector2f mVelocity;
    
    //owned by platformPool
    Platform* mRestingPlatform;

    // When the up arrow is pressed how much vertical velocity to give
    sf::Vector2f jumpInitialVelocity; 
    bool isJumping;
};

