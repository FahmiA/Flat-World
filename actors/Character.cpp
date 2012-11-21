#include "Character.hpp"

#include "util/NumberUtil.hpp"
#include "game/ID.hpp"
#include "util/Debug.hpp"

#include<iostream>
#include<math.h>
#include<list>
using namespace std;

#define ANGLE_DIFF_THRESHOLD 0.05f
#define MIN_GROUND_DIST 1
#define MIN_ANGLE_CHANGE_D 6

Character::Character(float x, float y, float width, float height, float speed, TSprite *sprite)
{
    this->speed = speed;

    sprite->setSize(width, height);
    sprite->setOrigin(0.0f, 1.0f); // Bottom-left
    sprite->setPosition(x, y);

    this->sprite = sprite;

    currentGround = 0;
    prevGround = 0;
    inJump = false;
    distanceFromGround = 0;
    prevAngle = 0;
    spriteDirection = Left;

    doMoveLeft = false;
    doMoveRight = false;
    facingDirection = Left;

    pause = false;
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
    sprite->lookLeft();
}

void Character::moveRight()
{
    facingDirection = Right;
    doMoveRight = true;
    sprite->lookRight();
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
    Vector2f boundsSize(sprite->getSize());
    boundsSize.y = -boundsSize.y;
    bounds = RectangleShape(boundsSize);
    bounds.setPosition(sprite->getPosition());
    bounds.setFillColor(Color::White);
    bounds.setOutlineThickness(2);

    if(Keyboard::isKeyPressed(Keyboard::P))
        pause = true;
    if(Keyboard::isKeyPressed(Keyboard::O))
        pause = false;

    if(Keyboard::isKeyPressed(Keyboard::Z))
    {
        sprite->setRotation(sprite->getRotation() + 0.1f);
        cout << sprite->getRotation() << endl;
    }

    if(Keyboard::isKeyPressed(Keyboard::X))
    {
        sprite->setRotation(sprite->getRotation() - 0.1f);
        cout << sprite->getRotation() << endl;
    }

    if(pause)
        return;

    // Get the elapsed time since the last frame
    float elapsedTime = clock->getElapsedTime().asSeconds();

    // Find the nearest island and assign it as the island the Character is on
    findCurrentIsland(world->getIslands());

    if(pause)
        return;

    // Move in responce to user input
    steer(elapsedTime);

    // Stick the Character to the current island.
    lockToIsland(elapsedTime);

    // Allow sub-classes to invoke specific behaviour
    subUpdate(clock, window, world);

    // Update sprite animation
    sprite->update(clock);

    //pause = true;
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
                    Vector2f spritePos = sprite->getPosition(); // Must copy
                    Vector2f groundPos = currentGround->getPosition(); // Must copy
                    float angleRad = coordUtil.getAngle(spritePos, 0, groundPos);
                    sprite->setRotation(angleRad - M_PI_2); // Rotate to the correct angle

                    cout << "found new ground" << endl;
                    //pause = true;

                    break;
                }
            }
        }
    }
}

void Character::steer(float elapsedTime)
{
    // Get the x and y movement velocity
    float velocityX = cos(sprite->getRotation()) * speed * elapsedTime;
    float velocityY = sin(sprite->getRotation()) * speed * elapsedTime;

    // Respond to user input events
    if(doMoveRight) // move right
    {
        sprite->move(velocityX, velocityY);
        doMoveRight = false;
    }

    if(doMoveLeft) // move left
    {
        sprite->move(-velocityX, -velocityY);
        doMoveLeft = false;
    }

    // If no current island exists, keep jumping
    if(currentGround == 0)
    {
        float angleR = sprite->getRotation();
        if(inJump)
        {
            angleR -= M_PI_2; // Up (jump)
        } else {
            angleR += M_PI_2; // Down (gravity)
        }

        // Get the new movement velocity
        velocityX = cos(angleR) * (speed * elapsedTime);
        velocityY = sin(angleR) * (speed * elapsedTime);

        // move the character
        sprite->move(velocityX, velocityY);
    }
}

