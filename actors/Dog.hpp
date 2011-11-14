#ifndef DOG_H
#define DOG_H

#include "Character.hpp"

class Dog: public Character
{
    public:
        Dog(float x, float y, float width, float height, float speed, Sprite *sprite);
        virtual ~Dog();

        void subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world);

    protected:

    private:
};

/*class DogState
{
    public:

};*/

#endif // DOG_H
