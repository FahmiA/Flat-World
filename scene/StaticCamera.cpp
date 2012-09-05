#include "StaticCamera.hpp"

StaticCamera::StaticCamera()
    : Camera(new View())
{
    getView()->reset(FloatRect(0, 0, 800, 600));
    getView()->setViewport(FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
}

StaticCamera::~StaticCamera()
{
    //dtor
}

void StaticCamera::update(Clock *clock, RenderWindow *window, World *world)
{
    // Nothing that needs to be done here
}
