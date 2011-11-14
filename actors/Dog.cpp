#include "Dog.hpp"

#include "../game/ID.hpp"

Dog::Dog(float x, float y, float width, float height, float speed, Sprite *sprite)
    : Character(x, y, width, height, speed, sprite)
{
    setID(ID_DOG);
}

Dog::~Dog()
{
    //dtor
}

void Dog::subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world)
{
    // finish
}
