#include "Character.hpp"

#include "util/NumberUtil.hpp"
#include "game/ID.hpp"

#include<iostream>
#include<math.h>
#include<list>
using namespace std;

#define ANGLE_DIFF_THRESHOLD 0.05f

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
    prevAngle = 0;

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

    // Find the nearest island and assign it as the island the Character is on
    findCurrentIsland(world->getIslands());

    // Move in responce to user input
    steer(elapsedTime);

    // Stick the Character to the current island.
    lockToIsland(elapsedTime);

    // Allow sub-classes to invoke specific behaviour
    subUpdate(clock, window, world);
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

    // Get the Sprite of the ground the charcter is on
    Sprite *groundSprite = currentGround->getSprite();

    // Variables to configure ground collision
    int lookDepth = groundSprite->GetSize().x/2; // Depth to ray-trace
    int lookOffset = 10; // Distance between left and right ray-traces
    float clampThreshold = 10; // Distance from ground, before character is clamped to the ground

    // Get the position at the bottom of the charcter
    // Global Positions
    float myCenterX = sprite->GetSize().x / 2.0f;
    float myBottomY = sprite->GetSize().y + distanceFromGround;
    Vector2f globalBottomLeft = sprite->TransformToGlobal(Vector2f(myCenterX - lookOffset, myBottomY));
    Vector2f globalBottomMiddle = sprite->TransformToGlobal(Vector2f(myCenterX, myBottomY));
    Vector2f globalBottomRight = sprite->TransformToGlobal(Vector2f(myCenterX + lookOffset, myBottomY));

    // Transform global positions to local ground positions
    Vector2f groundBottomLeft = groundSprite->TransformToLocal(globalBottomLeft);
    Vector2f groundBottomMiddle = groundSprite->TransformToLocal(globalBottomMiddle);
    Vector2f groundBottomRight = groundSprite->TransformToLocal(globalBottomRight);

    bool aboveGround = isAboveGround(*groundSprite);

     // Get the position of the ray-trace destination
    Vector2f globalTarget;
    if(aboveGround) // We are above ground
    {
        globalTarget = sprite->TransformToGlobal(Vector2f(myCenterX, myBottomY + lookDepth));
    }else{ // We are under ground
        globalTarget = sprite->TransformToGlobal(Vector2f(myCenterX, myBottomY - groundSprite->GetSize().x));
    }
    Vector2f groundTarget = groundSprite->TransformToLocal(globalTarget);

    // Ray-trace to get position of land below character
    Vector2f *groundLeftCollide = spriteUtil.rayTrace(groundSprite, groundBottomLeft.x, groundBottomLeft.y, groundTarget.x, groundTarget.y, aboveGround);
    Vector2f *groundMiddleCollide = spriteUtil.rayTrace(groundSprite, groundBottomMiddle.x, groundBottomMiddle.y, groundTarget.x, groundTarget.y, aboveGround);
    Vector2f *groundRightCollide = spriteUtil.rayTrace(groundSprite, groundBottomRight.x, groundBottomRight.y, groundTarget.x, groundTarget.y, aboveGround);

    //if(distanceFromGround > 0)
    //    aboveGround = false; // Pretending to be underground forces clamping

    lookLine = Shape::Line(globalBottomMiddle, globalTarget, 1, Color::Blue);

    if(groundLeftCollide != 0 && groundRightCollide != 0)
    {
        inJump = false;

        // Draw debug graphics
        Vector2f globalLeftCollide = groundSprite->TransformToGlobal(*groundLeftCollide);
        Vector2f globalRightCollide = groundSprite->TransformToGlobal(*groundRightCollide);
        angleLine = Shape::Line(globalLeftCollide, globalRightCollide, 1, Color::White);

        /* If the character is above ground, the character should naturally fall to the ground with gravity.
         * If the charcter is underground, the charcter should instantly snap to the ground.
         */
        if(aboveGround)
        {
            float groundAngleRad = coordUtil.getAngle(*groundLeftCollide, 0, *groundRightCollide);
            float groundAngleDeg = groundAngleRad * (180.0f / M_PI);

            // Only move if the angle of change is significant
            float groundDistance = coordUtil.getDistance(*groundMiddleCollide, groundBottomMiddle);
            bool significantChange = false;
            if(groundAngleRad - prevAngle > ANGLE_DIFF_THRESHOLD  ||
               prevAngle - groundAngleRad > ANGLE_DIFF_THRESHOLD)
            {
                prevAngle = groundAngleRad;
                significantChange = true;
            }

            // If close enough to the land, clamp the position to the land
            if(significantChange && groundDistance < clampThreshold)
            {
                clampToGround(globalLeftCollide, globalRightCollide);
            }else if(groundDistance > clampThreshold){
                // Not close enough to clamp so move with gravity
                float gravityAngle = groundAngleRad + M_PI_2;
                if(!aboveGround) // Reverse gravity if underground
                {
                    gravityAngle = gravityAngle - M_PI_2;
                }

                float velocityX = cos(gravityAngle) * (speed * elapsedTime);
                float velocityY = sin(gravityAngle) * (speed * elapsedTime);
                sprite->Move(velocityX, velocityY);
            }
        }else{
            clampToGround(globalLeftCollide, globalRightCollide);
        }
    }else{
        cout << "Couldn't get a lock on the ground." << endl;
    }

    // Perform cleanup
    delete groundLeftCollide;
    delete groundMiddleCollide;
    delete groundRightCollide;
}

