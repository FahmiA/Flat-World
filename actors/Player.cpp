#include "Player.hpp"

#include "../game/ID.hpp"

Player::Player(float x, float y, float width, float height, float speed, Sprite *sprite)
    : Character(x, y, width, height, speed, sprite)
{
    setID(ID_PLAYER);
}

Player::~Player() {}

void Player::subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world)
{
    const Input &input = window->GetInput();
    if(input.IsKeyDown(Key::Left))
        moveLeft();

    if(input.IsKeyDown(Key::Right))
        moveRight();

    if(input.IsKeyDown(Key::Space))
        landHop();
}
