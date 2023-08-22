#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>


class Platform {
public:
    Platform():
        mSize(sf::Vector2f(400, 10)),
        mSprite(),
        mVelocity(0.0, 50.0f)
    {
        mSprite.setPosition(0.0f, 0.0f);
        mSprite.setSize(mSize);
        mSprite.setFillColor(sf::Color::Yellow);
        mSprite.setOutlineColor(sf::Color::Red);
        mSprite.setOutlineThickness(2);
    }

    Platform(float width, float x) :
        mSize(sf::Vector2f(width, 10)),
        mSprite(),
        mVelocity(0.0, 100.0f)
    {
        mSprite.setPosition(x, 0.0f);
        mSprite.setFillColor(sf::Color::Yellow);
        mSprite.setOutlineColor(sf::Color::Red);
        mSprite.setOutlineThickness(2);
    }

    void update(const sf::Time& delta) {
        sf::Vector2f displacement = mVelocity * delta.asSeconds();

        mSprite.move(displacement);
    }

    void draw(sf::RenderTarget& target) {
        target.draw(mSprite);
    }
private:
    sf::Vector2f mSize;
    sf::Vector2f mVelocity;
    sf::RectangleShape mSprite;
};
class Character {
public:
    Character(const sf::Vector2f& pos) :
        mSprite(),
        mVelocity(0.0f, 0.0f),
        mDisplacement(0.0f,0.0f),
        mFloor(),
        jumpInitialVelocity(0.0f, 0.0f),
        isJumping(false)
    {
        mSprite.setPosition(pos);
        mSprite.setFillColor(sf::Color::White);
        mSprite.setOutlineColor(sf::Color::Blue);
        mSprite.setOutlineThickness(3);
        mSprite.setSize(sf::Vector2f(50, 50));

        mFloor.setSize(sf::Vector2f(800,10));
        mFloor.setPosition(0, pos.y + 100);
    }

    void update(const sf::Time& delta)
    {
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

        
        mVelocity.x = (jumpInitialVelocity.x + direction.x * speedRate);
        mVelocity.y = (jumpInitialVelocity.y + gravityRate * Character::gravity * delta.asSeconds());
        mDisplacement.x = mVelocity.x * delta.asSeconds();
        mDisplacement.y = (jumpInitialVelocity.y * delta.asSeconds() + 0.5f * gravityRate * gravity * delta.asSeconds() * delta.asSeconds());
        
        mSprite.move(mDisplacement);

        jumpInitialVelocity.y = mVelocity.y;
        
        //we hit the floor
        if (mSprite.getPosition().y + 50 > mFloor.getPosition().y) {
            auto x = mSprite.getPosition().x;
            mSprite.setPosition(x, mFloor.getPosition().y - 50);
            jumpInitialVelocity = { 0.0f, 0.0f };
            isJumping = false;
        }
        
    }
    void draw(sf::RenderTarget& window) {
        window.draw(mSprite);
        window.draw(mFloor);
    }
    
public:
    static const float speedRate;
    static const float gravityRate;
    static const float gravity;
private:
    sf::RectangleShape mSprite;
    sf::Vector2f mDisplacement;
    sf::Vector2f mVelocity;
    
    
    // When the up arrow is pressed how much vertical velocity to give
    sf::Vector2f jumpInitialVelocity; 
    bool isJumping;


    sf::RectangleShape mFloor;
    
};

const float Character::speedRate = 150;
const float Character::gravityRate = 100;
const float Character::gravity = 9.8f;

class App {
public:
    App(unsigned int width, unsigned int height):
        mWindowWidth(width),
        mWindowHeight(height),
        mWindow(sf::VideoMode(width,height), "JumpGame"),
        actor(sf::Vector2f(width/2.f, height/2.f)),
        platform()
    {}
    
    void processEvents();
    void update(const sf::Time& delta);
    void run();
    void render();


public:
    static const sf::Time timePerFrame;
    
private:
    unsigned int mWindowWidth;
    unsigned int mWindowHeight;
    sf::RenderWindow mWindow;
    Character actor;
    Platform platform;
};

const sf::Time App::timePerFrame = sf::seconds(1.f / 60.f);

void App::processEvents()  {
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void App::update(const sf::Time& delta) {
    actor.update(delta);
    platform.update(delta);
}

void App::render() {
    // clear the window with black color
    mWindow.clear();

    // draw everything here...
    actor.draw(mWindow);
    platform.draw(mWindow);
    // end the current frame
    mWindow.display();
}

void App::run() {
    //Game Loop
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        sf::Time loopTime = clock.restart();
        timeSinceLastUpdate += loopTime;
        // process and update should follow 60FPS
        while (timeSinceLastUpdate >= timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            
            processEvents();
            update(App::timePerFrame);
        }
        

        render();
    }
}
int main()
{
    App app(800,600);
    app.run();

    return 0;
}


