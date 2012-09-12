#ifndef SPRITEUTIL_H
#define SPRITEUTIL_H

#include "gameIO/ContentManager.hpp"
#include "util/AnimatedSprite.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

#include <string>
using namespace std;

/** Utility methods for working with Sprite objects. */
class SpriteUtil
{
    public:

        /** Performs a raytrace on a Sprite.
         * @param image Sprite image to perform raytrace on.
         * @param fromX Start x-position to raytrace from (clamped to sprite bounds).
         * @param fromY Start y-position to raytrace from (clamped to sprite bounds).
         * @param toX End x-position to raytrace to (clamped to sprite bounds).
         * @param toY End y-position to raytrace to (clamped to sprite bounds).
         * @param seekEmpty True to find the first transparent pixel. False to find the first solid pixel.
         * @return First position that is not transparent (is solid).
         */
        static Vector2f* rayTrace(const Image &image, int fromX, int fromY, int toX, int toY, bool seekEmpty = true);

        //static void resize(Sprite *sprite, float width, float height);
        //static Vector2f& getSize(Sprite *sprite);

        /** Loads a Sprite image given a path to an image.*/
        //static AnimatedSprite* loadSprite(string &path, unsigned int tColour, ContentManager *content);

        /** Loads a Font.
         * @param path The path of the font to load.
         * @param font A reference to the Font to initialise.
         * @param content The ContentManager to manage the loaded font.
         * @return Tue if the font path pointed to a valid font. False otherwise.
         */
        static bool loadFont(string &path, Font *&font, ContentManager *content);
};

#endif // SPRITEUTIL_H
