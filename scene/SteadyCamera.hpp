#ifndef STEADYCAMERA_H
#define STEADYCAMERA_H

#include "Camera.hpp"

/** A camera that follows the player.
 * Steady because it does not rotate with the player.
 */
class SteadyCamera: public Camera
{
    public:
        SteadyCamera(View *view);
        virtual ~SteadyCamera();

        virtual void update(Clock *clock, RenderWindow *window, World *world);
};

#endif // STEADYCAMERA_H
