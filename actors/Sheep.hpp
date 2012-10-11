#ifndef SHEEP_H
#define SHEEP_H

#include "actors/Character.hpp"
#include "actors/ActorStates.hpp"
#include "util/AnimatedSprite.hpp"

/** Defines a sheep to be captured by the player. */
class Sheep: public Character
{
    public:
        Sheep(float x, float y, float width, float height, float speed, AnimatedSprite *sprite);
        ~Sheep();

         void subUpdate(Clock *clock, RenderWindow *window, World *world);

    private:
        ActorState *state;
};

#endif // SHEEP_H
