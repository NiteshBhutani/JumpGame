#include "PlatformPool.hpp"
#include "Platform.hpp"
#include <deque>
#include <memory>

PlatformPool::PlatformPool() :
    mSize(20)
{
    std::unique_ptr<Platform> p1 = std::make_unique<Platform>(random(100,screenWidth/4), screenHeight-100,random(screenWidth/4, screenWidth/2));
    std::unique_ptr<Platform> p2 = std::make_unique<Platform>(random(100,screenWidth/2), (float)screenHeight/2, random(screenWidth/4, (int)(3*(screenWidth/4.0f)) ));

    platforms.push_back(std::move(p1));
    platforms.push_back(std::move(p2));

    createPlatforms();
}
    
std::deque<std::unique_ptr<Platform>>& PlatformPool::getPlatforms() {
    return platforms;
}

void PlatformPool::releaseFromFront() {
    if(!platforms.empty()) {
        platforms.pop_front();
    }

    if(platforms.size() < 10 ) {
        // if size of platform pool reduce by certain size and they again make the platforms as per size
        createPlatforms();
    }
}

void PlatformPool::createPlatforms() {
    int i = 1;
    while (platforms.size() < mSize) {
        auto lastPlatformPosition = platforms.back()->getPlatformYPosition();
        auto y =  lastPlatformPosition - (float)random(100,300) ;
        float x;
        if (i % 4 == 1) {
            x = random(0, screenWidth/4);
        }
        else if (i % 4 == 2) {
            x = random(screenWidth/4, screenWidth/2);
        }
        else if (i % 4 == 3) {
            x = random(screenWidth/2, (int) (0.625* screenWidth));
        }
        else {
            x = random((int)(0.625 * screenWidth), (int)(0.75 * screenWidth));
        }
        float width;

        if (i % 4 == 1 || i % 4 == 3) {
            width = random(100, screenWidth / 2);
        }
        else {
            width = random(100, screenWidth / 4);
        }
        std::unique_ptr<Platform> p = std::make_unique<Platform>(width,y,x);
        platforms.push_back(std::move(p));
        i++;
    }
}
