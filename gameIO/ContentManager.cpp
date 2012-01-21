#include "ContentManager.hpp"

ContentManager::ContentManager()
{
    imageMap = new map<string, Image*>();
    fontMap = new map<string, Font*>();
}

ContentManager::~ContentManager()
{
    // Deleting a map will also delete all of the map's contents.
    delete imageMap;
    delete fontMap;
}

Image* ContentManager::loadImage(string &path)
{
    if(imageMap->count(path) > 0)
    {
        // The image is currently loaded. Pass an alias.
        return (*imageMap)[path];
    }else{
        // Load the image.
        Image *image = new Image();
        if(!image->LoadFromFile(path))
            return 0;

        (*imageMap)[path] = image;
        return image;
    }
}

Font* ContentManager::loadFont(string &path)
{
    if(fontMap->count(path) > 0)
    {
        // The font is currently loaded. Pass the alias.
        return (*fontMap)[path];
    }else{
        // Load the font.
        Font *font = new Font();
        if(!font->LoadFromFile(path))
            return 0;

        (*fontMap)[path] = font;
        return font;
    }
}
