#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <SFML/Graphics.hpp>
using namespace sf;

#include <string>
#include <map>
using namespace std;

/** A class for managing the creation and destruction of external resources.
 * If a resource is requested that has not been loaded before, the new resource will be loaded and sent.
 * If a resource is requested which has been loaded before, an ALIASED (duplicate) pointer will be sent.
 *
 * All resources loaded will be destroyed when the destructor is called.
*/
class ContentManager
{
    public:
        /** Creates a new ContentManager. */
        ContentManager();
        virtual ~ContentManager();

        /** Loads or retrieves an image.
         * NULL is returned if the image could not be loaded.
         * @param path The path to the image to load.
         * @return The loaded image or NULL if the image could not be loaded.
        */
        Image* loadImage(string &path);

        /** Loads or retrieves an image.
         * NULL is returned if the image could not be loaded.
         * @param path The path to the image to load.
         * @param transparentColour Make this colour transparent.
         * @return The loaded image or NULL if the image could not be loaded.
        */
        Image* loadImage(string &path, unsigned int transparentColour);

         /** Loads or retrieves an image.
         * NULL is returned if the image could not be loaded.
         * @param path The path to the image to load.
         * @return The loaded image or NULL if the image could not be loaded.
        */
        Image* loadImage(char *path);

        /** Loads or retrieves a font.
         * NULL is returned if the font could not be loaded.
         * @param path The path to the font to load.
         * @return The loaded font or NULL if the font could not be loaded.
        */
        Font* loadFont(string &path);

        /** Frees all loaded resources from memory.
         * Is always called by the destructor.
         */
        void clear();

    private:
        map<string, Image*> *imageMap;
        map<string, Font*> *fontMap;

};

#endif // CONTENTMANAGER_H
