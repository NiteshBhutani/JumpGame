#pragma once

#include "Character.hpp"
#include "PlatformPool.hpp"
#include "Platform.hpp"

class App {
public:
    App(unsigned int width, unsigned int height);
    
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
