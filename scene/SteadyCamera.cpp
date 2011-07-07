#include "SteadyCamera.hpp"

#include "../game/World.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

SteadyCamera::SteadyCamera() { }

SteadyCamera::~SteadyCamera() { }

void SteadyCamera::update(Clock *clock, RenderWindow *window, World *world)
{
    View *view = getView();
    Player *player = world->getPlayer();

    view->SetCenter(player->getPosition());

    window->SetView(*view);
}
