#ifndef CAMERA_H
#define CAMERA_H

class World;

#include <SFML/Graphics.hpp>
using namespace sf;

class Camera
{
    public:
        Camera();
        virtual ~Camera();

        virtual void update(Clock *clock, RenderWindow *window, World *world) = 0;
        virtual void draw(RenderWindow *window);

    protected:
        View* getView();

    private:
        View *view;

};

#endif // CAMERA_H
