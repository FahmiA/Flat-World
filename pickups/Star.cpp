#include "Star.hpp"

#include "game/ID.hpp"


Star::Star(float x, float y, float width, float height, Sprite *sprite)
{
    setID(ID_STAR);

    sprite->SetCenter(sprite->GetSize().x/2, sprite->GetSize().y/2);
    sprite->SetX(x);
    sprite->SetY(y);
    sprite->Resize(width, height);
    this->sprite = sprite;
}

Star::~Star()
{
    //dtor
}

void Star::update(Clock *clock, RenderWindow *window, World *world) { }

void Star::draw(RenderWindow *window)
{
    window->Draw(*sprite);
}

const Vector2f& Star::getPosition()
{
    return sprite->GetPosition();
}

const Vector2f& Star::getSize()
{
    return sprite->GetSize();
}

float Star::getRotation()
{
    return sprite->GetRotation();
}

Sprite* Star::getSprite()
{
    return sprite;
}
