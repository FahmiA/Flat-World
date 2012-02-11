#ifndef STAR_H
#define STAR_H

#include "actors/Character.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

/** A star that is found somewhere in a level for the player to capture. */
class Star : public Character
{
    public:
        Star(float x, float y, float width, float height, Sprite *sprite);
        virtual ~Star();

        void subUpdate(Clock *clock, RenderWindow *window, World *world);

    private:
        Sprite *sprite;
};

#endif // STAR_H
