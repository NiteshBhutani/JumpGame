#include "Character.hpp"
#include "Camera.hpp"
#include "Constants.hpp"
#include "SFML/System/Vector2.hpp"
#include <iostream>
#include <vector>
#include <limits>


namespace {

    bool calculateIntesection(std::vector<sf::Vector2f>& line1, std::vector<sf::Vector2f>& line2) {
        auto result = false;
        // Line1 represented as a1x + b1y = c1
        auto& line1pt1 = line1[0];
        auto& line1pt2 = line1[1];
        double a1 = line1pt2.y - line1pt1.y;
        double b1 = line1pt1.x - line1pt2.x;
        double c1 = a1*(line1pt1.x) + b1*line1pt1.y;
    
        // Line2 represented as a2x + b2y = c2
        auto& line2pt1 = line2[0];
        auto& line2pt2 = line2[1];
        double a2 = line2pt2.y - line2pt1.y;
        double b2 = line2pt1.x - line2pt2.x;
        double c2 = a2*(line2pt1.x)+ b2*(line2pt1.y);
    
        double determinant = a1*b2 - a2*b1;
    
        if (determinant != 0) {
        
            double x = (b2*c1 - b1*c2)/determinant;
            double y = (a1*c2 - a2*c1)/determinant;
            
            //check if (x,y) lie in between line1 and line2. 
            //std::min(x1,x2) <= x <= std::max(x1,x2) and std::min(y1,y2) <= x <= std::max(y1,y2). 
            //Do this for both lines 
            if( std::min(line1pt1.x, line1pt2.x) <= x && x <= std::max(line1pt1.x, line1pt2.x) &&
                std::min(line1pt1.y, line1pt2.y) <= y && y <= std::max(line1pt1.y, line1pt2.y) &&
                std::min(line2pt1.x, line2pt2.x) <= x && x <= std::max(line2pt1.x, line2pt2.x) &&
                std::min(line2pt1.y, line2pt2.y) <= y && y <= std::max(line2pt1.y, line2pt2.y) ) 
            {
                    
                    result = true;
            }
            
        }

        return result;
    }
}

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
    mSprite.setOrigin(characterWidth/2.0f, characterHeight/2.0f);

    std::string idleRightSprite = "../assets/IdleRight.png";
    std::string runRightSprite = "../assets/RunRight.png";
    std::string jumpRightSprite = "../assets/JumpRight.png";
    std::string fallRightSprite = "../assets/FallRight.png";
    std::string idleLeftSprite = "../assets/IdleLeft.png";
    std::string runLeftSprite = "../assets/RunLeft.png";
    std::string jumpLeftSprite = "../assets/JumpLeft.png";
    std::string fallLeftSprite = "../assets/FallLeft.png";
    
    mTextures[(int)Movement::Idle][(int)Direction::Left].setup(idleLeftSprite, 0, 0, (int)characterWidth, (int)characterHeight, 5);
    mTextures[(int)Movement::Idle][(int)Direction::Right].setup(idleRightSprite, 0, 0, (int)characterWidth, (int)characterHeight, 5);
    mTextures[(int)Movement::Run][(int)Direction::Left].setup(runLeftSprite, 0, 0, (int)characterWidth, (int)characterHeight, 8);
    mTextures[(int)Movement::Run][(int)Direction::Right].setup(runRightSprite, 0, 0, (int)characterWidth, (int)characterHeight, 8);
    mTextures[(int)Movement::Jump][(int)Direction::Left].setup(jumpLeftSprite, 0, 0, (int)characterWidth, (int)characterHeight, 1);
    mTextures[(int)Movement::Jump][(int)Direction::Right].setup(jumpRightSprite, 0, 0, (int)characterWidth, (int)characterHeight, 1);
    mTextures[(int)Movement::Fall][(int)Direction::Left].setup(fallLeftSprite, 0, 0, (int)characterWidth, (int)characterHeight, 1);
    mTextures[(int)Movement::Fall][(int)Direction::Right].setup(fallRightSprite, 0, 0, (int)characterWidth, (int)characterHeight, 1);
    
}

