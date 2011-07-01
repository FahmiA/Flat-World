#include "Player.hpp"

#include "../game/World.hpp"
#include "../environment/Island.hpp"

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

    float velocityX = cos(sprite->GetRotation() * M_PI / 180) * (speed * elapsedTime);
    float velocityY = sin(sprite->GetRotation() * M_PI / 180) * (speed * elapsedTime);

    // Respond to user input events
    if(window->GetInput().IsKeyDown(Key::Right))
        sprite->Move(velocityX, -velocityY);

    if(window->GetInput().IsKeyDown(Key::Left))
        sprite->Move(-velocityX, velocityY);

    if(window->GetInput().IsKeyDown(Key::R))
        sprite->Rotate(100 * elapsedTime);

    // Perform collision detection
    list<GameObject*>* objects = world->getObjects();
    Island *ground = (Island*)(objects->front());

    // Get the pixels directly under the player
    Sprite *groundSprite = ground->getSprite();

    Vector2f bottomLeft = sprite->TransformToGlobal(Vector2f(0, sprite->GetSize().y));
    Vector2f bottomRight = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x, sprite->GetSize().y));
    bottomLeft = groundSprite->TransformToLocal(bottomLeft);
    bottomRight = groundSprite->TransformToLocal(bottomRight);

    Vector2f targetLeft = sprite->TransformToGlobal(Vector2f(0, sprite->GetSize().y + 5));
    Vector2f targetRight = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x, sprite->GetSize().y + 5));
    targetLeft = groundSprite->TransformToLocal(targetLeft);
    targetRight = groundSprite->TransformToLocal(targetRight);

    //cout << "lx: " << targetLeft.x <<"\tlt: " << targetLeft.y << "\trx: " << targetRight.x << "\try: " << targetRight.y << endl;
    int groundTop = groundSprite->GetPosition().y - groundSprite->GetSize().y/2;
    line = Shape::Line(targetLeft.x, targetLeft.y + groundTop, targetRight.x, targetRight.y + groundTop, 1, Color::White);

    if(bottomLeft.y >= 0 && bottomRight.y >= 0)
    {
        // Land directly below

        Vector2i *leftCollide = rayTrace(groundSprite, bottomLeft.x, bottomLeft.y, targetLeft.x, targetLeft.y);
        Vector2i *rightCollide = rayTrace(groundSprite, bottomRight.x, bottomRight.y, targetRight.x, targetRight.y);

        if(leftCollide == 0 || rightCollide == 0)
        {
            // Move down (with gravity)
            velocityX = cos(90 * M_PI / 180) * (speed * elapsedTime);
            velocityY = sin(90 * M_PI / 180) * (speed * elapsedTime);
            sprite->Move(velocityX, velocityY);
        }else{
            // Rotate to the correct angle and move up (against gravity)
            //cout << rightCollide->y << " " << leftCollide->y << " " << rightCollide->x << " " << leftCollide->x << endl;
            float angle = atan2(rightCollide->y - leftCollide->y, rightCollide->x - leftCollide->x);
            angle = angle * (180.0f/M_PI);

            sprite->SetRotation(-angle); // Rotate to the correct angle

            Color pixel = groundSprite->GetPixel(bottomLeft.x, bottomLeft.y);
            if(pixel.a > 200)
            {
                velocityX = cos((angle - 90) * M_PI / 180) * (speed * elapsedTime);
                velocityY = sin((angle - 90) * M_PI / 180) * (speed * elapsedTime);
                sprite->Move(velocityX, velocityY);
            }
        }

        delete leftCollide;
        delete rightCollide;
    }else if(bottomLeft.y < 0 && bottomRight.y < 0){
        // No land below
        // Move down (with gravity)
        velocityX = cos(90 * M_PI / 180) * (speed * elapsedTime);
        velocityY = sin(90 * M_PI / 180) * (speed * elapsedTime);
        sprite->Move(velocityX, velocityY);
    }else if(bottomRight.y >= 0){
        // Land to the right
        sprite->Rotate(1);
    }else if(bottomLeft.y >= 0){
        // Land to the left
        sprite->Rotate(-1);
        //cout << "here " << elapsedTime << endl;
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

    window->Draw(line);
}
