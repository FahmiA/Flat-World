#include "Sheep.hpp"

#include "../game/ID.hpp"

#include<iostream>
using namespace std;

Sheep::Sheep(float x, float y, float width, float height, float speed, Sprite *sprite)
    : Character(x, y, width, height, speed, sprite)
{
    timeSinceLastUpdate = 0;
    action = idle;

    setID(ID_SHEEP);
}

Sheep::~Sheep() {}

void Sheep::subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world)
{
    timeSinceLastUpdate += clock->GetElapsedTime();

    if(timeSinceLastUpdate > UPDATE_DELAY)
    {
        timeSinceLastUpdate = 0;

        Randomizer random;
        action = (Action)random.Random(0, 2);

    }else{
        Sprite *sprite = getSprite();
        if(action == left)
        {
            sprite->FlipX(false);
            moveLeft();
        }else if(action == right){
            sprite->FlipX(true);
            moveRight();
        }
    }
}
