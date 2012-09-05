#include "Character.hpp"

#include "util/NumberUtil.hpp"
#include "game/ID.hpp"

#include<iostream>
#include<math.h>
#include<list>
using namespace std;

#define ANGLE_DIFF_THRESHOLD 0.05f
#define MIN_GROUND_DIST 1
#define MIN_ANGLE_CHANGE_D 6

#define PRINT_V(vector) '(' << vector.x << ", " << vector.y << ')'
#define PRINT_R(pos, size) "(x: " << pos.x << ", y: " << pos.y << ", w: " << size.x << ", h: " << size.y << ')'
#define PRINT_NULL(value) ((value == 0) ? "null" : "not null")
#define PRINT_BOOL(value) ((value) ? "true" : "false")


Character::Character(float x, float y, float width, float height, float speed, Sprite *sprite)
{
    this->speed = speed;

    //sprite->setOrigin(0, sprite->GetSize().y); // Bottom-left
    SpriteUtil::resize(sprite, width, height);
    sprite->setOrigin(0, sprite->getLocalBounds().height); // Bottom-left
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

    // TODO: Implement flipping: http://en.sfml-dev.org/forums/index.php?topic=7604.msg54047#msg54047
    /*if(spriteDirection == Left)
        sprite->FlipX(false);
    else
        sprite->FlipX(true);*/
}

