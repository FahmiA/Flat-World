#include "Player.hpp"

#include "../game/World.hpp"
#include "../environment/Island.hpp"

#include<list>
using namespace std;

Player::Player(float x, float y, float width, float height, float speed, Sprite *sprite)
{
    this->speed = speed;

    sprite->SetCenter(sprite->GetImage()->GetWidth()/2, sprite->GetImage()->GetHeight()/2);
    sprite->SetX(x);
    sprite->SetY(y);
    sprite->Resize(width, height);
    this->sprite = sprite;
}

Player::~Player()
{
    delete sprite;
}

void Player::update(Clock *clock, RenderWindow *window, World *world)
{
    float elapsedTime = clock->GetElapsedTime();

    // Respond to user input events
    if(window->GetInput().IsKeyDown(Key::Right))
        sprite->Move(speed * elapsedTime, 0);

    if(window->GetInput().IsKeyDown(Key::Left))
        sprite->Move(-speed * elapsedTime, 0);

    // Perform collision detection
    list<GameObject*>* objects = world->getObjects();
    Island *ground = (Island*)(objects->front());

    // Get the pixels directly under the player
    Sprite *groundSprite = ground->getSprite();

    // Get where I want to look (bottom of image).
    int lookX = sprite->GetPosition().x;
    int lookY = sprite->GetPosition().y + (sprite->GetSize().y/2);

    // Get where i will look (position of next image).
    int targetX = lookX;
    int targetTop = groundSprite->GetPosition().y - (groundSprite->GetSize().y/2);
    int playerBottom = lookY;
    int targetY = playerBottom - targetTop;

    if(playerBottom < targetTop)
    {
        sprite->Move(0, speed * elapsedTime);
        return;
    }

    Color pixel = groundSprite->GetPixel(targetX, targetY);
    if(pixel.a < 200)
         sprite->Move(0, speed * elapsedTime);
    else
        sprite->Move(0, -speed * elapsedTime);
}

void Player::draw(RenderWindow *window)
{
    window->Draw(*sprite);
}
