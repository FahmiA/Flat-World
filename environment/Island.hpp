#ifndef ISLAND_H
#define ISLAND_H

#include "game/GameObject.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

class Island: public GameObject
{
    public:
        Island(float x, float y, float width, float height, Sprite *sprite);
        virtual ~Island();

        Sprite* getSprite();

        void update(Clock *clock, RenderWindow *window, World *world);
        void draw(RenderWindow *window);
        const Vector2f& getPosition();
        const Vector2f& getSize();

    private:
        Sprite *sprite;
};

#endif // ISLAND_H