void Character::lockToIsland(float elapsedTime)
{
    // Can't lock to island if no island currently exists (eg: in jump)
    if(currentGround == 0)
        return;

    // Get the Sprite of the ground the charcter is on
    TSprite &groundSprite = *currentGround->getSprite();
    TSprite &mySprite = *sprite;

    // Variables to configure ground collision
    int lookDepth = 100; // Depth to ray-trace
    int lookOffset = mySprite.getSize().x / 2; // Distance between left and right ray-traces
    lookOffset = abs(lookOffset);
    float clampThreshold = MIN_GROUND_DIST * 10; // Distance from ground, before character is clamped to the ground

    // Get the position at the bottom of the charcter
    // Global Positions
    float myCenterX = mySprite.getSize().x / 2;
    if(myCenterX < 0)
        myCenterX = -myCenterX;
    //if(myCenterX < 0)
    //    myCenterX = -myCenterX;
    float myBottomY = mySprite.getSize().y;

    Vector2f localBottomLeft(myCenterX - lookOffset, myBottomY);
    Vector2f localBottomMiddle(myCenterX, myBottomY);
    Vector2f localBottomRight(myCenterX + lookOffset, myBottomY);
    Vector2f globalBottomLeft = mySprite.toGlobal(localBottomLeft);
    Vector2f globalBottomMiddle = mySprite.toGlobal(localBottomMiddle);
    Vector2f globalBottomRight = mySprite.toGlobal(localBottomRight);

    // Transform global positions to local ground positions
    Vector2f groundBottomLeft = groundSprite.toLocal(globalBottomLeft);
    Vector2f groundBottomMiddle = groundSprite.toLocal(globalBottomMiddle);
    Vector2f groundBottomRight = groundSprite.toLocal(globalBottomRight);

    bool aboveGround = isAboveGround(localBottomLeft, groundSprite) &&
                       isAboveGround(localBottomRight, groundSprite);
    //cout << "aboveGround: " << PRINT_BOOL(aboveGround) << endl;

    // Get the position of the ray-trace destination
    Vector2f globalTarget;
    if(aboveGround) // We are above ground
    {
        globalTarget = mySprite.toGlobal(Vector2f(myCenterX, myBottomY + lookDepth));
    }else{ // We are under ground
        globalTarget = mySprite.toGlobal(Vector2f(myCenterX, myBottomY - groundSprite.getSize().x));
    }
    Vector2f groundTarget = groundSprite.toLocal(globalTarget);

    // Ray-trace to get position of land below character
    //TODO: Maybe I need to rescale the output of raytrace back to the ground sprite's scale.
    Vector2f *groundLeftCollide = SpriteUtil::rayTrace(groundSprite, groundBottomLeft.x, groundBottomLeft.y, groundTarget.x, groundTarget.y, aboveGround);
    Vector2f *groundMiddleCollide = SpriteUtil::rayTrace(groundSprite, groundBottomMiddle.x, groundBottomMiddle.y, groundTarget.x, groundTarget.y, aboveGround);
    Vector2f *groundRightCollide = SpriteUtil::rayTrace(groundSprite, groundBottomRight.x, groundBottomRight.y, groundTarget.x, groundTarget.y, aboveGround);

    lookLine = ConvexShape(3);
    lookLine.setPoint(0, globalBottomLeft);
    lookLine.setPoint(1, globalBottomRight);
    lookLine.setPoint(2, globalTarget);
    lookLine.setFillColor(Color::Blue);
    lookLine.setOutlineThickness(1);

    //cout << PRINT_BOOL(groundLeftCollide != 0) << ", " << PRINT_BOOL(groundRightCollide != 0) <<  endl;
    if(groundLeftCollide != 0 && groundRightCollide != 0)
    {
        //cout << PRINT_V(*groundLeftCollide) << " - " << PRINT_V(*groundRightCollide) << endl;
        inJump = false;

        float groundAngleRad = coordUtil.getAngle(*groundLeftCollide, 0, *groundRightCollide);

        // draw debug graphics
        Vector2f globalLeftCollide = groundSprite.toGlobal(*groundLeftCollide);
        Vector2f globalRightCollide = groundSprite.toGlobal(*groundRightCollide);
        angleLine = ConvexShape(4);
        angleLine.setPoint(0, globalBottomLeft);
        angleLine.setPoint(1, globalBottomRight);
        angleLine.setPoint(2, globalRightCollide);
        angleLine.setPoint(3, globalLeftCollide);
        angleLine.setFillColor(Color::Green);
        angleLine.setOutlineThickness(1);

        /* If the character is above ground, the character should naturally fall to the ground with gravity.
         * If the charcter is underground, the charcter should instantly snap to the ground.
         */
        if(aboveGround)
        {
            // Only move if the angle of change is significant
            float groundDistance = coordUtil.getDistance(*groundMiddleCollide, groundBottomMiddle);
            //cout << groundDistance << " < " << clampThreshold << endl;

            // If close enough to the land, clamp the position to the land
            if(groundDistance < clampThreshold)
            {
                clampToGround(globalLeftCollide, groundAngleRad);
            }else{
                // Not close enough to clamp so move with gravity
                float gravityAngle = groundAngleRad + M_PI_2;
                if(!aboveGround) // Reverse gravity if underground
                {
                    gravityAngle = gravityAngle - M_PI_2;
                }

                float velocityX = cos(gravityAngle) * (speed * elapsedTime);
                float velocityY = sin(gravityAngle) * (speed * elapsedTime);
                sprite->move(velocityX, velocityY);

                //float groundAngleDeg = AS_DEG(groundAngleRad);
                //sprite->setRotation(groundAngleDeg);
            }
        }else{
            clampToGround(globalLeftCollide, groundAngleRad);
        }
    }else{
        // Continue moving in the current direction
        float rotationRad = getRotation() + M_PI_2;
        float velocityX = cos(rotationRad) * (speed * elapsedTime);
        float velocityY = sin(rotationRad) * (speed * elapsedTime);
        sprite->move(velocityX, velocityY);
    }

    // Perform cleanup
    delete groundLeftCollide;
    delete groundMiddleCollide;
    delete groundRightCollide;
}

