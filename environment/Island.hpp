#ifndef ISLAND_H
#define ISLAND_H

#include "../game/GameObject.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

class Island: public GameObject
{
    public:
        Island(float x, float y, float width, float height, Sprite *sprite);
        virtual ~Island();

        void update(Clock *clock, RenderWindow *window);
        void draw(RenderWindow *window);

    private:
        Sprite *sprite;
};

#endif // ISLAND_H