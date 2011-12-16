#include "Camera.hpp"

Camera::Camera(View *view)
{
    this->view = view;
}

Camera::~Camera() { }

void Camera::draw(RenderWindow *window) { /* Sub-classes can implement draw only if they need it. */}

View* Camera::getView()
{
    return view;
}
