#ifndef PLAYER_H
#define PLAYER_H

#include "Character.hpp"

class Player: public Character
{
    public:
        Player(float x, float y, float width, float height, float speed, Sprite *sprite);
        ~Player();

        void subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world);
};

#endif // PLAYER_H
