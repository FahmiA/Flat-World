#include "Character.hpp"

#include "util/NumberUtil.hpp"
#include "game/ID.hpp"

#include<iostream>
#include<math.h>
#include<list>
using namespace std;

Character::Character(float x, float y, float width, float height, float speed, Sprite *sprite)
{
    this->speed = speed;

    sprite->Resize(width, height);
    //sprite->SetCenter(sprite->GetSize().x/2, sprite->GetSize().y/2);
    sprite->SetCenter(width/2, height/2);
    sprite->SetX(x);
    sprite->SetY(y);

    this->sprite = sprite;

    currentGround = 0;
    prevGround = 0;
    inJump = false;
    distanceFromGround = 0;

    doMoveLeft = false;
    doMoveRight = false;
    facingDirection = Left;
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
    facingDirection = Left;
    doMoveLeft = true;
    sprite->FlipX(false);
}

void Character::moveRight()
{
    facingDirection = Right;
    doMoveRight = true;
    sprite->FlipX(true);
}

Direction Character::getFacingDirection()
{
    return facingDirection;
}

void Character::update(Clock *clock, RenderWindow *window, World *world)
{
    // Un-comment the line below to pause the game on the first frame
    //return;

    // Get the elapsed time since the last frame
    float elapsedTime = clock->GetElapsedTime();

    // Allow sub-classes to invoke specific behaviour
    subUpdate(clock, window, world);

    // Find the nearest island and assign it as the island the Character is on
    findCurrentIsland(world->getIslands());

    // Move in responce to user input
    steer(elapsedTime);

    // Stick tthe Character to the current island.
    lockToIsland(elapsedTime);
}

