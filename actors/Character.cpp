#include "Character.hpp"

#include "util/NumberUtil.hpp"
#include "game/ID.hpp"

#include<iostream>
#include<math.h>
#include<list>
using namespace std;

#define ANGLE_DIFF_THRESHOLD 0.05f
#define MIN_GROUND_DIST 2
#define MIN_ANGLE_CHANGE_D 6

#define PRINT_V(vector) '(' << vector.x << ", " << vector.y << ')'
#define PRINT_NULL(value) "value is " << ((value == 0) ? "null." : "not null.")

Character::Character(float x, float y, float width, float height, float speed, Sprite *sprite)
{
    this->speed = speed;

    //sprite->setOrigin(0, sprite->GetSize().y); // Bottom-left
    SpriteUtil::resize(sprite, width, height);
    sprite->setOrigin(0, height); // Bottom-left
    sprite->setPosition(x, y);

    this->sprite = sprite;

    currentGround = 0;
    prevGround = 0;
    inJump = false;
    distanceFromGround = 0;
    prevAngle = 0;
    spriteDirection = Left;

    domoveLeft = false;
    domoveRight = false;
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
    domoveLeft = true;

    // TODO: Implement flipping: http://en.sfml-dev.org/forums/index.php?topic=7604.msg54047#msg54047
    /*if(spriteDirection == Left)
        sprite->FlipX(false);
    else
        sprite->FlipX(true);*/
}

void Character::moveRight()
{
    facingDirection = Right;
    domoveRight = true;

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
    float elapsedTime = clock->getElapsedTime().asSeconds ();

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
                if(coordUtil.collide(sprite, island->getSprite()))
                {
                    // Mark the new ground as the current ground
                    if(currentGround != 0) // Only set the previous ground if a current ground exists
                        prevGround = currentGround;
                    currentGround = island;

                    // Rotate to the new ground
                    float angle = atan2(currentGround->getPosition().y - sprite->getPosition().y,
                                        currentGround->getPosition().x - sprite->getPosition().x);
                    angle = angle * (180.0f/M_PI); // Convert the angle from radians to degrees
                    sprite->setRotation(-angle + 90); // Rotate to the correct angle

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
    float velocityX = cos(sprite->getRotation() * M_PI / 180) * (speed * elapsedTime);
    float velocityY = sin(sprite->getRotation() * M_PI / 180) * (speed * elapsedTime);

    // Respond to user input events
    if(domoveRight) // move right
    {
        sprite->move(velocityX, -velocityY);
        domoveRight = false;
    }

    if(domoveLeft) // move left
    {
        sprite->move(-velocityX, velocityY);
        domoveLeft = false;
    }

    // If no current island exists, keep jumping
    if(currentGround == 0)
    {
        float angle = -sprite->getRotation();
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
    Image groundImage = currentGround->getImage();
    Transform groundTransform = groundSprite->getTransform();
    //Transform spriteTransform = sprite->getTransform();
    Transform groundInvTransform = groundSprite->getInverseTransform();
    Transform spriteInvTransform = sprite->getInverseTransform();

    // Variables to configure ground collision
    int lookDepth = SpriteUtil::getSize(sprite).y; // Depth to ray-trace
    int lookOffset = SpriteUtil::getSize(sprite).x / 2; // Distance between left and right ray-traces
    float clampThreshold = MIN_GROUND_DIST * 10; // Distance from ground, before character is clamped to the ground

    // Get the position at the bottom of the charcter
    // Global Positions
    float myCenterX = SpriteUtil::getSize(sprite).x / 2;
    float myBottomY = SpriteUtil::getSize(sprite).y / 2 + distanceFromGround;
    // We scale down myCenterX/Y because TransformToGlobal will scale it up again
    Vector2f globalBottomLeft = spriteInvTransform.transformPoint(myCenterX - lookOffset, myBottomY);
    Vector2f globalBottomMiddle = spriteInvTransform.transformPoint(myCenterX, myBottomY);
    Vector2f globalBottomRight = spriteInvTransform.transformPoint(myCenterX + lookOffset, myBottomY);

    // Transform global positions to local ground positions
    Vector2f groundBottomLeft = groundTransform.transformPoint(globalBottomLeft);
    Vector2f groundBottomMiddle = groundTransform.transformPoint(globalBottomMiddle);
    Vector2f groundBottomRight = groundTransform.transformPoint(globalBottomRight);

    bool aboveGround = isAboveGround(*groundSprite);

     // Get the position of the ray-trace destination
    Vector2f globalTarget;
    if(aboveGround) // We are above ground
    {
        globalTarget = spriteInvTransform.transformPoint(myCenterX, myBottomY + lookDepth);
    }else{ // We are under ground
        globalTarget = spriteInvTransform.transformPoint(myCenterX, myBottomY - SpriteUtil::getSize(groundSprite).x / 2);
    }
    Vector2f groundTarget = groundTransform.transformPoint(globalTarget);

    // Ray-trace to get position of land below character
    Vector2f *groundLeftCollide = spriteUtil.rayTrace(groundImage, groundBottomLeft.x, groundBottomLeft.y, groundTarget.x, groundTarget.y, aboveGround);
    Vector2f *groundMiddleCollide = spriteUtil.rayTrace(groundImage, groundBottomMiddle.x, groundBottomMiddle.y, groundTarget.x, groundTarget.y, aboveGround);
    Vector2f *groundRightCollide = spriteUtil.rayTrace(groundImage, groundBottomRight.x, groundBottomRight.y, groundTarget.x, groundTarget.y, aboveGround);

    lookLine = ConvexShape(2);
    lookLine.setPoint(0, globalBottomMiddle);
    lookLine.setPoint(1, globalTarget);
    lookLine.setFillColor(Color::Blue);
    lookLine.setOutlineThickness(1);

    if(groundLeftCollide != 0 && groundRightCollide != 0)
    {
        inJump = false;

        float groundAngleRad = coordUtil.getAngle(*groundLeftCollide, 0, *groundRightCollide);

        // draw debug graphics
        Vector2f globalLeftCollide = groundInvTransform.transformPoint(*groundLeftCollide);
        Vector2f globalRightCollide = groundInvTransform.transformPoint(*groundRightCollide);
        lookLine = ConvexShape(2);
        lookLine.setPoint(0, globalLeftCollide);
        lookLine.setPoint(1, globalRightCollide);
        lookLine.setFillColor(Color::Black);
        lookLine.setOutlineThickness(2);

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
            }else{//} if(groundDistance > 20){
                // Not close enough to clamp so move with gravity
                float gravityAngle = groundAngleRad + M_PI_2;
                if(!aboveGround) // Reverse gravity if underground
                {
                    gravityAngle = gravityAngle - M_PI_2;
                }

                float velocityX = cos(gravityAngle) * (speed * elapsedTime);
                float velocityY = sin(gravityAngle) * (speed * elapsedTime);
                sprite->move(velocityX, velocityY);

                float groundAngleDeg = AS_DEG(groundAngleRad);
                sprite->setRotation(-groundAngleDeg);
            }
        }else{
            clampToGround(globalLeftCollide, groundAngleRad);
        }
    }else{
        // Continue moving in the current direction
        float rotationRad = getRotation() - M_PI_2;
        float velocityX = cos(rotationRad) * (speed * elapsedTime);
        float velocityY = sin(rotationRad) * (speed * elapsedTime);
        sprite->move(velocityX, velocityY);
    }

    // Perform cleanup
    delete groundLeftCollide;
    delete groundMiddleCollide;
    delete groundRightCollide;
}

