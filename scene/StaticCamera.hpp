#ifndef STATICCAMERA_H
#define STATICCAMERA_H

#include "Camera.hpp"

class StaticCamera : public Camera
{
    public:
        StaticCamera();
        virtual ~StaticCamera();

        virtual void update(Clock *clock, RenderWindow *window, World *world);

    private:
};

#endif // STATICCAMERA_H
