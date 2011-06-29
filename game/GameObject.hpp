#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class World;

#include <SFML/Graphics.hpp>
using namespace sf;

class GameObject
{
    public:
        //GameObject(float x, float y, float width, float height);

        virtual void update(Clock *clock, RenderWindow *window, World *world) = 0;

        virtual void draw(RenderWindow *window) = 0;
};

#endif