bool Character::isAboveGround(Sprite &groundSprite)
{
    bool aboveGround = true;

    float myCenterX = SpriteUtil::getSize(sprite).x / 2;
    float myBottomY = SpriteUtil::getSize(sprite).y / 2 + distanceFromGround;
    int airDistance = MIN_GROUND_DIST; // Maximum underground level before lifting charcater above ground;
    float myAirY = myBottomY - airDistance;

    // Point that should always be above ground.
    Transform spriteInvTransform = sprite->getInverseTransform();
    Transform groundTransform = groundSprite.getTransform();
    Vector2f globalAirMiddle = spriteInvTransform.transformPoint(myCenterX, myAirY);
    Vector2f groundAirMiddle = groundTransform.transformPoint(globalAirMiddle);

    // Take the air pixel
    int airPixelAlpha = 0;
    // Make sure the pixel is within the bounds of the ground sprite
    // TODO: Re-add
    /*if(coordUtil.isLocalPointInside(groundAirMiddle, groundSprite))
    {
        airPixelAlpha = (int)(groundSprite.GetPixel(groundAirMiddle.x, groundAirMiddle.y).a);
    }*/

    if(airPixelAlpha > 0)
    {
        aboveGround = false;
    }

    return aboveGround;
}

void Character::clampToGround(Vector2f &leftCollide, float groundAngleRad)
{
    /*float groundAngleRad = coordUtil.getAngle(leftCollide, 0, rightCollide);
    float groundAngleDeg = groundAngleRad * (180.0f / M_PI);

    //cout << "Before:\t" << sprite->getPosition().x << " * " << sprite->getPosition().y << endl;
    // Points defined in: http://www.clarku.edu/~djoyce/trig/right.html
    Vector2f pointA(leftCollide);

    Vector2f pointC (
                    (leftCollide.x + rightCollide.x) / 2.0f,
                    (leftCollide.y + rightCollide.y) / 2.0f
                    );

    float distanceCB = MIN_GROUND_DIST;//(sprite->GetSize().y + distanceFromGround) / 2.0f;

    Vector2f pointB(
                    pointC.x + (cos(groundAngleRad - M_PI_2) * distanceCB),
                    pointC.y + (sin(groundAngleRad - M_PI_2) * distanceCB)
                    );*/


    float groundAngleDeg = AS_DEG(groundAngleRad);
    sprite->setRotation(-groundAngleDeg);

    Vector2f newPos = Vector2f(leftCollide);
    float deltaAngle = coordUtil.getDistance(newPos, sprite->getPosition());
    if(deltaAngle > MIN_ANGLE_CHANGE_D)
        sprite->setPosition(newPos);

    /*if(groundAngleDeg != 0)
    {
        pause = true;
        lookLine = Shape::Line(sprite->getPosition(), leftCollide, 2, Color::Blue);
        cout << sprite->getPosition().x << ", " << sprite->getPosition().y << '\n' << leftCollide.x << ", " << leftCollide.y << endl;
        return;
    }*/


    //Vector2f newPos = Vector2f(pointB.x - sprite->GetSize().x/2, pointB.y - sprite->GetSize().y/2);
    //sprite->setPosition(newPos);

    //cout << -groundAngleDeg << endl;
    //cout << "\t lc: " << leftCollide.x << ", " << leftCollide.y << " rc: " << rightCollide.x << ", " << rightCollide.y << endl;
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
    //window->draw(bounds);

    // draw the charcter.
    window->draw(*sprite);

    // draw the debug graphics
    window->draw(angleLine);
    window->draw(lookLine);
}

const Vector2f& Character::getPosition()
{
    return sprite->getPosition();
}

const Vector2f& Character::getSize()
{
    return SpriteUtil::getSize(sprite);
}

float Character::getRotation()
{
    /*float rotation = sprite->getRotation();
    rotation *= M_PI / 180.0f;
    return rotation;*/

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
