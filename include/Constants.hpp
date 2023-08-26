#pragma once

#include <random>

namespace  {
    inline std::random_device rd;
    inline std::mt19937 gen(rd());
    
    inline static const unsigned int screenWidth = 800;
    inline static const unsigned int screenHeight = 600;

    inline float random(int low, int high)
    {
        std::uniform_int_distribution<> dist(low, high);
        return (float)dist(gen);
    }
}