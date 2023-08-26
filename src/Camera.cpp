#include "Camera.hpp"
#include "SFML/Graphics/Transform.hpp"
#include "SFML/System/Vector2.hpp"
#include <iostream>

Camera2D::Camera2D():
    mTransform(),
    mPos(0.0f, 0.0f)
    {}

void Camera2D::moveBy(sf::Vector2f &delta) {
    mPos += delta;
    mTransform.translate(delta);
}

sf::Transform& Camera2D::getTransform() {
    return mTransform;
}

sf::Vector2f& Camera2D::getPosition() {
    return mPos;
}
