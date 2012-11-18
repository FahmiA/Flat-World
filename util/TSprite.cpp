#include "TSprite.hpp"

#include <iostream>
using namespace std;

#include "util/Debug.hpp"
#include "util/SpriteUtil.hpp"
#include "util/Numberutil.hpp"

const string TSprite::ANIMATE_RUN = "Run";
const string TSprite::ANIMATE_IDLE = "Idle";

TSprite::TSprite(const Image &image)
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
    direction = Right;
}

TSprite::~TSprite()
{
    animations.clear();
}

void TSprite::addAnimation(Animation *animation)
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

void TSprite::update(Clock *clock)
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

        //updateDirection();

        // Update the frame counter
        currentFrame++;
        //cout << "Frame:" << currentFrame << '/' << currentAnimation->frames.size() << endl;

        // Reset the timer
        timeSinceLastFrame = 0;
    }
}

void TSprite::draw(RenderWindow *window)
{
    window->draw(sprite);
}

void TSprite::clear()
{
    sprite.setScale(originalScale);
    sprite.setPosition(originalPosition);
}

bool TSprite::play(string animationName)
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

void TSprite::pause()
{
    paused = true;
}

void TSprite::resume()
{
    paused = false;
}

void TSprite::stop()
{
    currentAnimation = 0;
    currentFrame = 0;
    timeSinceLastFrame = 0;
    paused = false;
}

void TSprite::lookLeft()
{
    if(direction != Left)
    {
        //sprite.setOrigin(sprite.getLocalBounds().width, sprite.getOrigin().y); // TODO: Generalise
        Vector2f scale = sprite.getScale();
        sprite.setScale(-scale.x, scale.y);
        direction = Left;
    }
}

void TSprite::lookRight()
{
    // TODO; Complete afrer lookLeft works
    /*if(direction != Right)
    {
        image.flipHorizontally();
        direction = Right;
    }*/
}

Vector2f TSprite::getPosition()
{
    Vector2f pos(sprite.getPosition());
    //pos = toLocal(pos);
    //pos = toGlobal(pos);
    return pos;
}

void TSprite::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

void TSprite::setOrigin(float x, float y)
{
    FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width * x, bounds.height * y);
}

Vector2f TSprite::getSize()
{
    IntRect textureSize = sprite.getTextureRect();
    Vector2f scale = sprite.getScale();

    Vector2f size(textureSize.width * scale.x, textureSize.height * scale.y);
    return size;
}

Vector2f TSprite::toGlobal(const Vector2f &point)
{
    Transform globalTransform = sprite.getTransform();

    /*Vector2f scaledPoint = globalTransform.transformPoint(point);
    scaledPoint.x /= sprite.getScale().x;
    scaledPoint.y /= sprite.getScale().y;
    return scaledPoint;*/
    Vector2f result(point);
    if(direction == Left)
    {
        //cout << "ME" << PRINT_V(getPosition()) << endl;
        //cout << PRINT_V(result) << endl;
        //result.x *= -1;
        //result.y *= -1;
        globalTransform.scale(-1,1);
    }

    result = globalTransform.transformPoint(point);
    return result;
}

Vector2f TSprite::toLocal(const Vector2f &point)
{
    Transform localTransform = sprite.getInverseTransform();

    /*Vector2f scaledPoint = localTransform.transformPoint(point);
    scaledPoint.x *= sprite.getScale().x;
    scaledPoint.y *= sprite.getScale().y;
    return scaledPoint;*/
    Vector2f result(point);
    if(direction == Left)
    {
        //result.x *= -1;
        //result.y *= -1;
        //result.x = sprite.getLocalBounds().left - result.x;
        localTransform.scale(-1,1);
    }

    result = localTransform.transformPoint(point);
    return result;
}

Image* TSprite::getImage()
{
    return &image;
}

Sprite* TSprite::getRawSprite()
{
    return &sprite;
}

void TSprite::setSize(float width, float height)
{
    FloatRect originalSize = sprite.getLocalBounds();
    float originalWidth = originalSize.width;
    float originalHeight = originalSize.height;

    if ((originalWidth > 0) && (originalHeight > 0))
        sprite.setScale(width / originalWidth, height / originalHeight);
}

void TSprite::setRotation(float angleR)
{
    sprite.setRotation(AS_DEG(angleR));
}

float TSprite::getRotation()
{
    return AS_RAD(sprite.getRotation());
}

void TSprite::move(float x, float y)
{
    sprite.move(x, y);
}

FloatRect TSprite::getLocalBounds()
{
    return sprite.getLocalBounds();
}