void Character::findCurrentIsland(list<Island*>* islands)
{
    // Get the nearest island if no current island exists
    if(currentGround == 0)
    {
        // Loop through all islands
        Island *island;
        for(list<Island*>::iterator it = islands->begin(); it != islands->end(); it++)
        {
            island = *it;
            if(prevGround != island) // Ignore the previous ground
            {
                // Check if the charcter is close to the island
                if(coordUtil.collide(sprite, island->getSprite()))
                {
                    // Mark the new ground as the current ground
                    prevGround = currentGround;
                    currentGround = island;

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
}

void Character::steer(float elapsedTime)
{
     // Get the x and y movement velocity
    float velocityX = cos(sprite->GetRotation() * M_PI / 180) * (speed * elapsedTime);
    float velocityY = sin(sprite->GetRotation() * M_PI / 180) * (speed * elapsedTime);

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

    // If no current island exists, keep jumping
    if(currentGround == 0)
    {
        float angle = -sprite->GetRotation();
        if(inJump)
        {
            angle -= 90; // Up (jump)
        } else {
            angle += 90; // Down (gravity)
        }

        // Get the new movement velocity
        velocityX = cos(angle * M_PI / 180) * (speed * elapsedTime);
        velocityY = sin(angle * M_PI / 180) * (speed * elapsedTime);

        // Move the character
        sprite->Move(velocityX, velocityY);
    }
}

void Character::lockToIsland(float elapsedTime)
{
    // Can't lock to island if no island currently exists (eg: in jump)
    if(currentGround == 0)
        return;

    int lookDepth = 50; // Depth to ray-trace
    int lookOffset = 50; // Distance between left and right ray-traces

    // Get the Sprite directly under the player
    Sprite *groundSprite = currentGround->getSprite();

    // Get the position at the bottom of the charcter
    // Global Positions
    Vector2f bottomLeft = sprite->TransformToGlobal(Vector2f((sprite->GetSize().x/2) - lookOffset, sprite->GetSize().y));
    Vector2f bottomMiddle = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + distanceFromGround));
    Vector2f bottomRight = sprite->TransformToGlobal(Vector2f((sprite->GetSize().x/2) + lookOffset, sprite->GetSize().y));
    Vector2f bottomMiddleUp = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y - 5 + distanceFromGround)); // 5 pixels up from base
    // Transform global positions to local ground positions
    bottomLeft = groundSprite->TransformToLocal(bottomLeft);
    bottomMiddle = groundSprite->TransformToLocal(bottomMiddle);
    bottomRight = groundSprite->TransformToLocal(bottomRight);
    bottomMiddleUp = groundSprite->TransformToLocal(bottomMiddleUp);

    // Get the position at the ray-trace distance
    // Global Positions
    Vector2f targetLeft = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    Vector2f targetMiddle = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    Vector2f targetRight = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    // Transform global positions to local ground positions
    targetLeft = groundSprite->TransformToLocal(targetLeft);
    targetMiddle = groundSprite->TransformToLocal(targetMiddle);
    targetRight = groundSprite->TransformToLocal(targetRight);

    // Land directly below
    Vector2f *leftCollide = spriteUtil.rayTrace(groundSprite, bottomLeft.x, bottomLeft.y, targetLeft.x, targetLeft.y);
    Vector2f *middleCollide = spriteUtil.rayTrace(groundSprite, bottomMiddle.x, bottomMiddle.y, targetMiddle.x, targetMiddle.y);
    Vector2f *rightCollide = spriteUtil.rayTrace(groundSprite, bottomRight.x, bottomRight.y, targetRight.x, targetRight.y);

    if(leftCollide == 0 || rightCollide == 0)
    {
        // No land below. Move with relative gravity
        float angle = -sprite->GetRotation() + 90;
        float velocityX = cos(angle * M_PI / 180) * (speed * elapsedTime);
        float velocityY = sin(angle * M_PI / 180) * (speed * elapsedTime);
        sprite->Move(velocityX, velocityY);
    }else{
        inJump = false;
        // Draw the ground line for debugging purposes
        Vector2f lineLeftPoint = groundSprite->TransformToGlobal(*leftCollide);
        Vector2f lineRightPoint = groundSprite->TransformToGlobal(*rightCollide);
        line = Shape::Line(lineLeftPoint, lineRightPoint, 1, Color::White);

        // Rotate to the correct angle
        float angle = atan2(rightCollide->y - leftCollide->y, rightCollide->x - leftCollide->x);
        angle = angle * (180.0f/M_PI); // Convert the angle from radians to degrees
        sprite->SetRotation(-angle); // Rotate to the correct angle

        // Move up or down depending on distance with ground
        if(middleCollide != 0)
        {
            // Only move up or down if the distance is not acceptable
            Color groundPixel(0, 0, 0, 0);
            Color groundAbovePixel(0, 0, 0, 0);

            // Get the pixel of the ground
            if(coordUtil.isLocalPointInside(bottomMiddle, *groundSprite)) // Ground exists, get the pixel.
                groundPixel = groundSprite->GetPixel(bottomMiddle.x, bottomMiddle.y);
            // Else ground does not exist, assume transparent.

            // Get the pixel of the position above ground
            if(coordUtil.isLocalPointInside(bottomMiddleUp, *groundSprite)) // Ground exists, get the pixel.
                groundAbovePixel = groundSprite->GetPixel(bottomMiddleUp.x, bottomMiddleUp.y);
            // Else ground does not exist, assume transparent.

            if(groundPixel.a == 0 || groundAbovePixel.a > 0)
            {
                float reverseAngle = angle;
                if(groundPixel.a == 0) // Transparent
                {
                    // Move down
                    reverseAngle += 90;
                }else{ // Not transparent
                    // Move up
                    reverseAngle -= 90;
                }

                // Move the charcter approprietly
                float adjustSpeed = speed/3; // The Speed to adjust the character automaticly must be small to avoid jerkiness
                float velocityX = cos((reverseAngle * M_PI) / 180) * (adjustSpeed * elapsedTime);
                float velocityY = sin((reverseAngle * M_PI) / 180) * (adjustSpeed * elapsedTime);
                sprite->Move(velocityX, velocityY);
            }
        }
    }

    // Perform cleanup.
    delete leftCollide;
    delete middleCollide;
    delete rightCollide;
}

CoordinateUtil& Character::getCoordinateUtil()
{
    return coordUtil;
}

SpriteUtil& Character::getSpriteUtil()
{
    return spriteUtil;
}

void Character::setDistanceFromGround(float distance)
{
    distanceFromGround = distance;
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
    float rotation = sprite->GetRotation();
    rotation *= M_PI / 180.0f;
    return rotation;
}

Sprite* Character::getSprite()
{
    return sprite;
}
