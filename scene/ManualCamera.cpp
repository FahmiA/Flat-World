#include "ManualCamera.hpp"

ManualCamera::ManualCamera(View *view) : Camera(view)
{
    speed = 500;
}

ManualCamera::~ManualCamera() { }

void ManualCamera::update(Clock *clock, RenderWindow *window, World *world)
{
    // Get the properties
    View *view = getView();
    float moveDistance = speed * clock->getElapsedTime().asSeconds();

    // Respond to user movement control
    if(Keyboard::isKeyPressed(Keyboard::Left))
        view->move(-moveDistance, 0);

    if(Keyboard::isKeyPressed(Keyboard::Right))
        view->move(moveDistance, 0);

    if(Keyboard::isKeyPressed(Keyboard::Up))
        view->move(0, -moveDistance);

    if(Keyboard::isKeyPressed(Keyboard::Down))
        view->move(0, moveDistance);

    // Respond to user zoom control
    if(Keyboard::isKeyPressed(Keyboard::Add))
        view->zoom(1.6f);
    if(Keyboard::isKeyPressed(Keyboard::Subtract))
        view->zoom(0.6f);
}
