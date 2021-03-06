#ifndef ISLAND_H
#define ISLAND_H

#include "game/GameObject.hpp"
#include "util/TSprite.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

/** Defines an island that Characters can move around on and jump between. */
class Island: public GameObject
{
    public:
        Island(float x, float y, float width, float height, TSprite *sprite);
        virtual ~Island();

        TSprite* getSprite();
        Image* getImage();

        void update(Clock *clock, RenderWindow *window, World *world);
        void draw(RenderWindow *window);

        const Vector2f& getPosition();
        void setPosition(float x, float y);
        const Vector2f& getSize();

    private:
        TSprite *sprite;
};

#endif // ISLAND_H
