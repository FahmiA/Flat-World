#ifndef MANUALCAMERA_H
#define MANUALCAMERA_H

#include "Camera.hpp"

/** A camera that is controlled by the user.
 * Controls: Up, Down, Left, and Right keys.
 */
class ManualCamera: public Camera
{
    public:
        ManualCamera(View *view);
        virtual ~ManualCamera();

        virtual void update(Clock *clock, RenderWindow *window, World *world);

    private:
        float speed;
};

#endif // MANUALCAMERA_H
