#include "Player.hpp"

#include "../game/World.hpp"
#include "../environment/Island.hpp"

#include <stdio.h>
#include<iostream>
#include<math.h>
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

    if(playerBottom < targetTop)
    {
        sprite->Move(0, speed * elapsedTime);
        return;
    }
    static float prevAngle = 0.0f;

    Vector2i *leftCollide = rayTrace(groundSprite, targetXLeft, targetY, lookXLeft, targetY + 5);
    Vector2i *rightCollide = rayTrace(groundSprite, targetXRight, targetY, lookXRight, targetY + 5);

    if(leftCollide == 0 || rightCollide == 0)
    {
        // Move down
        sprite->Move(0, speed * elapsedTime);
    }else{
        int groundDistanceLeft = targetY - leftCollide->y;
        int groundDistanceRight = targetY - rightCollide->y;
        float angle = atan2(groundDistanceRight - groundDistanceLeft, targetXRight - targetXLeft);
        angle = angle * (180.0f/3.14156f);

        sprite->Rotate(-prevAngle);
        sprite->Rotate(angle);
        prevAngle = angle;

        if(groundDistanceRight == 0)
        {
            sprite->Move(0, -speed * elapsedTime);
        }

        delete leftCollide;
        delete rightCollide;
    }
}

Vector2i* Player::rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY)
{
    //printf("fromX: %d\tfromY: %d\ttoX: %d\ttoY: %d\n", fromX, fromY, toX, toY);
    // Simplified Bresenham line algorithm
    // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    // x0 = fromX   y0 = fromY  x1 = toX    y1 = toY

    // Declare the required variables
    Vector2i *collidePosition = 0;
    int differenceX = abs(toX - fromX);
    int differenceY = abs(toY - fromY);
    int error = differenceX - differenceY;
    int slopeX = (fromX < toX) ? 1 : -1;
    int slopeY = (fromY < toY) ? 1 : -1;
    int error2 = 0;

    while(true)
    {
        // Check pixel (fromX, fromY)
        Color pixel = sprite->GetPixel(fromX, fromY);

        if(pixel.a > 200) // Not transparent
        {
            collidePosition = new Vector2i(fromX, fromY);
            break;
        }

        if(fromX == toX && fromY == toY)
            break;

        error2 = 2 * error;
        if(error2 > -differenceY)
        {
            error = error - differenceY;
            fromX = fromX + slopeX;
        }

        if(error2 < differenceX){
            error = error + differenceX;
            fromY = fromY + slopeY;
        }
    }
    return collidePosition;
}

void Player::draw(RenderWindow *window)
{
    window->Draw(*sprite);
}
