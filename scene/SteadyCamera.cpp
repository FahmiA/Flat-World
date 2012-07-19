#include "SteadyCamera.hpp"

#include "game/World.hpp"
#include "actors/Player.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

SteadyCamera::SteadyCamera(View *view) : Camera(view)
{
    //view->Zoom(0.4f);
}

SteadyCamera::~SteadyCamera() { }

void SteadyCamera::update(Clock *clock, RenderWindow *window, World *world)
{
    View *view = getView();
    Player *player = world->getPlayer();

    Vector2f position = player->getPosition();
    position.y -= player->getSize().y / 2;

    view->SetCenter(position);
}
