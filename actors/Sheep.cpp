#include "Sheep.hpp"

#include "../game/ID.hpp"

#include<iostream>
using namespace std;

Sheep::Sheep(float x, float y, float width, float height, float speed, AnimatedSprite *sprite)
    : Character(x, y, width, height, speed, sprite)
{
    setID(ID_SHEEP);

    state = new WonderState;
}

Sheep::~Sheep()
{
    delete state;
}

void Sheep::subUpdate(Clock *clock, RenderWindow *window, World *world)
{
    state->performAction(this, clock);

    ((AnimatedSprite*)getSprite())->update(clock);
}
