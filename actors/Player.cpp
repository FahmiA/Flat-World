#include "Player.hpp"

#include "../game/ID.hpp"
#include "../util/CoordinateUtil.hpp"
#include "../GUI/HUD.hpp"

#include <iostream>
using namespace std;

Player::Player(float x, float y, float width, float height, float speed, Sprite *sprite)
    : Character(x, y, width, height, speed, sprite)
{
    setID(ID_PLAYER);
}

Player::~Player() {}

void Player::subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world)
{
    const Input &input = window->GetInput();
    if(input.IsKeyDown(Key::Left))
        moveLeft();

    if(input.IsKeyDown(Key::Right))
        moveRight();

    if(input.IsKeyDown(Key::Space))
        landHop();

    check_unit_collide(world);
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
