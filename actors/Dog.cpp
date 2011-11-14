#include "Dog.hpp"

#include "../game/ID.hpp"
#include "../util/CoordinateUtil.hpp"
#include "Player.hpp"

Dog::Dog(float x, float y, float width, float height, float speed, Sprite *sprite)
    : Character(x, y, width, height, speed, sprite)
{
    setID(ID_DOG);

    state = new WonderState;
}

Dog::~Dog()
{
    delete state;
}

void Dog::subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world)
{
    state->performAction(this, clock);

    CoordinateUtil coordUtil;
    Player &player = *(world->getPlayer());

    if(coordUtil.distance(player.getPosition(), getPosition()) < 100)
    {
        delete state;
        state = new ChaseState;
    }
}
