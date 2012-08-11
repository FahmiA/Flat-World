#include "Island.hpp"

#include "game/ID.hpp"
#include "util/SpriteUtil.hpp"

Island::Island(float x, float y, float width, float height, Sprite *sprite, Image *image)
{
    SpriteUtil::resize(sprite, width, height);
    sprite->setOrigin(sprite->getLocalBounds().width / 2, sprite->getLocalBounds().height / 2);
    sprite->setPosition(x, y);

    this->sprite = sprite;
    this->image = image;

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
    return image;
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