bool Character::isAboveGround(Sprite &groundSprite)
{
    bool aboveGround = true;

    float myCenterX = sprite->GetSize().x / 2.0;
    float myBottomY = sprite->GetSize().y + distanceFromGround;
    int airDistance = (int)(sprite->GetSize().x * 0.1); // Maximum underground level before lifting charcater above ground;
    float myAirY = myBottomY - airDistance;

    // Point that should always be above ground.
    Vector2f globalAirMiddle = sprite->TransformToGlobal(Vector2f(myCenterX, myAirY));
    Vector2f groundAirMiddle = groundSprite.TransformToLocal(globalAirMiddle);

    // Take the air pixel
    int airPixelAlpha = 0;
    // Make sure the pixel is within the bounds of the ground sprite
    if(coordUtil.isLocalPointInside(groundAirMiddle, groundSprite))
    {
        airPixelAlpha = (int)(groundSprite.GetPixel(groundAirMiddle.x, groundAirMiddle.y).a);
    }

    if(airPixelAlpha > 0)
    {
        aboveGround = false;
    }

    return aboveGround;
}

void Character::clampToGround(Vector2f &leftCollide, Vector2f &rightCollide)
{
    float groundAngleRad = coordUtil.getAngle(leftCollide, 0, rightCollide);
    float groundAngleDeg = groundAngleRad * (180.0f / M_PI);

    //cout << "Before:\t" << sprite->GetPosition().x << " * " << sprite->GetPosition().y << endl;
    // Points defined in: http://www.clarku.edu/~djoyce/trig/right.html
    Vector2f pointA(leftCollide);

    Vector2f pointC (
                    (leftCollide.x + rightCollide.x) / 2.0f,
                    (leftCollide.y + rightCollide.y) / 2.0f
                    );

    float distanceCB = (sprite->GetSize().y + distanceFromGround) / 2.0f;

    Vector2f pointB(
                    pointC.x + (cos(groundAngleRad - M_PI_2) * distanceCB),
                    pointC.y + (sin(groundAngleRad - M_PI_2) * distanceCB)
                    );

    sprite->SetPosition(pointB);
    sprite->SetRotation(-groundAngleDeg);
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

    // Draw the debug graphics
    window->Draw(angleLine);
    window->Draw(lookLine);
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
