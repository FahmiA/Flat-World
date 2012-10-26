#include "AnimatedSprite.hpp"

#include <iostream>
using namespace std;

#include "util/SpriteUtil.hpp"
#include "util/Numberutil.hpp"

const string AnimatedSprite::ANIMATE_RUN = "Run";
const string AnimatedSprite::ANIMATE_IDLE = "Idle";

AnimatedSprite::AnimatedSprite(const Image &image)
{
    // Initialise the Sprite
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    // Store the sprite and image details
    this->image = image; // TODO: Remove expensive duplicate
    originalScale = sprite.getScale();
    originalPosition = sprite.getPosition();

    // State
    currentAnimation = 0;
    timeSinceLastFrame = 0;
    paused = false;
    direction = Left;
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

    // Set the subject if this is the first animation
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

    originalScale = sprite.getScale();
    originalPosition = sprite.getPosition();

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
        IntRect subRect(frame.x , frame.y,
                        frame.width,
                        frame.height);

        // Remember some original Sprite values
        Vector2f size = getSize();
        Vector2f origin(sprite.getOrigin());
        FloatRect localBounds(getLocalBounds());

        // Update the frame (changes Sprite values)
        sprite.setTextureRect(subRect);

        // Re-apply the original Sprite values
        setSize(size.x, size.y);
        sprite.setOrigin(Vector2f((origin.x / localBounds.width) * sprite.getLocalBounds().width,
                           (origin.y / localBounds.height) * sprite.getLocalBounds().height));

        updateDirection();

        // Update the frame counter
        currentFrame++;
        //cout << "Frame:" << currentFrame << '/' << currentAnimation->frames.size() << endl;

        // Reset the timer
        timeSinceLastFrame = 0;
    }
}

void AnimatedSprite::draw(RenderWindow *window)
{
    window->draw(sprite);
}

void AnimatedSprite::clear()
{
    sprite.setScale(originalScale);
    sprite.setPosition(originalPosition);
}

void AnimatedSprite::updateDirection()
{
    Vector2f scale = sprite.getScale();

    // Calculate the position of the flipped sprite
    Transform globalTransform = sprite.getTransform();
    Vector2f flippedOrigin = Vector2f(sprite.getOrigin().x + sprite.getLocalBounds().width, sprite.getOrigin().y);
    Vector2f flippedPos = globalTransform.transformPoint(flippedOrigin);

    // Fli0p the sprite if requested
    if(direction == Left && scale.x < 0)
    {
        // Keep the scale positive
        scale.x = -scale.x;
        sprite.setScale(scale.x, scale.y);
        sprite.setPosition(flippedOrigin);
    }else if(direction != Left && scale.x > 0){
        // Keep the scale negative
        scale.x = -scale.x;
        sprite.setScale(scale.x, scale.y);
        sprite.setPosition(flippedPos);
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

void AnimatedSprite::lookLeft()
{
    //direction = Left;
}

void AnimatedSprite::lookRight()
{
    //direction = Right;
}

Vector2f AnimatedSprite::getPosition()
{
    return sprite.getPosition();
}

void AnimatedSprite::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

void AnimatedSprite::setOrigin(float x, float y)
{
    FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width * x, bounds.height * y);
}

Vector2f AnimatedSprite::getSize()
{
    IntRect textureSize = sprite.getTextureRect();
    Vector2f scale = sprite.getScale();

    Vector2f size(textureSize.width * scale.x, textureSize.height * scale.y);
    return size;
}

Vector2f AnimatedSprite::toGlobal(const Vector2f &point)
{
    Transform globalTransform = sprite.getTransform();
    return globalTransform.transformPoint(point);
}

Vector2f AnimatedSprite::toLocal(const Vector2f &point)
{
    Transform localTransform = sprite.getInverseTransform();
    return localTransform.transformPoint(point);
}

Image* AnimatedSprite::getImage()
{
    return &image;
}

Sprite* AnimatedSprite::getRawSprite()
{
    return &sprite;
}

void AnimatedSprite::setSize(float width, float height)
{
    FloatRect originalSize = sprite.getLocalBounds();
    float originalWidth = originalSize.width;
    float originalHeight = originalSize.height;

    if ((originalWidth > 0) && (originalHeight > 0))
        sprite.setScale(width / originalWidth, height / originalHeight);
}

void AnimatedSprite::setRotation(float angleR)
{
    sprite.setRotation(AS_DEG(angleR));
}

float AnimatedSprite::getRotation()
{
    return AS_RAD(sprite.getRotation());
}

void AnimatedSprite::move(float x, float y)
{
    sprite.move(x, y);
}

FloatRect AnimatedSprite::getLocalBounds()
{
    return sprite.getLocalBounds();
}
