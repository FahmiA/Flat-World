#include "Camera.hpp"

Camera::Camera()
{
    view = new View();
}

Camera::~Camera()
{
    delete view;
}

void Camera::draw(RenderWindow *window) { /* Sub-classes can implement draw only if they need it. */}

View* Camera::getView()
{
    return view;
}
