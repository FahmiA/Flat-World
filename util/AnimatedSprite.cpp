#include "AnimatedSprite.hpp"

#include <iostream>
using namespace std;

#include "util/SpriteUtil.hpp"

AnimatedSprite::AnimatedSprite() : Sprite()
{
    currentAnimation = 0;
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

    // Set the subrect if this is the first animation
    // This is to ensure the sprite is sized correctly
    if(animations.size() == 1)
    {
        play(animation->name);
        stop();
    }
}

void AnimatedSprite::update(Clock *clock)
{
    if(paused || currentAnimation == 0)
        return;

    bool advanceFrame = false;
    if(clock != 0)
    {
        timeSinceLastFrame += clock->getElapsedTime().asSeconds();
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

        // Display the frame
        AnimationFrame &frame = *currentAnimation->frames[currentFrame];
        //float scale = min(size->x / frame.width, size->y / frame.height);
        IntRect subRect(frame.x, frame.y,
                        frame.width,
                        frame.height);

        // Remember some original Sprite values
        Vector2f size(SpriteUtil::getSize(this));
        Vector2f origin(getOrigin());
        FloatRect localBounds(getLocalBounds());

        // Update the frame (changes Sprite values)
        setTextureRect(subRect);

        // Re-apply the original Sprite values
        SpriteUtil::resize(this, size.x, size.y);
        setOrigin(Vector2f((origin.x / localBounds.width) * getLocalBounds().width,
                           (origin.y / localBounds.height) * getLocalBounds().height));

        // Update the frame counter
        currentFrame++;

        // Reset the timer
        timeSinceLastFrame = 0;
    }
}

bool AnimatedSprite::play(string animationName)
{
    if(currentAnimation != 0 && animationName == currentAnimation->name)
        return true;

    // Stop the current animation
    stop();

    // Set the new animation
    if(animations.count(animationName) == 0)
        return false; // The animation does not exist

    currentAnimation = animations[animationName];

    // Set the subrect to the first frame of the animation.
    update(0);

    return true;
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
