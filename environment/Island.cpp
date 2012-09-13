#include "Island.hpp"

#include "game/ID.hpp"
#include "util/SpriteUtil.hpp"

Island::Island(float x, float y, float width, float height, AnimatedSprite *sprite)
{
    sprite->setSize(width, height);
    sprite->setOrigin(0.5f, 0.5f);
    sprite->setPosition(x, y);

    this->sprite = sprite;

    setID(ID_ISLAND);
}

Island::~Island()
{
    delete sprite;
}

Sprite* Island::getSprite()
{
    return sprite;
}

Image* Island::getImage()
{
    return sprite->getImage();
}

void Island::update(Clock *clock, RenderWindow *window, World *world)
{

}

void Island::draw(RenderWindow *window)
{
    window->draw(*sprite);
}

const Vector2f& Island::getPosition()
{
    return sprite->getPosition();
}

void Island::setPosition(float x, float y)
{
    sprite->setPosition(x, y);
}

const Vector2f& Island::getSize()
{
    return SpriteUtil::getSize(sprite);
}
