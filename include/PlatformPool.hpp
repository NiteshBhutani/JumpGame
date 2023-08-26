#pragma once

#include "Platform.hpp"
#include "Constants.hpp"
#include <deque>

class PlatformPool {

public:
    PlatformPool();
    
    std::deque<std::unique_ptr<Platform>>& getPlatforms();
    void releaseFromFront();

private:
    void createPlatforms();

private:
    size_t mSize;
    std::deque<std::unique_ptr<Platform>> platforms;
};

