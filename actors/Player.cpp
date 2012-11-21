#include "Player.hpp"

#include "game/ID.hpp"
#include "util/CoordinateUtil.hpp"
#include "GUI/HUD.hpp"
#include "pickups/Star.hpp"

#include <iostream>
using namespace std;

const float Player::PUSH_BACK_DURATION_SECS = 0.3;
const float Player::PUSH_BACK_MAX_HEIGHT = 40;
const string Player::ANIMATE_JUMP_UP = "JumpUp";
const string Player::ANIMATE_JUMP_DOWN = "JumpDown";

Player::Player(float x, float y, float width, float height, float speed, TSprite *sprite)
    : Character(x, y, width, height, speed, sprite)
{
    setID(ID_PLAYER);

    pushedBackDirection = None;
    pushBackTimeSecs = 0;
}

Player::~Player() {}

void Player::subUpdate(Clock *clock, RenderWindow *window, World *world)
{
    if(pushedBackDirection != None)
    {
        doActionPushBack(clock->getElapsedTime().asSeconds());
    }else{
        doActionNormal(window);
    }

    check_unit_collide(world);
}

void Player::pushBack(Direction pushDirection)
{
    pushedBackDirection = pushDirection;
}

Direction Player::getPushDirection()
{
    return pushedBackDirection;
}

void Player::doActionNormal(RenderWindow *window)
{
    if(Keyboard::isKeyPressed(Keyboard::Left))
    {
        moveLeft();
        getSprite()->play(TSprite::ANIMATE_RUN);
    }else if(Keyboard::isKeyPressed(Keyboard::Right))
    {
        moveRight();
        getSprite()->play(TSprite::ANIMATE_RUN);
    }else{
        getSprite()->play(TSprite::ANIMATE_IDLE);
    }

    if(Keyboard::isKeyPressed(Keyboard::Space))
        landHop();

    if(Keyboard::isKeyPressed(Keyboard::P))
        pushBack(Left);
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

        // move in the direction of the push
        if(pushedBackDirection == Left)
            moveLeft();
        else
            moveRight();
    }
}

Character* Player::check_unit_collide(World *world)
{
    list<GameObject*>* gameObjects = world->getObjects();

    CoordinateUtil coordUtil;

    for(list<GameObject*>::iterator it = gameObjects->begin(); it != gameObjects->end(); it++)
    {
        GameObject *gameObject = (*it);
        int id = gameObject->getID();

        // Check for sheep to capture
        if(id == ID_SHEEP)
        {
            Character *character = (Character*)gameObject;
            if(coordUtil.collide(getSprite(), character->getSprite()))
            {
                // Remove the item
                world->removeLevelObject(character);

                // Update the HUD
                HUD* hud = world->getHud();
                hud->addSheep(1);

                break;
            }
        }

        // Check for star to capture
        if(id == ID_STAR)
        {
            Star *star = (Star*)gameObject;
            if(coordUtil.collide(getSprite(), star->getSprite()))
            {
                // Remove the item
                world->removeLevelObject(star);

                // Update the HUD
                HUD* hud = world->getHud();
                hud->addStars(1);

                break;
            }
        }
    }

    return 0;
}
