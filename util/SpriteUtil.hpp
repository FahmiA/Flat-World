#ifndef SPRITEUTIL_H
#define SPRITEUTIL_H

#include <SFML/Graphics.hpp>
using namespace sf;

class SpriteUtil
{
    public:
        SpriteUtil();

         /** Performs a raytrace on a Sprite.
            sprite = sprite to perform raytrace on.
            fromX, fromY =  start position to raytrace from (clamped to sprite bounds).
            toX, toY = end position to raytrace to (clamped to sprite bounds).
            return = first position that is not transparent.*/
        Vector2f* rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY);
};

#endif // SPRITEUTIL_H
