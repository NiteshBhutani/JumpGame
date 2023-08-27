#include "Character.hpp"
#include <iostream>

const float Character::speedRate = 150;
const float Character::gravityRate = 100;
const float Character::gravity = 9.8f;


Character::Character(const sf::Vector2f& pos, Platform* p) :
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

void Character::update(const sf::Time& delta)
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
    
void Character::draw(sf::RenderTarget& window, Camera2D& camera) {
    window.draw(mSprite, camera.getTransform());
}

////////////////////////////////////////
// This is not AABB collision. In this 
// we check whether box bottom line is within 
// platform Height and box mid in x direction is within 
// platform width. 
bool Character::checkCollision(Platform* p) {
    if(!p) {
        //assert
        return false;
    }

    bool result = true;

    auto charYBottom = mSprite.getPosition().y + 50.0f;
    auto platformYTop = p->getPlatformYPosition();
    auto platformYBottom = p->getPlatformYPosition() + 10.0f;
    
    if(charYBottom < platformYTop || charYBottom > platformYBottom) {
        return false;
    }
    
    auto charXMid = mSprite.getPosition().x + 25.0f;
    auto platformX = p->getPlatformXPosition();

    
    if(charXMid < platformX.first || charXMid > platformX.second) {
        return false;
    }

    // std::cout << "Char Y Bottom = " << charYBottom << " Char Mid = " << charXMid << std::endl;
    // std::cout << "Platform Y Top = " << platformYTop << " Platform Y Bottom = " << platformYBottom << std::endl;
    // std::cout << "Platform X Left = " << platformX.first << "Platform X Right = " << platformX.second << std::endl;

    return result;
}
    
void Character::updateRestingPlatform(Platform* p) {
    mRestingPlatform = p;
    isJumping = false;
    jumpInitialVelocity = {0.0f, 0.0f};
    
}

bool Character::shouldCheckForCollision() {
    return isJumping && jumpInitialVelocity.y >= 0;
}