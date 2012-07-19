#include "ManualCamera.hpp"

ManualCamera::ManualCamera(View *view) : Camera(view)
{
    speed = 500;
    view->Zoom(0.6);
}

ManualCamera::~ManualCamera() { }

void ManualCamera::update(Clock *clock, RenderWindow *window, World *world)
{
    // Get the properties
    const Input &input = window->GetInput();
    View *view = getView();
    float moveDistance = speed * clock->GetElapsedTime();

    // Respond to user control
    if(input.IsKeyDown(Key::Left))
        view->Move(-moveDistance, 0);

    if(input.IsKeyDown(Key::Right))
        view->Move(moveDistance, 0);

    if(input.IsKeyDown(Key::Up))
        view->Move(0, -moveDistance);

    if(input.IsKeyDown(Key::Down))
        view->Move(0, moveDistance);
}
