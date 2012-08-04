#include "AnimatedSprite.hpp"

#include <iostream>
using namespace std;

AnimatedSprite::AnimatedSprite() : Sprite()
{
    currentAnimation = 0;
    timeSinceLastFrame = 0;
    paused = false;
}

AnimatedSprite::~AnimatedSprite()
{
    animations.clear();

    delete size;
    delete center;
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
        size = new Vector2f(GetSize().x, GetSize().y);
        center = new Vector2f(GetCenter().x, GetCenter().y);
        play(animation->name);
        stop();
    }
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

        // Display the frame
        AnimationFrame &frame = *currentAnimation->frames[currentFrame];
        //float scale = min(size->x / frame.width, size->y / frame.height);
        IntRect subRect(frame.x, frame.y,
                        frame.x + frame.width,
                        frame.y + frame.height);
        SetSubRect(subRect);
        Resize(*size);
        Sprite::SetCenter(Vector2f((center->x / size->x) * frame.width,
                                   (center->y / size->y) * frame.height));
        //cout << "Size: " << GetSize().x << " * " << GetSize().y << endl;
        //cout << "Cent: " << GetCenter().x << " * " << GetCenter().y << endl;

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

void AnimatedSprite::SetSize(Vector2f size)
{
    delete this->size;
    this->size = new Vector2f(size.x, size.y);
    //cout << "---" <<  size.x << endl;
}

void AnimatedSprite::SetCenter(Vector2f center)
{
    delete this->center;
    this->center = new Vector2f(center.x, center.y);
}

/*int AnimatedSprite::getFrameHeight()
{
    return GetSubRect().GetHeight();
}

int AnimatedSprite::getFrameWidth()
{
    return GetSubRect().GetWidth();
}*/

