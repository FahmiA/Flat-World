#include "ContentManager.hpp"

ContentManager::ContentManager()
{
    imageMap = new map<string, Image*>();
    fontMap = new map<string, Font*>();
}

ContentManager::~ContentManager()
{
    // CLear the contents
    clear();

    //Delete the data structures
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
        if(!image->loadFromFile(path))
            return 0;

        (*imageMap)[path] = image;
        return image;
    }
}

Image* ContentManager::loadImage(string &path, unsigned int transparentColour)
{
    if(imageMap->count(path) > 0)
    {
        // The image is currently loaded. Pass an alias.
        return (*imageMap)[path];
    }else{
        // Load the image.
        Image *image = new Image();
        if(!image->loadFromFile(path))
            return 0;

        // Set the transparency
        Color mask(0, 0, 0);
        mask.r = (transparentColour >> 16) & 0xFF;
        mask.g = (transparentColour >> 8) & 0xFF;
        mask.b = transparentColour & 0xFF;
        image->createMaskFromColor(mask);

        (*imageMap)[path] = image;
        return image;
    }
}

Image* ContentManager::loadImage(char *path)
{
    string *pathString = new string(path);
    loadImage(*pathString);
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
        if(!font->loadFromFile(path))
            return 0;

        (*fontMap)[path] = font;
        return font;
    }
}

void ContentManager::clear()
{
    // Clearing a map will also delete all of the map's contents.
    imageMap->clear();
    fontMap->clear();
}
