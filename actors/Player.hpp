#ifndef PLAYER_H
#define PLAYER_H

#include "Character.hpp"

class Player: public Character
{
    public:
        Player(float x, float y, float width, float height, float speed, Sprite *sprite);
        ~Player();

        void subUpdate(Clock *clock, RenderWindow *window, World *world);

        /** Pushes the player back. Commonly when attacked. */
        void pushBack();

    private:
        bool beingPushedBack;
        float pushBackTimeSecs;
        const float PUSH_BACK_DURATION_SECS;
        const float PUSH_BACK_MAX_HEIGHT;

        void doActionNormal(RenderWindow *window);
        void doActionPushBack(float elapsedTime);

        Character* check_unit_collide(World *world);
};

#endif // PLAYER_H
