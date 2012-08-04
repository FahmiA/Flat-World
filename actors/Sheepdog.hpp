#ifndef DOG_H
#define DOG_H

#include "actors/Character.hpp"
#include "actors/ActorStates.hpp"

#include "util/AnimatedSprite.hpp"

class LevelBuilder;

/** Defines a sheepdog which can attack the player. */
class Sheepdog: public Character
{
    public:
        Sheepdog(float x, float y, float width, float height,
            float speed, AnimatedSprite *sprite, LevelBuilder *levelBuilder);
        virtual ~Sheepdog();

        void subUpdate(Clock *clock, RenderWindow *window, World *world);

    protected:

    private:
        LevelBuilder *levelBuilder;
        ActorState *state;
        float timeSincePlayerSeen;

        AnimatedSprite *sprite;

        /** Checks if the player is in the FOV and eacts.
         * If the Player is seen, the Sheepdog will chase the player,
         * If the player is not seen, the SHeepdog will eventually forget
         * the player and stop chasing.
         * @param elapsedTime The elapsed time, in seconds, since the last frame.
         * @param player The Player to look for.
         */
        void seekPlayer(float elapsedTime, Player &player);

        /** Checks if the Sheepdog has collided with the player and reacts.
         * If the Player has collided with the Sheepdog, the player will be
         * attacked. But only if the Sheepdog is currently chasing the Player.
         * @param The state of the game world.
         */
        void checkPlayerCollide(World &world);
};

#endif // DOG_H
