#ifndef STATICCAMERA_H
#define STATICCAMERA_H

#include "Camera.hpp"

/** A camera that does not move from its current position.
 * Useful for displaying static images such as the HUD.
 */
class StaticCamera : public Camera
{
    public:
        StaticCamera();
        virtual ~StaticCamera();

        virtual void update(Clock *clock, RenderWindow *window, World *world);

    private:
};

#endif // STATICCAMERA_H
