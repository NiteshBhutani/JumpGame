#include "Platform.hpp"
#include "Constants.hpp"

Platform::Platform():
    mSize(sf::Vector2f(400, 10)),
    mSprite(),
    mVelocity(0.0, 0.0f)
{
    mSprite.setPosition(0.0f, 0.0f);
    mSprite.setSize(mSize);
    mSprite.setFillColor(sf::Color::Yellow);
    mSprite.setOutlineColor(sf::Color::Red);
    mSprite.setOutlineThickness(2);
}

Platform::Platform(float width, float y, float x) :
    mSize(sf::Vector2f(width, 10)),
    mSprite(),
    mVelocity(0.0, 0.0f)
{
    mSprite.setPosition(x, y);
    mSprite.setSize(mSize);
    mSprite.setFillColor(sf::Color::Yellow);
    mSprite.setOutlineColor(sf::Color::Red);
    mSprite.setOutlineThickness(2);
}

void Platform::update(const sf::Time& delta) {
    
    sf::Vector2f displacement = mVelocity * delta.asSeconds();
    mSprite.move(displacement);

}

void Platform::draw(sf::RenderTarget& target, Camera2D& camera) {

    target.draw(mSprite, camera.getTransform());

}

float Platform::getPlatformYPosition() const {

    return mSprite.getPosition().y;
}

bool Platform::checkPlatformStillInFocus(Camera2D& cam) {
    auto y = mSprite.getPosition().y + 10;

    auto cameraBottomY = -cam.getPosition().y + screenHeight;

    if(y > cameraBottomY) {
        return false;
    }
    return true;
}