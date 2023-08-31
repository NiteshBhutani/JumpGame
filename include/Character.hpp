#pragma once

#include "Camera.hpp"
#include "Platform.hpp"

#include <iostream>

enum class Direction { Left, Right};
enum class Movement { Idle, Run, Jump, Fall};

class Animation {
public:
    Animation();

    void setup(std::string name, int x, int y, int width, int height, int numFrames) ;
    void update(sf::Time delta);
    void step();
    void applyTexture(sf::Sprite& sp, Direction dir) ;

public:
    static sf::Time holdTime;
    
private:
    sf::Time mTimeSinceLastFrame;
    sf::Texture mTexture;
    std::vector<sf::IntRect> frames;
    int currentTextRect;
    Direction currentRunDir;

};


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
    sf::Sprite mSprite;
    // Row represent movement and column direction. So [Run][Left] or [Run][Right]
    Animation mTextures[4][2];
    Direction mDirection;
    Movement mMovement;
    sf::Vector2f mDisplacement;
    sf::Vector2f mVelocity;
    
    //owned by platformPool
    Platform* mRestingPlatform;

    // When the up arrow is pressed how much vertical velocity to give
    sf::Vector2f jumpInitialVelocity; 
    bool isJumping;
};

