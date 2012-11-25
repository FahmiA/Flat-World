#include "SteadyCamera.hpp"

#include "game/World.hpp"
#include "actors/Player.hpp"
#include "util/NumberUtil.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

#include <iostream>
using namespace std;

SteadyCamera::SteadyCamera(View *view) : Camera(view)
{
    view->zoom(1.2f);
}

SteadyCamera::~SteadyCamera() { }

void SteadyCamera::update(Clock *clock, RenderWindow *window, World *world)
{
    View *view = getView();
    Player *player = world->getPlayer();
    float elapsedTime = clock->getElapsedTime().asSeconds();

    // Set the position
    Vector2f position = player->getPosition();
    position.y -= player->getSize().y / 2;
    view->setCenter(position);

    // Set the rotation
    float newRotation = player->getRotation();// / AS_RAD(view->getRotation()));
    view->setRotation(AS_DEG(newRotation));
}