void Character::update(const sf::Time& delta)
{
    mMovement = Movement::Idle;
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
        jumpInitialVelocity = { 0.0f , jumpYSpeed * pixelPerMeter }; // inital up speed on -8m/s
        isJumping = true;
    }

    // jump physics
    if(!isJumping) {
        //if not jumping then only character can only move in x-direction
        mVelocity.x = (direction.x * pixelPerMeter);//vel.x = 1m/s * direction
        mVelocity.y = 0.0f;
        mDisplacement.y = 0.0f;
        mDisplacement.x = mVelocity.x * delta.asSeconds();

        //check character doesn't leave right side of platform
        if (mSprite.getPosition().x + mDisplacement.x > mRestingPlatform->getPlatformXPosition().second) {
            mDisplacement.x = mRestingPlatform->getPlatformXPosition().second - (mSprite.getPosition().x);
        }

        //check character doesn't leave left side of platform
        if (mSprite.getPosition().x + mDisplacement.x < mRestingPlatform->getPlatformXPosition().first) {
            mDisplacement.x = mRestingPlatform->getPlatformXPosition().first - (mSprite.getPosition().x);
        }
    
    } else {
        
        // while jumping character can move both in x direction, thats why changing velocity of x in every update call
        // based on direction
        mVelocity.x = direction.x * pixelPerMeter * jumpXSpeed; // vel.x = 2.2m/s
        mVelocity.y = (jumpInitialVelocity.y + pixelPerMeter * Character::gravity * delta.asSeconds()); // acceleration = 9.8 m/s2 downwards
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

    mTextures[(int)mMovement][(int)mDirection].update(delta);
    mTextures[(int)mMovement][(int)mDirection].applyTexture(mSprite, mDirection);
    
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

    // bool result = true;

    //auto charYBottom = mSprite.getPosition().y + (characterHeight/2.0f);
    //auto platformYTop = p->getPlatformYPosition();
    //auto platformYBottom = p->getPlatformYPosition() + platformHeight;
    //
    ////std::cout << " displacement = " << mDisplacement.y << std::endl;
    //auto charXMid = mSprite.getPosition().x;
    //auto platformX = p->getPlatformXPosition();

    ////as height of platform is 10.0f. So if differnce in position within each update call is greater than 10.0f, then we have to use line intersection method to check collision
    ////This is too complex maybe something simpler
    //if (mDisplacement.y > platformHeight) {
    //    //use interpolation to calculate displacement at platform position with x is within x interval of platform
    //    
    //    std::vector<sf::Vector2f> line1 = { sf::Vector2f(platformX.first, platformYTop), sf::Vector2f(platformX.second, platformYTop) };
    //    std::vector<sf::Vector2f> line2 = { sf::Vector2f(charXMid, charYBottom), sf::Vector2f(charXMid + mDisplacement.x, charYBottom + mDisplacement.y) };
    //    result = calculateIntesection(line1, line2);
    //    
    //    return result;
    //}
    //
    ////std::cout << "Char Y Bottom = " << charYBottom << " Char Mid = " << charXMid << std::endl;
    ////std::cout << "Platform Y Top = " << platformYTop << " Platform Y Bottom = " << platformYBottom << std::endl;
    ////std::cout << "Platform X Left = " << platformX.first << "Platform X Right = " << platformX.second << std::endl;

    //// Here 2.5 is margin of error. There can be cases in which position sample are more than 10 units apart. For those cases, a 2.5 units
    //// margin of error is added. Therefor check will between platformTop and platformBottom + 2.5. If box positon lies in bettween this then 
    //// its a collision
    //if(charYBottom < platformYTop || charYBottom  > platformYBottom ) {
    //    return false;
    //}
    //
    //
    //
    //if(charXMid < platformX.first || charXMid > platformX.second) {
    //    return false;
    //}

    //AABB collision 

    //using a very tight bounding box for character
    auto charYBottom = mSprite.getPosition().y + (characterHeight/2.0f);
    auto charYTop = mSprite.getPosition().y; // bottom half bounding box of character is considered
    auto charXLeft = mSprite.getPosition().x - (characterWidth/4.0f); // 1/4 on each side from the origin
    auto charXRight = mSprite.getPosition().x + (characterWidth/4.0f);

    auto platformYTop = p->getPlatformYPosition();
    auto platformYBottom = p->getPlatformYPosition() + platformHeight;
    auto platformXLeft = p->getPlatformXPosition().first;
    auto platformXRight = p->getPlatformXPosition().second;

    if (charYBottom < platformYTop || platformYBottom < charYTop) {
        return false;
    }

    if (charXRight < platformXLeft || platformXRight < charXLeft) {
        return false;
    }

    return true;
}
    
void Character::updateRestingPlatform(Platform* p) {
    mRestingPlatform = p;
    isJumping = false;
    jumpInitialVelocity = {0.0f, 0.0f};
    mSprite.setPosition(sf::Vector2f(mSprite.getPosition().x, mRestingPlatform->getPlatformYPosition()- platformOutlineThickness - characterHeight/2.0f - 1/*for padding*/));
    
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


//constructor
Animation::Animation() :
    mTimeSinceLastFrame(sf::Time::Zero),
    mTexture(),
    currentTextRect(0),
    currentRunDir(Direction::Right)
{}

void Animation::setup(std::string name, int x, int y, int width, int height, int numFrames) 
{
    if (!mTexture.loadFromFile(name)) {
        //Handle error
        std::cout << "Failed to load texture";
    }
    for (int i = 0; i < numFrames; i++) {
        frames.push_back(sf::IntRect(i * width, 0, width, height));
    }

}

void Animation::update(sf::Time delta) {
    mTimeSinceLastFrame += delta;
    while (mTimeSinceLastFrame >= holdTime) {
        mTimeSinceLastFrame -= holdTime;
        step();
    }
}

void Animation::step() {
    currentTextRect++;
    if (currentTextRect >= frames.size()) {
        currentTextRect = 0;
    }
}

void Animation::applyTexture(sf::Sprite& sp, Direction dir) {
    
    sp.setTexture(mTexture);
    sp.setTextureRect(frames[currentTextRect]);
    
}