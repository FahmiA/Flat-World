#include "AnimatedSprite.hpp"

#include <iostream>
using namespace std;

AnimatedSprite::AnimatedSprite() : Sprite() {}

void AnimatedSprite::setSpriteSheet(int framesPerSecond,
                                    int frameHeight, int frameWidth, int frameGap,
                                    int startOffsetX, int startOffsetY)
{
    // Remember arguments
    frameDelay = 1.0f / (float)(framesPerSecond); // Take as a fraction of one seccond
    this->frameHeight = frameHeight;
    this->frameWidth = frameWidth;
    this->frameGap = frameGap;
    this->startOffsetX = startOffsetX;
    this->startOffsetY = startOffsetY;

    // Setup extra state
    animations = new map<int, AnimationDetail*>();
    timeSinceLastFrame = 0;
    paused = false;
}

AnimatedSprite::~AnimatedSprite()
{
    delete animations;
}

void AnimatedSprite::addAnimation(int id, int startFrameX, int startFrameY, int noOfFrames)
{
    // Create the animation detail
    AnimationDetail *animationDetail = new AnimationDetail();
    animationDetail->startFrameX = startOffsetX + (startFrameX * frameWidth);
    animationDetail->startFrameY = startOffsetY + (startFrameY * frameHeight);
    animationDetail->noOfFrames = noOfFrames;

    // Delete the old animation if one exists
    if(animations->count(id) > 0)
        delete (*animations)[id];

    // Add the new animation
    (*animations)[id] = animationDetail;
}

void AnimatedSprite::update(Clock *clock)
{
    if(paused)
        return;

    if(clock == 0)
    {
        timeSinceLastFrame = frameDelay;
    }else{
        timeSinceLastFrame += clock->GetElapsedTime();
    }

    //cout << timeSinceLastFrame << ' ' << frameDelay << endl;
    // Check whether the next frame should be displayed
    if(timeSinceLastFrame >= frameDelay)
    {
        // Progress to the next frame
        if(currentFrame > currentAnimation->noOfFrames)
            currentFrame = 0; // Loop the animation

        int imageWidth = GetImage()->GetWidth();

        int framePosX = currentAnimation->startFrameX + (currentFrame * frameWidth);
        framePosX = framePosX % imageWidth;

        int framePosY = startOffsetY + (framePosX / imageWidth);

        SetSubRect(IntRect(framePosX, framePosY, framePosX + frameWidth,
                                        framePosY + frameHeight));
        //cout << "New SubRect: from = (" << framePosX << ", " << framePosY << ")" << endl;
        //cout << '\t' << currentFrame << endl;

        currentFrame++;

        // Update the center
        /*float xPercent = GetCenter().x/GetSize().x;
        float yPercent = GetCenter().y/GetSize().y;
        int newCenterX = framePosX + (frameWidth * xPercent);
        int newCenterY = framePosY + (frameHeight * yPercent);
        SetCenter(newCenterX, newCenterY);*/


        // Reset the timer
        timeSinceLastFrame = 0;
    }
}

bool AnimatedSprite::play(int animationId)
{
    // Stop the current animation
    stop();

    // Set the new animation

    if(animations->count(animationId) == 0)
        return false; // The animation does not exist

    currentAnimation = (*animations)[animationId];

    // Set the subrect to the first frame in the animation.
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

int AnimatedSprite::getFrameHeight()
{
    return frameHeight;
}

int AnimatedSprite::getFrameWidth()
{
    return frameWidth;
}

