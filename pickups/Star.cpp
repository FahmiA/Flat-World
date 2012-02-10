#include "Star.hpp"

#include "game/ID.hpp"

#define STAR_SPEED 100

Star::Star(float x, float y, float width, float height, Sprite *sprite)
    : Character(x, y, width, height, STAR_SPEED, sprite)
{
    setID(ID_STAR);

    setDistanceFromGround(height/2);
}

Star::~Star()
{
    //dtor
}

void Star::subUpdate(Clock *clock, RenderWindow *window, World *world)
{
    moveLeft();
}
