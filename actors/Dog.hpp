#ifndef DOG_H
#define DOG_H

#include "Character.hpp"
#include "ActorStates.hpp"

#include "../util/AnimatedSprite.hpp"

class Dog: public Character
{
    public:
        Dog(float x, float y, float width, float height, float speed, Sprite *sprite);
        virtual ~Dog();

        void subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world);

    protected:

    private:
        ActorState *state;
};

/*class DogState
{
    public:

};*/

#endif // DOG_H
