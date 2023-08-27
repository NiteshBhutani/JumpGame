#pragma once

#include "Character.hpp"
#include "PlatformPool.hpp"
#include "Platform.hpp"

class App {
public:
    App(unsigned int width, unsigned int height):
        mWindowWidth(width),
        mWindowHeight(height),
        mWindow(sf::VideoMode(width,height), "JumpGame"),
        actor(),
        mCameraSpeed(0.0f,0.5f), //Camera is moving up with constant speed (Camera speed is alwys inverse of direction where we want to go)
        mCamera(),
        mPlatformPool()
    {
        actor = std::make_shared<Character>(sf::Vector2f(width/2.f, height/2.f), mPlatformPool.getPlatforms().front().get());
    }
    
    void processEvents();
    void update(const sf::Time& delta);
    void run();
    void render();

    void checkCollisionWithPlatforms();

public:
    static const sf::Time timePerFrame;


private:
    unsigned int mWindowWidth;
    unsigned int mWindowHeight;
    sf::Vector2f mCameraSpeed;
    sf::RenderWindow mWindow;
    std::shared_ptr<Character> actor;
    PlatformPool mPlatformPool;
    Camera2D mCamera;
};
