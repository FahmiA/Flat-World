#ifndef ANIMATEDCHARACTER_H
#define ANIMATEDCHARACTER_H

#include "actors/Character.hpp"
#include "util/AnimatedSprite.hpp"

/** A Charcter which has animations. */
class AnimatedCharacter : public Character
{
    public:
        AnimatedCharacter(float x, float y, float width, float height,
                          float speed, AnimatedSprite *sprite);
        virtual ~AnimatedCharacter();

        virtual void subUpdate(Clock *clock, RenderWindow *window, World *world);

        /** Gets the animated Sprite.
          * @return The animated sprite.
          */
        AnimatedSprite* getAniSprite();

        /** Animation name for moving. */
        static const string ANIMATE_RUN;
        /** Animation name for standing still. */
        static const string ANIMATE_IDLE;

    protected:
        /** Convenience method for playing an animation. */
        void playAnimation(string animationName);

    private:
        AnimatedSprite *aniSprite;
};

#endif // ANIMATEDCHARACTER_H
