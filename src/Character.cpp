#include "Character.hpp"
#include "Camera.hpp"
#include "Constants.hpp"
#include <iostream>


const float Character::speedRate = 150;
const float Character::gravityRate = 100;
const float Character::gravity = 9.8f;
sf::Time Animation::holdTime = sf::seconds(0.05f);


Character::Character(const sf::Vector2f& pos, Platform* p) :
    mSprite(),
    mVelocity(0.0f, 0.0f),
    mDisplacement(0.0f,0.0f),
    jumpInitialVelocity(0.0f, 0.0f),
    isJumping(false),
    mRestingPlatform(p),
    mTextures(),
    mDirection(Direction::Right),
    mMovement(Movement::Idle)
{
    mSprite.setPosition(pos);
    mSprite.setOrigin(40.0f, 40.0f);

    std::string idleSprite = "../assets/Idle.png";
    std::string runSprite = "../assets/Run.png";
    std::string jumpSprite = "../assets/Jump.png";
    
    mTextures[(int)Movement::Idle].setup(idleSprite, 0, 0, 80, 80, 5);
    mTextures[(int)Movement::Run].setup(runSprite, 0, 0, 80, 80, 8);
    mTextures[(int)Movement::Jump].setup(jumpSprite, 0, 120, 80, 80, 1);
    mTextures[(int)Movement::Fall].setup(jumpSprite, 0, 200, 80, 80, 1);
    
}

void Character::update(const sf::Time& delta)
{
    mMovement = Movement::Idle;
    mDirection = Direction::Right;
    // process input
    sf::Vector2f direction = { 0.0, 0.0 };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction.x += 1.0f;
        mMovement = Movement::Run;
        mDirection = Direction::Right;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction.x += -1.0f;
        mMovement = Movement::Run;
        mDirection = Direction::Left;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isJumping) {
        jumpInitialVelocity = { 0.0f , -800.0f };
        isJumping = true;
    }

    // jump physics
    if(!isJumping) {
        //if not jumping then only character can only move in x-direction
        mVelocity.x = (direction.x * speedRate);
        mVelocity.y = 0.0f;
        mDisplacement.y = 0.0f;
        mDisplacement.x = mVelocity.x * delta.asSeconds();

        //check character doesn't leave right side of platform
        if (mSprite.getPosition().x + 50.0f + mDisplacement.x > mRestingPlatform->getPlatformXPosition().second) {
            mDisplacement.x = mRestingPlatform->getPlatformXPosition().second - (mSprite.getPosition().x + 50.0f);
        }

        //check character doesn't leave left side of platform
        if (mSprite.getPosition().x + mDisplacement.x < mRestingPlatform->getPlatformXPosition().first) {
            mDisplacement.x = mRestingPlatform->getPlatformXPosition().first - (mSprite.getPosition().x);
        }
    
    } else {
        
        // while jumping character can move both in x direction, thats why changing velocity of x in every update call
        // based on direction
        mVelocity.x = direction.x * speedRate;
        mVelocity.y = (jumpInitialVelocity.y + gravityRate * Character::gravity * delta.asSeconds());
        mDisplacement.x = mVelocity.x * delta.asSeconds();
        mDisplacement.y = mVelocity.y * delta.asSeconds();
        jumpInitialVelocity.y = mVelocity.y;
    }

    if(isJumping) {
        if(mVelocity.y >= 0) {
            mMovement = Movement::Fall;
        } else {
            mMovement = Movement::Jump;
        }
    }

    mTextures[(int)mMovement].update(delta);
    mTextures[(int)mMovement].applyTexture(mSprite, mDirection);
    
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
    
    auto charXMid = mSprite.getPosition().x + 25.0f;
    auto platformX = p->getPlatformXPosition();

    //std::cout << "Char Y Bottom = " << charYBottom << " Char Mid = " << charXMid << std::endl;
    //std::cout << "Platform Y Top = " << platformYTop << " Platform Y Bottom = " << platformYBottom << std::endl;
    //std::cout << "Platform X Left = " << platformX.first << "Platform X Right = " << platformX.second << std::endl;

    // Here 2.5 is margin of error. There can be cases in which position sample are more than 10 units apart. For those cases, a 2.5 units
    // margin of error is added. Therefor check will between platformTop and platformBottom + 2.5. If box positon lies in bettween this then 
    // its a collision
    if(charYBottom < platformYTop || charYBottom  > platformYBottom + 2.5 ) {
        return false;
    }
    
    
    
    if(charXMid < platformX.first || charXMid > platformX.second) {
        return false;
    }

    
    return result;
}
    
void Character::updateRestingPlatform(Platform* p) {
    mRestingPlatform = p;
    isJumping = false;
    jumpInitialVelocity = {0.0f, 0.0f};
    mSprite.setPosition(sf::Vector2f(mSprite.getPosition().x, mRestingPlatform->getPlatformYPosition()-53.0f));
    
}

bool Character::shouldCheckForCollision() {
    return isJumping && jumpInitialVelocity.y >= 0;
}

bool Character::outOfGame(Camera2D& camera) {

    auto cameraBottomPos = -camera.getPosition().y + screenHeight;
    auto y = mSprite.getPosition().y;
    if(y - screenHeight > cameraBottomPos) { // we will wait for box to go screenHeight units below before quiting
        return true;
    }

    return false;
}