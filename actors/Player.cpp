#include "Player.hpp"

#include "game/ID.hpp"
#include "util/CoordinateUtil.hpp"
#include "GUI/HUD.hpp"

#include <iostream>
using namespace std;

Player::Player(float x, float y, float width, float height, float speed, Sprite *sprite)
    : Character(x, y, width, height, speed, sprite),
    PUSH_BACK_DURATION_SECS(1), PUSH_BACK_MAX_HEIGHT(40)
{
    setID(ID_PLAYER);

    pushedBackDirection = None;
    pushBackTimeSecs = 0;
}

Player::~Player() {}

void Player::subUpdate(Clock *clock, RenderWindow *window, World *world)
{
    // DEBUG: Invoke or cancel an attack from a sheepdog
    if(window->GetInput().IsKeyDown(Key::A))
    {
        pushBack(Left);
    }
    // DEBUG: Invoke or cancel an attack from a sheepdog
    if(window->GetInput().IsKeyDown(Key::S))
    {
        pushBack(Right);
    }

    if(pushedBackDirection != None)
    {
        doActionPushBack(clock->GetElapsedTime());
    }else{
        doActionNormal(window);
    }

    check_unit_collide(world);
}

void Player::pushBack(Direction pushDirection)
{
    pushedBackDirection = pushDirection;
}

void Player::doActionNormal(RenderWindow *window)
{
    const Input &input = window->GetInput();
    if(input.IsKeyDown(Key::Left))
        moveLeft();

    if(input.IsKeyDown(Key::Right))
        moveRight();

    if(input.IsKeyDown(Key::Space))
        landHop();
}

void Player::doActionPushBack(float elapsedTime)
{
    // Update the time that has been spent so far being pushed back
    pushBackTimeSecs += elapsedTime;

    // Check if the push back action is completed
    if(pushBackTimeSecs > PUSH_BACK_DURATION_SECS)
    {
        pushedBackDirection = None;
        pushBackTimeSecs = 0;
        setDistanceFromGround(0);
    }else{
        // Still being pushed back
        float groundDistance = PUSH_BACK_MAX_HEIGHT * (pushBackTimeSecs / PUSH_BACK_DURATION_SECS);
        setDistanceFromGround(groundDistance);

        // Move in the direction of the push
        if(pushedBackDirection == Left)
            moveLeft();
        else
            moveRight();
    }
}

Character* Player::check_unit_collide(World *world)
{
    list<GameObject*>* gameObjects = world->getObjects();

    CoordinateUtil cordUtil;

    for(list<GameObject*>::iterator it = gameObjects->begin(); it != gameObjects->end(); it++)
    {
        GameObject *gameObject = (*it);
        int id = gameObject->getID();

        if(id == ID_SHEEP || id == ID_DOG)
        {
            Character* character = (Character*)gameObject;
            if(cordUtil.collide(getSprite(), character->getSprite()))
            {
                if(id == ID_SHEEP)
                {
                    // Remove the item
                    world->removeLevelObject(character);

                    // Update the HUD
                    HUD* hud = world->getHud();
                    hud->addSheep(1);
                }

                break;
            }
        }
    }

    return 0;
}
