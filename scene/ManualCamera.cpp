#include "ManualCamera.hpp"

ManualCamera::ManualCamera(View *view) : Camera(view)
{
    speed = 400;
}

ManualCamera::~ManualCamera() { }

void ManualCamera::update(Clock *clock, RenderWindow *window, World *world)
{
    const Input &input = window->GetInput();
    View *view = getView();
    float moveDistance = speed * clock->GetElapsedTime();

    if(input.IsKeyDown(Key::Left))
        view->Move(-moveDistance, 0);

    if(input.IsKeyDown(Key::Right))
        view->Move(moveDistance, 0);

    if(input.IsKeyDown(Key::Up))
        view->Move(0, -moveDistance);

     if(input.IsKeyDown(Key::Down))
        view->Move(0, moveDistance);

    window->SetView(*view);
}
