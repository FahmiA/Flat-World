#include "AnimatedCharacter.hpp"

const string AnimatedCharacter::ANIMATE_RUN = "Run";
const string AnimatedCharacter::ANIMATE_IDLE = "Idle";

AnimatedCharacter::AnimatedCharacter(float x, float y,
    float width, float height, float speed, AnimatedSprite *sprite)
    : Character(x, y, width, height, speed, sprite)

{
    // Store the animated sprite
    aniSprite = sprite;

    // Notify the sprite of changes in size and center
    aniSprite->SetSize(SpriteUtil::getSize(sprite));
    aniSprite->setOrigin(sprite->getOrigin());
}

AnimatedCharacter::~AnimatedCharacter()
{
    //dtor
}

void AnimatedCharacter::subUpdate(Clock *clock, RenderWindow *window, World *world)
{
    aniSprite->update(clock);
}

AnimatedSprite* AnimatedCharacter::getAniSprite()
{
    return aniSprite;
}

void AnimatedCharacter::playAnimation(string animationName)
{
    aniSprite->play(animationName);
}
