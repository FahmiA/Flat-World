#ifndef CAMERA_H
#define CAMERA_H

class World;

#include <SFML/Graphics.hpp>
using namespace sf;

class Camera
{
    public:
        Camera(View *view);
        virtual ~Camera();

         /** Sets the camera as the active camera for the window.
         * @param window The window to change the camera on.
         */
        void activate(RenderWindow *window);

        virtual void update(Clock *clock, RenderWindow *window, World *world) = 0;
        virtual void draw(RenderWindow *window);

    protected:
        View* getView();

    private:
        View *view;

};

#endif // CAMERA_H
