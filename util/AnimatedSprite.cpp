#include "AnimatedSprite.hpp"

#include <iostream>
using namespace std;

AnimatedSprite::AnimatedSprite() : Sprite()
{
    timeSinceLastFrame = 0;
    paused = false;
}

AnimatedSprite::~AnimatedSprite()
{
    animations.clear();
}

void AnimatedSprite::addAnimation(Animation *animation)
{
    // Delete the old animation if one exists
    if(animations.count(animation->name) > 0)
        delete animations[animation->name];

    // Add the new animation
    animations[animation->name] = animation;
}

void AnimatedSprite::setTransparentColour(unsigned int tColour)
{
    // TODO: Implement (May need SFML v2.0)
}

void AnimatedSprite::update(Clock *clock)
{
    if(paused || currentAnimation == 0)
        return;

    bool advanceFrame = false;
    if(clock != 0)
    {
        timeSinceLastFrame += clock->GetElapsedTime();
        if(timeSinceLastFrame > 1.0f / currentAnimation->frameRate) // seconds
            advanceFrame = true;
    }else{
        // No click will force an animation change
        advanceFrame = true;
    }

    // Check whether the next frame should be displayed
    if(advanceFrame)
    {
        // Progress to the next frame
        if(currentFrame >= currentAnimation->frames.size())
            currentFrame = 0; // Loop the animation

        // DIsplay the frame
        AnimationFrame &frame = *currentAnimation->frames[currentFrame];
        SetSubRect(IntRect(frame.x, frame.y,
                           frame.x + frame.width,
                           frame.y + frame.height));
        currentFrame++;

        // Reset the timer
        timeSinceLastFrame = 0;
    }
}

bool AnimatedSprite::play(string animationName)
{
    // Stop the current animation
    stop();

    // Set the new animation
    if(animations.count(animationName) == 0)
        return false; // The animation does not exist

    currentAnimation = animations[animationName];

    // Set the subrect to the first frame of the animation.
    update(0);
}

void AnimatedSprite::pause()
{
    paused = true;
}

void AnimatedSprite::resume()
{
    paused = false;
}

void AnimatedSprite::stop()
{
    currentAnimation = 0;
    currentFrame = 0;
    timeSinceLastFrame = 0;
    paused = false;
}

/*int AnimatedSprite::getFrameHeight()
{
    return GetSubRect().GetHeight();
}

int AnimatedSprite::getFrameWidth()
{
    return GetSubRect().GetWidth();
}*/

