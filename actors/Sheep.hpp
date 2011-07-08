#ifndef SHEEP_H
#define SHEEP_H

#include "Character.hpp"

#define UPDATE_DELAY 0.5f

class Sheep: public Character
{
    public:
        Sheep(float x, float y, float width, float height, float speed, Sprite *sprite);
        ~Sheep();

         void subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world);

    private:
        float timeSinceLastUpdate;

        enum Action {idle, left, right};
        Action action;
};

#endif // SHEEP_H
