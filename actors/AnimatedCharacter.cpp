#include "AnimatedCharacter.hpp"

#include <iostream>
using namespace std;

#include "util/Commons.hpp"

const string AnimatedCharacter::ANIMATE_RUN = "Run";
const string AnimatedCharacter::ANIMATE_IDLE = "Idle";

AnimatedCharacter::AnimatedCharacter(float x, float y,
    float width, float height, float speed, AnimatedSprite *sprite)
    : Character(x, y, width, height, speed, sprite)
{
    // Store the animated sprite
    aniSprite = sprite;
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
