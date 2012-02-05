#ifndef PLAYER_H
#define PLAYER_H

#include "Character.hpp"
#include "util/Commons.hpp"

class Player: public Character
{
    public:
        Player(float x, float y, float width, float height, float speed, Sprite *sprite);
        ~Player();

        void subUpdate(Clock *clock, RenderWindow *window, World *world);

        /** Pushes the player back. Commonly when attacked.
         * Can be pushed left or right, but not up or down.
         * @param pushDirection The direction which the player is being pushed in.
         */
        void pushBack(Direction pushDirection);

        /** Gets the direction the player is being pushed back.
         * "Left" or "Right" if the player is being pushed back.
         * "None" if the player is not being pushed back.
         * @return Direction of push or "None" if not being pushed.
         */
        Direction getPushDirection();

    private:
        Direction pushedBackDirection;
        float pushBackTimeSecs;
        const float PUSH_BACK_DURATION_SECS;
        const float PUSH_BACK_MAX_HEIGHT;

        void doActionNormal(RenderWindow *window);
        void doActionPushBack(float elapsedTime);

        Character* check_unit_collide(World *world);
};

#endif // PLAYER_H
