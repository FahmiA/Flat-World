#include "StaticCamera.hpp"

StaticCamera::StaticCamera()
    : Camera(new View())
{
    getView()->setViewport(FloatRect(0, 0, 800, 600));
}

StaticCamera::~StaticCamera()
{
    //dtor
}

void StaticCamera::update(Clock *clock, RenderWindow *window, World *world)
{
    // Nothing that needs to be done here
}
