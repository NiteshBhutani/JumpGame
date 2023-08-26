#include "Camera.hpp"
#include "Constants.hpp"
#include "Platform.hpp"
#include "PlatformPool.hpp"
#include "SFML/System/Vector2.hpp"

#include <SFML/Graphics.hpp>

#include <ios>
#include <iostream>

class Character {
public:
    Character(const sf::Vector2f& pos, Platform* p) :
        mSprite(),
        mVelocity(0.0f, 0.0f),
        mDisplacement(0.0f,0.0f),
        jumpInitialVelocity(0.0f, 0.0f),
        isJumping(false),
        mRestingPlatform(p)
    {
        mSprite.setPosition(pos);
        mSprite.setFillColor(sf::Color::White);
        mSprite.setOutlineColor(sf::Color::Blue);
        mSprite.setOutlineThickness(3);
        mSprite.setSize(sf::Vector2f(50, 50));

        if(!isJumping && mRestingPlatform) {
            mSprite.setPosition(sf::Vector2f(mSprite.getPosition().x, mRestingPlatform->getPlatformYPosition()-50.0f));
        }
    }

    void update(const sf::Time& delta)
    {
        // process input
        sf::Vector2f direction = { 0.0, 0.0 };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            direction.x += 1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            direction.x += -1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isJumping) {
            jumpInitialVelocity = { direction.x * speedRate , -800.0f };
            isJumping = true;
        }


        //check collision


        // if not jumping rest on platform
        if(!isJumping && mRestingPlatform) {
            mSprite.setPosition(sf::Vector2f(mSprite.getPosition().x, mRestingPlatform->getPlatformYPosition()-53.0f));
        }
        
        // jump physics
        if(!isJumping) {
            //if not jumping then only character can only move in x-direction
            mVelocity.x = (direction.x * speedRate);
            mDisplacement.x = mVelocity.x * delta.asSeconds();
        
        } else {
            // while jumping character can move both in x/y direction
            mVelocity.x = (jumpInitialVelocity.x + direction.x * speedRate);
            mVelocity.y = (jumpInitialVelocity.y + gravityRate * Character::gravity * delta.asSeconds());
            mDisplacement.x = mVelocity.x * delta.asSeconds();
            mDisplacement.y = (jumpInitialVelocity.y * delta.asSeconds() + 0.5f * gravityRate * gravity * delta.asSeconds() * delta.asSeconds());
            jumpInitialVelocity.y = mVelocity.y;
        }

        mSprite.move(mDisplacement);

    }
    
    void draw(sf::RenderTarget& window, Camera2D& camera) {
        window.draw(mSprite, camera.getTransform());
    }
    
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

const float Character::speedRate = 150;
const float Character::gravityRate = 100;
const float Character::gravity = 9.8f;

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

const sf::Time App::timePerFrame = sf::seconds(1.f / 60.f);

void App::processEvents()  {
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void App::update(const sf::Time& delta) {
    //check front of platform pool if it is still in focus
    auto& platforms = mPlatformPool.getPlatforms();
    if(!platforms.empty() && !platforms.front()->checkPlatformStillInFocus(mCamera)) {
        mPlatformPool.releaseFromFront();
    }

    actor->update(delta);
    for(auto& p : mPlatformPool.getPlatforms())
    {
        p->update(delta);
    }

    mCamera.moveBy(mCameraSpeed);
}

void App::render() {
    // clear the window with black color
    mWindow.clear();

    for(auto& p : mPlatformPool.getPlatforms())
    {
        p->draw(mWindow, mCamera);
    }

    actor->draw(mWindow, mCamera);
    
    // end the current frame
    mWindow.display();
}

void App::run() {
    //Game Loop
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        sf::Time loopTime = clock.restart();
        timeSinceLastUpdate += loopTime;
        // process and update should follow 60FPS
        while (timeSinceLastUpdate >= timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            
            processEvents();
            update(App::timePerFrame);
        }
        

        render();
    }
}

int main()
{
    
    App app(screenWidth,screenHeight);
    app.run();

    return 0;
}


