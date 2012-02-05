#ifndef DOG_H
#define DOG_H

#include "actors/Character.hpp"
#include "actors/ActorStates.hpp"

#include "util/AnimatedSprite.hpp"

class LevelBuilder;

class Dog: public Character
{
    public:
        Dog(float x, float y, float width, float height,
            float speed, Sprite *sprite, LevelBuilder *levelBuilder);
        virtual ~Dog();

        void subUpdate(Clock *clock, RenderWindow *window, World *world);

    protected:

    private:
        LevelBuilder *levelBuilder;
        ActorState *state;
        float timeSincePlayerSeen;

        void seekPlayer(float elapsedTime, Player &player);
        void checkPlayerCollide(World &world);
};

#endif // DOG_H
