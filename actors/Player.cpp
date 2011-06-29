#include "Player.hpp"

Player::Player(float x, float y, float width, float height, float speed, Sprite *sprite)
{
    this->speed = speed;

    sprite->SetCenter(sprite->GetImage()->GetWidth()/2, sprite->GetImage()->GetHeight()/2);
    sprite->SetX(x);
    sprite->SetY(y);
    sprite->Resize(width, height);
    this->sprite = sprite;
}

Player::~Player()
{
    delete sprite;
}

void Player::update(Clock *clock, RenderWindow *window)
{
    float elapsedTime = clock->GetElapsedTime();

    if(window->GetInput().IsKeyDown(Key::Right))
        sprite->Move(speed * elapsedTime, 0);

    if(window->GetInput().IsKeyDown(Key::Left))
        sprite->Move(-speed * elapsedTime, 0);
}

void Player::draw(RenderWindow *window)
{
    window->Draw(*sprite);
}
