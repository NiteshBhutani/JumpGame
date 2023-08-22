#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
 
int random(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}


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

    Platform(float width, float y, float x) :
        mSize(sf::Vector2f(width, 10)),
        mSprite(),
        mVelocity(0.0, 50.0f)
    {
        mSprite.setPosition(x, y);
        mSprite.setSize(mSize);
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
        jumpInitialVelocity(0.0f, 0.0f),
        isJumping(false)
    {
        mSprite.setPosition(pos);
        mSprite.setFillColor(sf::Color::White);
        mSprite.setOutlineColor(sf::Color::Blue);
        mSprite.setOutlineThickness(3);
        mSprite.setSize(sf::Vector2f(50, 50));

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
        
        
    }
    void draw(sf::RenderTarget& window) {
        window.draw(mSprite);
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
    {
        auto platformActorPair1 = std::make_pair(Platform(random(100,mWindowWidth/3.0f), 0, random(0, mWindowWidth/2.0f)), nullptr);
        auto platformActorPair2 = std::make_pair(Platform(random(100,mWindowWidth/4.0f), 300,random(mWindowWidth/4.0f, mWindowWidth/2.0f)), nullptr);
        auto platformActorPair3 = std::make_pair(Platform(random(100,mWindowWidth/2.0f),500, random(mWindowWidth/4.0f, 3 * (mWindowWidth/4.0f) )), nullptr);
        
        platform.emplace_back(platformActorPair1);
        platform.emplace_back(platformActorPair2);
        platform.emplace_back(platformActorPair3);
        

    }
    
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
    std::vector<std::pair<Platform, Character*>> platform;
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
    for(auto& p : platform)
    {
        p.first.update(delta);
    }
}

void App::render() {
    // clear the window with black color
    mWindow.clear();

    // draw everything here...
    actor.draw(mWindow);
    for(auto& p : platform)
    {
        p.first.draw(mWindow);
    }
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


