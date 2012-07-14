#include "Island.hpp"

#include "game/ID.hpp"

Island::Island(float x, float y, float width, float height, Sprite *sprite)
{
    sprite->SetCenter(sprite->GetSize().x / 2, sprite->GetSize().y / 2);
    sprite->Resize(width, height);
    sprite->SetX(x);
    sprite->SetY(y);

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

void Island::update(Clock *clock, RenderWindow *window, World *world)
{

}

void Island::draw(RenderWindow *window)
{
    window->Draw(*sprite);
}

const Vector2f& Island::getPosition()
{
    return sprite->GetPosition();
}

void Island::setPosition(float x, float y)
{
    sprite->SetPosition(x, y);
}

const Vector2f& Island::getSize()
{
    return sprite->GetSize();
}
