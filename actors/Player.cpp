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
    int lookXLeft = sprite->GetPosition().x - (sprite->GetSize().x/2);
    int lookXRight = sprite->GetPosition().x + (sprite->GetSize().x/2);

    // Get where I will look (position of next image).
    int targetXLeft = lookXLeft;
    int targetXRight = lookXRight;
    int targetTop = groundSprite->GetPosition().y - (groundSprite->GetSize().y/2);
    int playerBottom = sprite->GetPosition().y + (sprite->GetSize().y/2);
    int targetY = playerBottom - targetTop;


    //Vector2i *leftCollide = rayTrace(groundSprite, lookXLeft, targetY, targetY + 10);

    if(playerBottom < targetTop)
    {
        sprite->Move(0, speed * elapsedTime);
        return;
    }

    Vector2i *leftCollide = rayTrace(groundSprite, lookXLeft, targetY, targetY + 5);

    if(leftCollide == 0)
    {
        // Move down
        sprite->Move(0, speed * elapsedTime);
    }else{
        /*Color pixel = groundSprite->GetPixel(leftCollide->x, leftCollide->y);

        if(pixel.a < 200)
            sprite->Move(0, speed * elapsedTime);
        else
            sprite->Move(0, -speed * elapsedTime);*/

        int groundDistance = targetY - leftCollide->y;
        if(groundDistance == 0)
            sprite->Move(0, -speed * elapsedTime);

        delete leftCollide;
    }
}

Vector2i* Player::rayTrace(Sprite *sprite, int fromX, int fromY, int toY)
{
    // Get the sprite
    Vector2i *collidePosition = 0;

    for(int y = fromY; y <= toY; y++)
    {
        Color pixel = sprite->GetPixel(fromX, y);
        if(pixel.a > 200) // Not transparent
        {
            collidePosition = new Vector2i(fromX, y);
            break;
        }
    }

    return collidePosition;
}

void Player::draw(RenderWindow *window)
{
    window->Draw(*sprite);
}
