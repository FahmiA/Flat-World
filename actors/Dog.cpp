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

    ((AnimatedSprite*)getSprite())->update(clock);

    /*CoordinateUtil coordUtil;
    Player &player = *(world->getPlayer());

    // Change to a chassing state when the player is in the field of view (FOV)
    int fovDepth = 200;
    Vector2f targetPosition(getPosition().x + fovDepth, getPosition().y + fovDepth);
    getSprite()->TransformToLocal(targetPosition);
    Vector2f *collidePos = rayTrace(world->getPlayer()->getSprite(), getPosition().x,
                                   getPosition().y, targetPosition.x, targetPosition.y);

    //if(coordUtil.distance(player.getPosition(), getPosition()) < 100)
    if(collidePos != 0)
    {
        cout << "Dog ANGRY!" << endl;
        delete state;
        state = new ChaseState;
    }*/
}
