#pragma once

#include "SFML/Graphics/Transform.hpp"
#include "SFML/System/Vector2.hpp"
class Camera2D {
    public:
        Camera2D();

        sf::Transform& getTransform();
        void moveBy(sf::Vector2f& delta);
    private:
        sf::Vector2f mPos;
        sf::Transform mTransform;
};