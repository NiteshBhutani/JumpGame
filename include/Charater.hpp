#pragma once

#include "Platform.hpp"

class Character {
public:
    Character(const sf::Vector2f& pos, Platform* p);

    void update(const sf::Time& delta);
    void draw(sf::RenderTarget& window, Camera2D& camera);
    ////////////////////////////
    // TODO : Improve collision detection 
    // Issues : sporadically box can jump tp platform above it, if its too clode
    // Issues : If box is half inside the platform, this algorithm doesn't catch that
    ///////////////////////////
    bool checkCollision(Platform* p);
    void updateRestingPlatform(Platform* p);
    bool shouldCheckForCollision();

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

