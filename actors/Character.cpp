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

bool Character::isJumping()
{
    return inJump;
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

    // Stick the Character to the current island.
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
                    if(currentGround != 0) // Only set the previous ground if a current ground exists
                        prevGround = currentGround;
                    currentGround = island;

                    // Rotate to the new ground
                    float angle = atan2(currentGround->getPosition().y - sprite->GetPosition().y, currentGround->getPosition().x - sprite->GetPosition().x);
                    angle = angle * (180.0f/M_PI); // Convert the angle from radians to degrees
                    //sprite->SetRotation(-angle + 90); // Rotate to the correct angle

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

    // Variables to configure ground collision
    int lookDepth = 50; // Depth to ray-trace
    int lookOffset = 10; // Distance between left and right ray-traces
    float clampThreshold = 10;

    // Get the Sprite directly under the player
    Sprite *groundSprite = currentGround->getSprite();

    // Get the position at the bottom of the charcter
    // Global Positions
    float myCenterX = sprite->GetSize().x / 2.0f;
    float myCenterY = sprite->GetSize().y / 2.0f;
    float myBottomY = sprite->GetSize().y;
    Vector2f bottomLeft = sprite->TransformToGlobal(Vector2f(myCenterX - lookOffset, myBottomY));
    Vector2f bottomMiddle = sprite->TransformToGlobal(Vector2f(myCenterX, myBottomY + distanceFromGround));
    Vector2f bottomRight = sprite->TransformToGlobal(Vector2f(myCenterX + lookOffset, myBottomY));

    // Transform global positions to local ground positions
    bottomLeft = groundSprite->TransformToLocal(bottomLeft);
    bottomMiddle = groundSprite->TransformToLocal(bottomMiddle);
    bottomRight = groundSprite->TransformToLocal(bottomRight);

    // Get the position of the ray-trace destination
    Vector2f target = sprite->TransformToGlobal(Vector2f(myCenterX, myBottomY+ lookDepth));
    target = groundSprite->TransformToLocal(target);

    // Ray-trace to get position of land below character
    Vector2f *leftCollide = spriteUtil.rayTrace(groundSprite, bottomLeft.x, bottomLeft.y, target.x, target.y);
    Vector2f *middleCollide = spriteUtil.rayTrace(groundSprite, bottomMiddle.x, bottomMiddle.y, target.x, target.y);
    Vector2f *rightCollide = spriteUtil.rayTrace(groundSprite, bottomRight.x, bottomRight.y, target.x, target.y);

    // Both points find ground
    if(leftCollide != 0 && rightCollide != 0)
    {
        inJump = false;
        // Draw the ground line for debugging purposes
        Vector2f globalLeftCollide = groundSprite->TransformToGlobal(*leftCollide);
        Vector2f globalRightCollide = groundSprite->TransformToGlobal(*rightCollide);
        line = Shape::Line(globalLeftCollide, globalLeftCollide, 1, Color::White);

        float groundAngleRad = -coordUtil.getAngle(*leftCollide, 0, *rightCollide);
        float groundAngleDeg = groundAngleRad * (180.0f / M_PI);
        cout << "Distance to ground: " << coordUtil.getDistance(*middleCollide, bottomMiddle) << endl;

        // If close enough to the land, clamp the position to the land
        if(coordUtil.getDistance(*middleCollide, bottomMiddle) < clampThreshold)
        {
            //cout << "Before:\t" << sprite->GetPosition().x << " * " << sprite->GetPosition().y << endl;
            // Points defined in: http://www.clarku.edu/~djoyce/trig/right.html
            Vector2f pointA(globalLeftCollide);

            Vector2f pointC (
                            (globalLeftCollide.x + globalRightCollide.x) / 2.0f,
                            (globalLeftCollide.y + globalRightCollide.y) / 2.0f
                            );

            float distanceAC = coordUtil.getDistance(pointA, pointC);
            float distanceCB = sprite->GetSize().y / 2.0f;
            float angleAB = atan2(distanceCB, distanceAC);
            float distanceAB = hypotf(distanceCB, distanceAC);
            //cout << "angleAB: " << angleAB << endl;
            //cout << "distanceAB: " << distanceAB << endl;
            Vector2f newPosition(
                                 pointA.x + (cos(angleAB) * distanceAB),
                                 pointA.y - (sin(angleAB) * distanceAB)
                                 );
            sprite->SetPosition(newPosition);
        }else{
            // Not close enough to land, fall with gravity
            float gravityAngle = groundAngleRad + M_PI_2;
            float velocityX = cos(gravityAngle) * (speed * elapsedTime);
            float velocityY = sin(gravityAngle) * (speed * elapsedTime);
            sprite->Move(velocityX, velocityY);
        }
    }


    // Perform cleanup
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

Island* Character::getCurrentGround()
{
    return currentGround;
}

Island* Character::getPreviousGround()
{
    return prevGround;
}
