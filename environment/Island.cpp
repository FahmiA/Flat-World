#include "Island.hpp"

#include "game/ID.hpp"
#include "util/SpriteUtil.hpp"

Island::Island(float x, float y, float width, float height, TSprite *sprite)
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

TSprite* Island::getSprite()
{
    return sprite;
}

const Image* Island::getImage()
{
    return sprite->getImage();
}

void Island::update(Clock *clock, RenderWindow *window, World *world)
{

}

void Island::draw(RenderWindow *window)
{
    sprite->draw(window);
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
    return sprite->getSize();
}
