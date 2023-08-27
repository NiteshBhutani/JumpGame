#include "Camera.hpp"
#include "Constants.hpp"
#include "Platform.hpp"
#include "PlatformPool.hpp"
#include "SFML/System/Vector2.hpp"

#include <SFML/Graphics.hpp>

#include <ios>
#include <iostream>
#include <ostream>

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

        if(!isJumping && mRestingPlatform) {
            mSprite.setPosition(sf::Vector2f(mSprite.getPosition().x, mRestingPlatform->getPlatformYPosition()-53.0f));
        }
        
        // jump physics
        if(!isJumping) {
            //if not jumping then only character can only move in x-direction
            mVelocity.x = (direction.x * speedRate);
            mVelocity.y = 0.0f;
            mDisplacement.y = 0.0f;
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

    ////////////////////////////
    // TODO : Improve collision detection 
    // Issues : sporadically box can jump tp platform above it, if its too clode
    // Issues : If box is half inside the platform, this algorithm doesn't catch that
    ///////////////////////////
    bool checkCollision(Platform* p) {
        bool result = false;

        auto charYTop = mSprite.getPosition().y;
        auto charYBottom = mSprite.getPosition().y + 50;
        auto charLeftX = mSprite.getPosition().x + 15;
        auto charRightX = mSprite.getPosition().x + 40;

        if(!p) {
            //assert
            return result;
        }

        auto platformY = p->getPlatformYPosition();
        auto platformX = p->getPlatformXPosition();

        //std::cout << "CharY = " << charYBottom << " CharLeftX " << charLeftX << " charRightX " << charRightX << std::endl;
        //std::cout << "platformY = " << platformY << " platformLeftX " << platformX.first << " PlatformRightX " << platformX.second << std::endl;
        
        if(charYTop < platformY && charYBottom >= platformY && charLeftX >= platformX.first && charRightX <= platformX.second) {
            return true;
        }
        return result;
    }
    
    void updatePlatform(Platform* p) {
        mRestingPlatform = p;
        isJumping = false;
        jumpInitialVelocity = {0.0f, 0.0f};
        
    }

    bool shouldCheckForCollision() {
        return isJumping && jumpInitialVelocity.y >= 0;
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

void App::checkCollisionWithPlatforms() {
    
    Platform* lastCollidedPlatform = nullptr;
    
    if(!actor->shouldCheckForCollision()) return;
    
    for(auto& p : mPlatformPool.getPlatforms())
    {
        if(actor->checkCollision(p.get())) {
            lastCollidedPlatform = p.get();
        }
    }
    if(lastCollidedPlatform) {
        actor->updatePlatform(lastCollidedPlatform);
    }
}

void App::update(const sf::Time& delta) {
    //check front of platform pool if it is still in focus
    auto& platforms = mPlatformPool.getPlatforms();
    if(!platforms.empty() && !platforms.front()->checkPlatformStillInFocus(mCamera)) {
        mPlatformPool.releaseFromFront();
    }

    //check collision
    this->checkCollisionWithPlatforms();

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


