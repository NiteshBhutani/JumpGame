#pragma once

#include "Camera.hpp"
#include "Platform.hpp"


enum class Direction { Left, Right};
enum class Movement { Idle, Run, Jump, Fall};

class Animation {
public:
    Animation() :
        mTimeSinceLastFrame(sf::Time::Zero),
        mTexture(),
        currentTextRect(0),
        currentRunDir(Direction::Right)
    {}

    void setup(std::string name, int x, int y, int width, int height, int numFrames) 
    {
        if (!mTexture.loadFromFile(name)) {
        //Handle error
        }
        for (int i = 0; i < numFrames; i++) {
            frames.push_back(sf::IntRect(i * width, 0, width, height));
        }

    }

    void update(sf::Time delta) {
        mTimeSinceLastFrame += delta;
        while (mTimeSinceLastFrame >= holdTime) {
            mTimeSinceLastFrame -= holdTime;
            step();
        }
    }

    void step() {
        currentTextRect++;
        if (currentTextRect >= frames.size()) {
            currentTextRect = 0;
        }
    }

    void applyTexture(sf::Sprite& sp, Direction dir) {
        if(currentRunDir != dir) {
            currentRunDir = dir;
            sp.scale(-1.0, 1.0f);
        }
        sp.setTexture(mTexture);
        sp.setTextureRect(frames[currentTextRect]);
        
    }
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
    Animation mTextures[4];
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

