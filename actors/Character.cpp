#include "Character.hpp"

#include "../util/NumberUtil.hpp"
#include "../util/CoordinateUtil.hpp"
#include "../game/ID.hpp"

#include<iostream>
#include<math.h>
#include<list>
using namespace std;

Character::Character(float x, float y, float width, float height, float speed, Sprite *sprite)
{
    this->speed = speed;

    sprite->SetCenter(sprite->GetSize().x/2, sprite->GetSize().y/2);
    sprite->SetX(x);
    sprite->SetY(y);
    sprite->Resize(width, height);
    this->sprite = sprite;

    currentGround = 0;
    prevGround = 0;
    inJump = false;

    doMoveLeft = false;
    doMoveRight = false;
}

Character::~Character()
{
    if(sprite)
        delete sprite;
}

void Character::landHop()
{
    if(!inJump)
    {
        cout << "Island Jump" << endl;
        prevGround = currentGround;
        currentGround = 0;
        inJump = true;
    }
}

void Character::moveLeft()
{
    doMoveLeft = true;
}

void Character::moveRight()
{
    doMoveRight = true;
}

void Character::update(Clock *clock, RenderWindow *window, World *world)
{
    // Un-comment the line below to pause the game on the first frame
    //return;

    float elapsedTime = clock->GetElapsedTime();
    CoordinateUtil coordUtil;

    float velocityX = cos(sprite->GetRotation() * M_PI / 180) * (speed * elapsedTime);
    float velocityY = sin(sprite->GetRotation() * M_PI / 180) * (speed * elapsedTime);

    subUpdate(velocityX, velocityY, clock, window, world);

    // Respond to user input events
    if(doMoveRight) // Move right
    {
        sprite->Move(velocityX, -velocityY);
        doMoveRight = false;
    }

    if(doMoveLeft) // Move left
    {
        sprite->Move(-velocityX, velocityY);
        doMoveLeft = false;
    }

    int lookDepth = 50; // Depth to ray-trace
    //cout << sprite->GetSize().x << " " << sprite->GetSize().y << endl;
    int lookOffset = 50; // Distance between left and right ray-traces

    // Perform collision detection
    if(currentGround == 0)
    {
        // Get the nearest island
        list<GameObject*>* objects = world->getObjects();
        for(list<GameObject*>::iterator it = objects->begin(); it != objects->end(); it++)
        {
            if((*it)->getID() == ID_ISLAND && prevGround != *it) // Ignore the previous ground
            {
                //float distance = coordUtil.distance(sprite->GetPosition(), (*it)->getPosition());
                if(coordUtil.collide(sprite, ((Island*)(*it))->getSprite()))
                {
                    // Mark the new ground as the current ground
                    prevGround = currentGround;
                    currentGround = (Island*)(*it);

                    //cout << currentGround->getPosition().x << " " << currentGround->getPosition().y << " - " << elapsedTime << endl;

                    // Rotate to the new ground
                    float angle = atan2(currentGround->getPosition().y - sprite->GetPosition().y, currentGround->getPosition().x - sprite->GetPosition().x);
                    angle = angle * (180.0f/M_PI); // Convert the angle from radians to degrees
                    sprite->SetRotation(-angle + 90); // Rotate to the correct angle

                    cout << "found new ground" << endl;
                    break;
                }
            }
        }
    }

    if(currentGround == 0)
    {
        float angle = -sprite->GetRotation();
        if(inJump)
        {
            angle -= 90; // Up (jump)
            //cout << "up"  << endl;
        } else {
            angle += 90; // Down (gravity)
            //cout << "down" << endl;
        }

        velocityX = cos(angle * M_PI / 180) * (speed * elapsedTime);
        velocityY = sin(angle * M_PI / 180) * (speed * elapsedTime);
        sprite->Move(velocityX, velocityY);

        return;
    }/*else{
        inJump = false;
    }*/

    // Get the pixels directly under the player
    Sprite *groundSprite = currentGround->getSprite();

    // Get the position at the bottom of the charcter
    // Global Positions
    Vector2f bottomLeft = sprite->TransformToGlobal(Vector2f((sprite->GetSize().x/2) - lookOffset, sprite->GetSize().y));
    Vector2f bottomMiddle = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y));
    Vector2f bottomRight = sprite->TransformToGlobal(Vector2f((sprite->GetSize().x/2) + lookOffset, sprite->GetSize().y));
    Vector2f bottomMiddleUp = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y - 5)); // 5 pixels up from base
    // Local positions to ground
    bottomLeft = groundSprite->TransformToLocal(bottomLeft);
    bottomMiddle = groundSprite->TransformToLocal(bottomMiddle);
    bottomRight = groundSprite->TransformToLocal(bottomRight);
    bottomMiddleUp = groundSprite->TransformToLocal(bottomMiddleUp);

    // Get the position at the ray-trace distance
    // Global Positions
    Vector2f targetLeft = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    Vector2f targetMiddle = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    Vector2f targetRight = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    // Local positions to ground
    targetLeft = groundSprite->TransformToLocal(targetLeft);
    targetMiddle = groundSprite->TransformToLocal(targetMiddle);
    targetRight = groundSprite->TransformToLocal(targetRight);

    // Land directly below
    Vector2f *leftCollide = rayTrace(groundSprite, bottomLeft.x, bottomLeft.y, targetLeft.x, targetLeft.y);
    Vector2f *middleCollide = rayTrace(groundSprite, bottomMiddle.x, bottomMiddle.y, targetMiddle.x, targetMiddle.y);
    Vector2f *rightCollide = rayTrace(groundSprite, bottomRight.x, bottomRight.y, targetRight.x, targetRight.y);

    // Resolve the problem of only one ray-trace finding land
    /*if(rightCollide == 0 && leftCollide != 0)
    {
        //rightCollide = new Vector2f(bottomRight.x, leftCollide->y + 5);
        rightCollide = &groundSprite->TransformToGlobal(Vector2f(bottomRight.x, leftCollide->y));
        rightCollide = &sprite->TransformToLocal(*rightCollide);
        rightCollide->y += 10;
        rightCollide = &sprite->TransformToGlobal(*rightCollide);
        rightCollide = &groundSprite->TransformToLocal(*rightCollide);
        cout << "Right Collide missing" << endl;
    }else if(rightCollide != 0 && leftCollide == 0){
        //leftCollide = new Vector2f(bottomLeft.x, rightCollide->y + 5);
        leftCollide = &groundSprite->TransformToGlobal(Vector2f(bottomLeft.x, rightCollide->y));
        leftCollide = &sprite->TransformToLocal(*leftCollide);
        leftCollide->y += 10;
        leftCollide = &sprite->TransformToGlobal(*leftCollide);
        leftCollide = &groundSprite->TransformToLocal(*leftCollide);
        cout << "Left Collide missing" << endl;
    }*/

    if(leftCollide == 0 || rightCollide == 0)
    {
        // No land below. Move with relative gravity
        float angle = -sprite->GetRotation() + 90;
        velocityX = cos(angle * M_PI / 180) * (speed * elapsedTime);
        velocityY = sin(angle * M_PI / 180) * (speed * elapsedTime);
        sprite->Move(velocityX, velocityY);
    }else{
        inJump = false;
        // Draw the ground line for debugging purposes
        int groundTop = groundSprite->GetPosition().y - (groundSprite->GetSize().y/2);
        int groundLeft = groundSprite->GetPosition().x - (groundSprite->GetSize().x/2);
        line = Shape::Line(leftCollide->x + groundLeft, leftCollide->y + groundTop, rightCollide->x + groundLeft, rightCollide->y + groundTop, 1, Color::White);

        // Rotate to the correct angle
        float angle = atan2(rightCollide->y - leftCollide->y, rightCollide->x - leftCollide->x);
        angle = angle * (180.0f/M_PI); // Convert the angle from radians to degrees
        sprite->SetRotation(-angle); // Rotate to the correct angle

        // Move up or down depending on distance with ground.
        if(middleCollide != 0)
        {
            // Get the distance to the ground
            float distance = coordUtil.distance(bottomMiddle, *middleCollide);

            // Only move up or down if the distance is not acceptable
            bool adjustPosition = false;

            if(bottomMiddleUp.x >= groundSprite->GetSize().x || bottomMiddleUp.y >= groundSprite->GetSize().y)
            {
                adjustPosition = true;
            }else{
                Color groundAbovePixel = groundSprite->GetPixel(bottomMiddleUp.x, bottomMiddleUp.y);
                if(groundAbovePixel.a > 0)
                    adjustPosition = true;
            }

            if(adjustPosition)
            {
                Color pixel;

                // Get the pixel of the ground
                if(coordUtil.isLocalPointInside(bottomMiddle, *groundSprite)) // Ground exists, get the pixel.
                    pixel = groundSprite->GetPixel(bottomMiddle.x, bottomMiddle.y);
                else // Ground does not exist, assume transparent.
                    pixel = Color(0, 0, 0, 0);

                float reverseAngle = angle;
                if(pixel.a == 0) // Transparent
                {
                    // Move down
                    reverseAngle += 90;
                }else{ // Not transparent
                    // Move up
                    reverseAngle -= 90;
                }

                // Move the charcter approprietly
                velocityX = cos((reverseAngle * M_PI) / 180) * (speed * elapsedTime);
                velocityY = sin((reverseAngle * M_PI) / 180) * (speed * elapsedTime);
                sprite->Move(velocityX, velocityY);
            }
        }
    }

    // Perform cleanup.
    delete leftCollide;
    delete middleCollide;
    delete rightCollide;
}

Vector2f* Character::rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY)
{
    //printf("fromX: %d\tfromY: %d\ttoX: %d\ttoY: %d\n", fromX, fromY, toX, toY);
    // Simplified Bresenham line algorithm
    // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    // x0 = fromX   y0 = fromY  x1 = toX    y1 = toY

    // Clamp positions to within the bounds of the sprite
    clamp(fromX, 0, sprite->GetSize().x - 1);
    clamp(fromY, 0, sprite->GetSize().y - 1);
    clamp(toX, 0, sprite->GetSize().x - 1);
    clamp(toY, 0, sprite->GetSize().y - 1);

    // Declare the required variables
    Vector2f *collidePosition = 0;
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

        if(pixel.a > 0) // Not transparent
        {
            collidePosition = new Vector2f(fromX, fromY);
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

void Character::draw(RenderWindow *window)
{
    // Draw the charcter.
    window->Draw(*sprite);

    // Draw the ground line for debugging.
    window->Draw(line);
}

const Vector2f& Character::getPosition()
{
    return sprite->GetPosition();
}

const Vector2f& Character::getSize()
{
    return sprite->GetSize();
}

float Character::getRotation()
{
    return sprite->GetRotation();
}

Sprite* Character::getSprite()
{
    return sprite;
}
