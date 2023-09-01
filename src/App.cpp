#include "App.hpp"
#include "Constants.hpp"

const sf::Time App::timePerFrame = sf::seconds(1.f / 60.f);

App::App(unsigned int width, unsigned int height):
    mWindowWidth(width),
    mWindowHeight(height),
    mWindow(sf::VideoMode(width,height), "JumpGame"),
    actor(),
    mCameraSpeed(0.0f,0.5f), //Camera is moving up with constant speed (Camera speed is alwys inverse of direction where we want to go)
    mCamera(),
    mPlatformPool()
{
    auto initialRestingPlatform = mPlatformPool.getPlatforms().front().get();
    auto platformX = initialRestingPlatform->getPlatformXPosition();
    auto x = (float) random((int)platformX.first, (int)platformX.second);
    auto y = initialRestingPlatform->getPlatformYPosition() - 28.0f;
    actor = std::make_shared<Character>(sf::Vector2f(x,y), initialRestingPlatform);

}

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
    
    for (auto& p : mPlatformPool.getPlatforms())
    {
        if(actor->checkCollision(p.get())) {
            lastCollidedPlatform = p.get();
        }
    }
    if(lastCollidedPlatform) {
        actor->updateRestingPlatform(lastCollidedPlatform);
    }
}

void App::update(const sf::Time& delta) {

    //if actor of game then quit
    if(actor->outOfGame(mCamera)) {
        mWindow.close(); 
    }

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
