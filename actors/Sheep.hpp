#ifndef SHEEP_H
#define SHEEP_H

#include "Character.hpp"
#include "ActorStates.hpp"

class Sheep: public Character
{
    public:
        Sheep(float x, float y, float width, float height, float speed, Sprite *sprite);
        ~Sheep();

         void subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world);

    private:
        ActorState *state;
};

#endif // SHEEP_H