void Character::moveRight()
{
    facingDirection = Right;
    doMoveRight = true;

    /*if(spriteDirection == Left)
        sprite->FlipX(true);
    else
        sprite->FlipX(false);*/
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

    bounds = RectangleShape(Vector2f(sprite->getPosition().y - SpriteUtil::getSize(sprite).y,
                            sprite->getPosition().x + SpriteUtil::getSize(sprite).x));
    bounds.setPosition(sprite->getPosition());
    bounds.setFillColor(Color::White);
    bounds.setOutlineThickness(2);

    if(Keyboard::isKeyPressed(Keyboard::P))
        pause = true;
    if(Keyboard::isKeyPressed(Keyboard::O))
        pause = false;
    if(pause)
        return;

    // Get the elapsed time since the last frame
    float elapsedTime = clock->getElapsedTime().asSeconds();

    // Find the nearest island and assign it as the island the Character is on
    findCurrentIsland(world->getIslands());

    // move in responce to user input
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
                /*cout << "Comparing:" << endl;
                cout << '\t' << PRINT_R(sprite->getPosition(), SpriteUtil::getSize(sprite)) << endl;
                cout << '\t' << PRINT_R(island->getSprite()->getPosition(), SpriteUtil::getSize(island->getSprite())) << endl;*/
                if(coordUtil.collide(sprite, island->getSprite()))
                {
                    // Mark the new ground as the current ground
                    if(currentGround != 0) // Only set the previous ground if a current ground exists
                        prevGround = currentGround;
                    currentGround = island;

                    // Rotate to the new ground
                    //float angleRad = atan2(currentGround->getPosition().y - sprite->getPosition().y,
                    //                    currentGround->getPosition().x - sprite->getPosition().x);
                    float angleRad = coordUtil.getAngle(sprite->getPosition(), 0, currentGround->getPosition());
                    float angleDeg = AS_DEG(angleRad); // Convert the angle from radians to degrees
                    sprite->setRotation(angleDeg - 90); // Rotate to the correct angle

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
    float velocityX = cos(sprite->getRotation() * M_PI / 180) * (speed * elapsedTime);
    float velocityY = sin(sprite->getRotation() * M_PI / 180) * (speed * elapsedTime);

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
        float angle = sprite->getRotation();
        if(inJump)
        {
            angle -= 90; // Up (jump)
        } else {
            angle += 90; // Down (gravity)
        }

        // Get the new movement velocity
        velocityX = cos(angle * M_PI / 180) * (speed * elapsedTime);
        velocityY = sin(angle * M_PI / 180) * (speed * elapsedTime);

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
    Sprite *groundSprite = currentGround->getSprite();
    Image *groundImage = currentGround->getImage();
    Transform groundLocalTransform = groundSprite->getInverseTransform();
    Transform groundGlobalTransform = groundSprite->getTransform();
    Transform spriteGlobalTransform = sprite->getTransform();

    // Variables to configure ground collision
    int lookDepth = 100; // Depth to ray-trace
    int lookOffset = sprite->getLocalBounds().width / 2; // Distance between left and right ray-traces
    float clampThreshold = MIN_GROUND_DIST * 10; // Distance from ground, before character is clamped to the ground

    // Get the position at the bottom of the charcter
    // Global Positions
    float myCenterX = sprite->getLocalBounds().width / 2;
    float myBottomY = sprite->getLocalBounds().height;

    Vector2f localBottomLeft(myCenterX - lookOffset, myBottomY);
    Vector2f localBottomMiddle(myCenterX, myBottomY);
    Vector2f localBottomRight(myCenterX + lookOffset, myBottomY);
    Vector2f globalBottomLeft = spriteGlobalTransform.transformPoint(localBottomLeft);
    Vector2f globalBottomMiddle = spriteGlobalTransform.transformPoint(localBottomMiddle);
    Vector2f globalBottomRight = spriteGlobalTransform.transformPoint(localBottomRight);

    // Transform global positions to local ground positions
    Vector2f groundBottomLeft = groundLocalTransform.transformPoint(globalBottomLeft);
    Vector2f groundBottomMiddle = groundLocalTransform.transformPoint(globalBottomMiddle);
    Vector2f groundBottomRight = groundLocalTransform.transformPoint(globalBottomRight);

    bool aboveGround = isAboveGround(localBottomLeft, *currentGround) &&
                       isAboveGround(localBottomRight, *currentGround);
    //cout << "aboveGround: " << PRINT_BOOL(aboveGround) << endl;

    // Get the position of the ray-trace destination
    Vector2f globalTarget;
    if(aboveGround) // We are above ground
    {
        globalTarget = spriteGlobalTransform.transformPoint(myCenterX, myBottomY + lookDepth);
    }else{ // We are under ground
        globalTarget = spriteGlobalTransform.transformPoint(myCenterX, myBottomY - SpriteUtil::getSize(groundSprite).x);
    }
    Vector2f groundTarget = groundLocalTransform.transformPoint(globalTarget);

    // Ray-trace to get position of land below character
    //TODO: Maybe I need to rescale the output of raytrace back to the ground sprite's scale.
    Vector2f *groundLeftCollide = SpriteUtil::rayTrace(*groundImage, groundBottomLeft.x, groundBottomLeft.y, groundTarget.x, groundTarget.y, aboveGround);
    Vector2f *groundMiddleCollide = SpriteUtil::rayTrace(*groundImage, groundBottomMiddle.x, groundBottomMiddle.y, groundTarget.x, groundTarget.y, aboveGround);
    Vector2f *groundRightCollide = SpriteUtil::rayTrace(*groundImage, groundBottomRight.x, groundBottomRight.y, groundTarget.x, groundTarget.y, aboveGround);

    lookLine = ConvexShape(3);
    lookLine.setPoint(0, globalBottomLeft);
    lookLine.setPoint(1, globalBottomRight);
    lookLine.setPoint(2, globalTarget);
    lookLine.setFillColor(Color::Blue);
    lookLine.setOutlineThickness(1);

    //cout << (groundLeftCollide == 0) << " " << (groundRightCollide == 0) << endl;
    if(groundLeftCollide != 0 && groundRightCollide != 0)
    {
        inJump = false;

        float groundAngleRad = coordUtil.getAngle(*groundLeftCollide, 0, *groundRightCollide);

        // draw debug graphics
        Vector2f globalLeftCollide = groundGlobalTransform.transformPoint(*groundLeftCollide);
        Vector2f globalRightCollide = groundGlobalTransform.transformPoint(*groundRightCollide);
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

bool Character::isAboveGround(Vector2f spritePoint, Island &ground)
{
    bool aboveGround = true;

    int airDistance = MIN_GROUND_DIST; // Maximum underground level before lifting charcater above ground.
    float myAirY = spritePoint.y - airDistance; // Point that should always be above ground.

    Sprite *groundSprite = ground.getSprite();
    Image *groundImage = ground.getImage();
    Transform spriteGlobalTransform = sprite->getTransform();
    Transform groundLocalTransform = groundSprite->getInverseTransform();
    Vector2f globalAirMiddle = spriteGlobalTransform.transformPoint(spritePoint.x, myAirY);
    Vector2f groundAirMiddle = groundLocalTransform.transformPoint(globalAirMiddle);

    // Take the air pixel
    int airPixelAlpha = 0;
    // Make sure the pixel is within the bounds of the ground sprite
    if(coordUtil.isLocalPointInside(groundAirMiddle, *groundSprite))
    {
        airPixelAlpha = (int)(groundImage->getPixel(groundAirMiddle.x, groundAirMiddle.y).a);
    }

    if(airPixelAlpha > 0)
    {
        aboveGround = false;
    }

    return aboveGround;
}

void Character::clampToGround(Vector2f &leftCollide, float groundAngleRad)
{
    float groundAngleDeg = AS_DEG(groundAngleRad);
    sprite->setRotation(groundAngleDeg);

    Vector2f newPos = Vector2f(leftCollide);
    float deltaDist = coordUtil.getDistance(newPos, sprite->getPosition());
    if(deltaDist > MIN_ANGLE_CHANGE_D)
        sprite->setPosition(newPos);
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
    //window->draw(bounds);

    // draw the debug graphics

    //window->draw(lookLine);

    // draw the charcter.
    window->draw(*sprite);

    //window->draw(angleLine);
}

const Vector2f& Character::getPosition()
{
    return sprite->getPosition();
}

const Vector2f& Character::getSize()
{
    return SpriteUtil::getSize(sprite);
}

float Character::getSpeed()
{
    return speed;
}

float Character::getRotation()
{
    return AS_RAD(sprite->getRotation());
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

void Character::setSpriteDirection(Direction direction)
{
    spriteDirection = direction;
}
