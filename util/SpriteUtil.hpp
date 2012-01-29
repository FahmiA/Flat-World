#ifndef SPRITEUTIL_H
#define SPRITEUTIL_H

#include <SFML/Graphics.hpp>
using namespace sf;

class SpriteUtil
{
    public:
        SpriteUtil();

        /** Performs a raytrace on a Sprite.
         * @param sprite Sprite to perform raytrace on.
         * @param fromX Start x-position to raytrace from (clamped to sprite bounds).
         * @param fromY Start y-position to raytrace from (clamped to sprite bounds).
         * @param toX End x-position to raytrace to (clamped to sprite bounds).
         * @param toY End y-position to raytrace to (clamped to sprite bounds).
         * @return First position that is not transparent (is solid).
         */
        Vector2f* rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY);
};

#endif // SPRITEUTIL_H
