#include "Player.hpp"

#include "../game/World.hpp"
#include "../environment/Island.hpp"
#include "../util/NumberUtil.hpp"
#include "../util/CoordinateUtil.hpp"

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
    if(window->GetInput().IsKeyDown(Key::Right))    // Move right
        sprite->Move(velocityX, -velocityY);

    if(window->GetInput().IsKeyDown(Key::Left))     // Move left
        sprite->Move(-velocityX, velocityY);

    if(window->GetInput().IsKeyDown(Key::R))        // Rotate (DEEBUG FEATURE)
        sprite->Rotate(100 * elapsedTime);

    // Perform collision detection
    list<GameObject*>* objects = world->getObjects();
    Island *ground = (Island*)(objects->front());

    // Get the pixels directly under the player
    Sprite *groundSprite = ground->getSprite();

    int lookDepth = 50; // Depth to ray-trace
    int lookOffset = 50; // Distance between left and right ray-traces

    // Get the position at the bottom of the charcter
    // Global Positions
    Vector2f bottomLeft = sprite->TransformToGlobal(Vector2f((sprite->GetSize().x/2) - lookOffset, sprite->GetSize().y));
    Vector2f bottomMiddle = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y));
    Vector2f bottomRight = sprite->TransformToGlobal(Vector2f((sprite->GetSize().x/2) + lookOffset, sprite->GetSize().y));
    // Local positions to ground
    bottomLeft = groundSprite->TransformToLocal(bottomLeft);
    bottomMiddle = groundSprite->TransformToLocal(bottomMiddle);
    bottomRight = groundSprite->TransformToLocal(bottomRight);

    // Get the position at the ray-trace distance
    // Global Positions
    Vector2f targetLeft = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    Vector2f targetMiddle = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    Vector2f targetRight = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    // Local positions to ground
    targetLeft = groundSprite->TransformToLocal(targetLeft);
    targetMiddle = groundSprite->TransformToLocal(targetMiddle);
    targetRight = groundSprite->TransformToLocal(targetRight);

    // TODO: Move the bottom positions to be within the image bounds
    //clampCoordinates(bottomLeft, targetLeft, groundSprite)
    //clampCoordinates(bottomMiddle, targetMiddle, groundSprite)
    //clampCoordinates(bottomRight, targetRight, groundSprite)

    int groundTop = groundSprite->GetPosition().y - (groundSprite->GetSize().y/2);
    int groundLeft = groundSprite->GetPosition().x - (groundSprite->GetSize().x/2);

    // If the target iw within the image bounds
    //if(bottomLeft.y >= 0 && (int)(bottomRight.y) >= 0 && bottomLeft.x >= 0 && bottomRight.x >= 0
    //   && bottomLeft.y < groundSprite->GetSize().y && bottomRight.y < groundSprite->GetSize().y
    //   && bottomLeft.x < groundSprite->GetSize().x && bottomRight.x < groundSprite->GetSize().x) // Inside island region

    {
        // Land directly below
        Vector2f *leftCollide = rayTrace(groundSprite, bottomLeft.x, bottomLeft.y, targetLeft.x, targetLeft.y);
        Vector2f *middleCollide = rayTrace(groundSprite, bottomMiddle.x, bottomMiddle.y, targetMiddle.x, targetMiddle.y);
        Vector2f *rightCollide = rayTrace(groundSprite, bottomRight.x, bottomRight.y, targetRight.x, targetRight.y);

        // Resolve the problem of only one ray-trace finding land
        if(rightCollide == 0 && leftCollide != 0)
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
        }

        if(leftCollide == 0 && rightCollide == 0){
            // Move with relative gravity
            float angle = sprite->GetRotation() + 90;
            velocityX = cos(angle * M_PI / 180) * (speed * elapsedTime);
            velocityY = sin(angle * M_PI / 180) * (speed * elapsedTime);
            sprite->Move(velocityX, velocityY);
            //cout << "here: " << elapsedTime << endl;
        }else{
            line = Shape::Line(leftCollide->x + groundLeft, leftCollide->y + groundTop, rightCollide->x + groundLeft, rightCollide->y + groundTop, 1, Color::White);
            // Rotate to the correct angle and move up (against gravity)
            //cout << rightCollide->y << " " << leftCollide->y << " " << rightCollide->x << " " << leftCollide->x << endl;
            float angle = atan2(rightCollide->y - leftCollide->y, rightCollide->x - leftCollide->x);
            angle = angle * (180.0f/M_PI); // Convert the angle from radians to degrees
            //cout << "Angle: " << angle << endl;

            sprite->SetRotation(-angle); // Rotate to the correct angle

            if(middleCollide != 0)
            {
                float dx = bottomMiddle.x - middleCollide->x;
                float dy = bottomMiddle.y - middleCollide->y;
                float distance = (dx * dx) + (dy * dy);
                distance = sqrt(distance);

                //cout << distance << endl;
                if(distance > 2 || distance < 1)
                {
                    CoordinateUtil coordUtil;
                    Color pixel;

                    if(coordUtil.isLocalPointInside(bottomMiddle, *groundSprite))
                        pixel = groundSprite->GetPixel(bottomMiddle.x, bottomMiddle.y);
                    else
                        pixel = Color(0, 0, 0, 0);

                    float reverseAngle = angle;
                    //cout << (int)(pixel.a) << endl;
                    if(pixel.a == 0) // Transparent
                    {
                        // Move down
                        reverseAngle += 90;
                    }else{ // Not transparent
                        // Move up
                        reverseAngle -= 90;
                    }
                    //cout << angle << " " << reverseAngle << endl;
                    velocityX = cos((reverseAngle * M_PI) / 180) * (speed * elapsedTime);
                    velocityY = sin((reverseAngle * M_PI) / 180) * (speed * elapsedTime);
                    sprite->Move(velocityX, velocityY);
                }
            }
        }

        delete leftCollide;
        delete middleCollide;
        delete rightCollide;
    }

    //cout << sprite->GetRotation() << endl;
}

// Clamps the origin
/*void Player::clampCoordinates(Vector2f &origin, Vector2f &target, Sprite *bounds)
{
    // TODO: If the origin and target is within bounds to nothing.
    float lookAngle = atan2(target.y - origin.y, target.x - origin.x);
    float clampedX = -1;
    float clampedY = -1;


}*/

Vector2f* Player::rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY)
{
    //printf("fromX: %d\tfromY: %d\ttoX: %d\ttoY: %d\n", fromX, fromY, toX, toY);
    // Simplified Bresenham line algorithm
    // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    // x0 = fromX   y0 = fromY  x1 = toX    y1 = toY

    /*CoordinateUtil coordUtil;
    Vector2f fromPosition(fromX, fromY);
    Vector2f toPosition(toX, toY);
    bool fromInside = coordUtil.isLocalPointInside(fromPosition, *sprite);
    bool toInside = coordUtil.isLocalPointInside(toPosition, *sprite);

    if(!fromInside && !toInside)
    {
        return 0;
    }else if(!fromInside){
        clamp(fromX, 0, sprite->GetSize().x - 1);
        clamp(fromY, 0, sprite->GetSize().y - 1);
    }else if(!toInside){
        clamp(toX, 0, sprite->GetSize().x - 1);
        clamp(toY, 0, sprite->GetSize().y - 1);
    }*/

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

void Player::draw(RenderWindow *window)
{
    window->Draw(*sprite);

    window->Draw(line);
}