bool Character::isAboveGround(Vector2f spritePoint, TSprite &groundSprite)
{
    bool aboveGround = true;

    int airDistance = MIN_GROUND_DIST; // Maximum underground level before lifting charcater above ground.
    float myAirY = spritePoint.y - airDistance; // Point that should always be above ground.

    Vector2f globalAirMiddle = sprite->toGlobal(Vector2f(spritePoint.x, myAirY));
    Vector2f groundAirMiddle = groundSprite.toLocal(globalAirMiddle);
    groundAirMiddle.x = (int)groundAirMiddle.x;
    groundAirMiddle.y = (int)groundAirMiddle.y;

    // Take the air pixel
    int airPixelAlpha = 0;
    // Make sure the pixel is within the bounds of the ground sprite
    if(coordUtil.isLocalPointInside(groundAirMiddle, groundSprite.getSize()))
    {
        //cout << PRINT_V(groundSprite.getSize()) << ", " << PRINT_V(groundAirMiddle) << endl;
        airPixelAlpha = (int)(groundSprite.getPixel(groundAirMiddle.x, groundAirMiddle.y).a);
    }

    if(airPixelAlpha > 0)
    {
        aboveGround = false;
    }

    //cout << PRINT_BOOL(aboveGround) << endl;
    return aboveGround;
}

void Character::clampToGround(Vector2f &leftCollide, float groundAngleRad)
{
    sprite->setRotation(groundAngleRad);

    Vector2f newPos = Vector2f(leftCollide);
    float deltaDist = coordUtil.getDistance(newPos, sprite->getPosition());
    //cout << deltaDist << " > " << MIN_ANGLE_CHANGE_D << endl;
    if(deltaDist > MIN_ANGLE_CHANGE_D)
    {
        sprite->setPosition(newPos.x, newPos.y);
    }
}

CoordinateUtil& Character::getCoordinateUtil()
{
    return coordUtil;
}

void Character::setDistanceFromGround(float distance)
{
    distanceFromGround = distance;
}

void Character::draw(RenderWindow *window)
{
    window->draw(bounds);

    // draw the debug graphics

    window->draw(lookLine);

    // draw the charcter.
    sprite->draw(window);

    window->draw(angleLine);
}

const Vector2f& Character::getPosition()
{
    return sprite->getPosition();
}

void Character::setPosition(float x, float y)
{
    return sprite->setPosition(x, y);
}

const Vector2f& Character::getSize()
{
    return sprite->getSize();
}

float Character::getSpeed()
{
    return speed;
}

float Character::getRotation()
{
    return sprite->getRotation();
}

TSprite* Character::getSprite()
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

void Character::setSpriteDirection(Direction direction)
{
    spriteDirection = direction;
}
