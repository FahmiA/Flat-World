#include "Island.hpp"

#include "../game/ID.hpp"

Island::Island(float x, float y, float width, float height, Sprite *sprite)
{
    sprite->SetCenter(sprite->GetImage()->GetWidth()/2, sprite->GetImage()->GetHeight()/2);
    sprite->SetX(x);
    sprite->SetY(y);
    sprite->Resize(width, height);
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

const Vector2f& Island::getSize()
{
    return sprite->GetSize();
}
