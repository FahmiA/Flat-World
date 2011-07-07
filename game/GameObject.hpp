#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class World;

#include <SFML/Graphics.hpp>
using namespace sf;

class GameObject
{
    public:

        virtual void update(Clock *clock, RenderWindow *window, World *world) = 0;

        virtual void draw(RenderWindow *window) = 0;

        virtual const Vector2f& getPosition() = 0;

        virtual const Vector2f& getSize() = 0;
};

#endif // GAMEOBJECT_H
