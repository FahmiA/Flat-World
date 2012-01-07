#include "StaticCamera.hpp"

StaticCamera::StaticCamera()
    : Camera(new View())
{
    //ctor
}

StaticCamera::~StaticCamera()
{
    //dtor
}

void StaticCamera::update(Clock *clock, RenderWindow *window, World *world)
{
    // Nothing that needs to be done here
}
