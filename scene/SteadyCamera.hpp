#ifndef STEADYCAMERA_H
#define STEADYCAMERA_H

#include "Camera.hpp"

class SteadyCamera: public Camera
{
    public:
        SteadyCamera();
        virtual ~SteadyCamera();

        virtual void update(Clock *clock, RenderWindow *window, World *world);
};

#endif // STEADYCAMERA_H
