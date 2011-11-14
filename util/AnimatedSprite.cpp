#include "AnimatedSprite.hpp"

AnimatedSprite::AnimatedSprite(Sprite *spriteSheet, int framesPerSecond,
                                int frameHeight, int frameWidth, int frameGap,
                                int startOffsetX, int startOffsetY)
{
    // Remember arguments
    this->spriteSheet = spriteSheet;
    //this->framesPerSecond = framesPerSecond;
    frameDelay = (float)(framesPerSecond) / 100.0f; // Take as a fraction of one seccond
    this->frameHeight = frameHeight;
    this->frameWidth = frameWidth;
    this->frameGap = frameGap;
    this->startOffsetX = startOffsetX;
    this->startOffsetY = startOffsetY;

    // Setup extra state
    animations = new map<int, AnimationDetail*>();
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

void AnimatedSprite::update(RenderWindow *window, Clock *clock)
{
    //TODO: finish AnimatedSprite update method
    float currentTime = clock->GetElapsedTime();

    // Check whether the next frame should be displayed
    if(currentTime - lastFrameTime >= frameDelay)
    {
        // Progress to the next frame
        currentFrame++;
        if(currentFrame > currentAnimation->noOfFrames)
            currentFrame = 0; // Loop the animation

        Vector2f spriteSheetSize = spriteSheet->GetSize();

        int framePosX = currentAnimation->startFrameX + (currentFrame * frameWidth);
        framePosX = framePosX % (int)spriteSheetSize.x;

        int framePosY = framePosX / spriteSheetSize.x;

        spriteSheet->SetSubRect(IntRect(framePosX, framePosY, framePosY + frameWidth,
                                        framePosY + frameHeight));

        // Reset the last frame time
        lastFrameTime = currentTime;
    }

    // Render the current frame
    window->Draw(*spriteSheet);
}

bool AnimatedSprite::play(int animationId)
{
    // Stop the current animation
    stop();

    // Set the new animation

    if(animations->count(animationId) == 0)
        return false; // The animation does not exist

    currentAnimation = (*animations)[animationId];
}

bool AnimatedSprite::stop()
{
    currentAnimation = 0;
    currentFrame = 0;
}
