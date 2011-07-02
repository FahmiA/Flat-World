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

    int lookDepth = 50;
    int lookOffset = 14;
    Vector2f bottomLeft = sprite->TransformToGlobal(Vector2f((sprite->GetSize().x/2) - lookOffset, sprite->GetSize().y));
    Vector2f bottomMiddle = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y));
    Vector2f bottomRight = sprite->TransformToGlobal(Vector2f((sprite->GetSize().x/2) + lookOffset, sprite->GetSize().y));
    bottomLeft = groundSprite->TransformToLocal(bottomLeft);
    bottomMiddle = groundSprite->TransformToLocal(bottomMiddle);
    bottomRight = groundSprite->TransformToLocal(bottomRight);

    Vector2f targetLeft = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    Vector2f targetMiddle = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    Vector2f targetRight = sprite->TransformToGlobal(Vector2f(sprite->GetSize().x/2, sprite->GetSize().y + lookDepth));
    targetLeft = groundSprite->TransformToLocal(targetLeft);
    targetMiddle = groundSprite->TransformToLocal(targetMiddle);
    targetRight = groundSprite->TransformToLocal(targetRight);

    //cout << "lx: " << targetLeft.x <<"\tlt: " << targetLeft.y << "\trx: " << targetRight.x << "\try: " << targetRight.y << endl;
    int groundTop = groundSprite->GetPosition().y - (groundSprite->GetSize().y/2);
    int groundLeft = groundSprite->GetPosition().x - (groundSprite->GetSize().x/2);
    //int groundBottom = groundSprite->GetPosition().y + (groundSprite->GetSize().x/2);
    //int groundRight = groundSprite->GetPosition().x + (groundSprite->GetSize().x/2);
    //line = Shape::Line(bottomLeft.x, bottomLeft.y + groundTop, bottomRight.x, bottomRight.y + groundTop, 1, Color::White);

    if(bottomLeft.y >= 0 && (int)(bottomRight.y) >= 0 && bottomLeft.x >= 0 && bottomRight.x >= 0
       && bottomLeft.y < groundSprite->GetSize().y && bottomRight.y < groundSprite->GetSize().y
       && bottomLeft.x < groundSprite->GetSize().x && bottomRight.x < groundSprite->GetSize().x) // Inside island region
    {
        //cout << "In Island: " << elapsedTime << endl;
        // Land directly below
        Vector2f *leftCollide = rayTrace(groundSprite, bottomLeft.x, bottomLeft.y, targetLeft.x, targetLeft.y);
        Vector2f *middleCollide = rayTrace(groundSprite, bottomMiddle.x, bottomMiddle.y, targetMiddle.x, targetMiddle.y);
        Vector2f *rightCollide = rayTrace(groundSprite, bottomRight.x, bottomRight.y, targetRight.x, targetRight.y);

        if(rightCollide == 0 && leftCollide != 0)
        {
            /*float dx = bottomLeft.x - leftCollide->x;
            float dy = bottomLeft.y - leftCollide->y;
            float distance = (dx * dx) + (dy * dy);
            distance = sqrt(distance);
            int x = bottomRight.x;
            int y = leftCollide->y + (2 * distance);
            rightCollide = new Vector2f(x, y);

            cout << "New Position: x = " << rightCollide->x << "  y = " << rightCollide->y << endl;*/
            rightCollide = new Vector2f(bottomRight.x, bottomRight.y);
            cout << "Right Collide missing" << endl;
        }else if(rightCollide != 0 && leftCollide == 0){
            leftCollide = new Vector2f(bottomLeft.x, bottomLeft.y);
            cout << "Left Collide missing" << endl;
        }

        if(leftCollide == 0 && rightCollide == 0){
            //cout << "\tleftCollide: " << ((leftCollide == 0) ? "False" : "True") << " rightCollide: " << ((rightCollide == 0) ? "False" : "True") << endl;
            float angle = sprite->GetRotation() - 90;
            velocityX = cos(angle * M_PI / 180) * (speed * elapsedTime);
            velocityY = sin(angle * M_PI / 180) * (speed * elapsedTime);
            sprite->Move(velocityX, velocityY);
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
                    Color pixel = groundSprite->GetPixel(bottomMiddle.x, bottomMiddle.y);
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
    }else{
        // No land below
        // Move down (with gravity)
        float angle = sprite->GetRotation() + 90;
        velocityX = cos(angle * M_PI / 180) * (speed * elapsedTime);
        velocityY = sin(angle * M_PI / 180) * (speed * elapsedTime);
        sprite->Move(velocityX, velocityY);

        cout << "No Land below 2: blx = " << bottomLeft.x << " bly = " << bottomLeft.y << " brx = " << bottomRight.x << " bry = " << bottomRight.y << endl;
    }

    //cout << sprite->GetRotation() << endl;
}

Vector2f* Player::rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY)
{
    //printf("fromX: %d\tfromY: %d\ttoX: %d\ttoY: %d\n", fromX, fromY, toX, toY);
    // Simplified Bresenham line algorithm
    // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    // x0 = fromX   y0 = fromY  x1 = toX    y1 = toY

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
