#pragma once

#include <random>

namespace  {
    inline std::random_device rd;
    inline std::mt19937 gen(rd());
    
    inline static const unsigned int screenWidth = 800;
    inline static const unsigned int screenHeight = 600;
    inline static float pixelPerMeter = 100.0f; // this represent how many pixel 1 meter of real worl represent. so vel = 2m/s will be 200 pixel/sec and acceleration of 9.8 m/s2 will 9800 pixel/s2
    
    inline static float platformHeight = 10.0f;
    inline static float platformOutlineThickness = 2;


    inline static float characterHeight = 50.0f;
    inline static float characterWidth = 50.0f;

    inline static float jumpYSpeed = -8.0f; //-8m/s
    inline static float jumpXSpeed = 2.2f;
    inline float random(int low, int high)
    {
        std::uniform_int_distribution<> dist(low, high);
        return (float)dist(gen);
    }
}