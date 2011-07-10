#include "ContentManager.hpp"

ContentManager::ContentManager()
{
    imageMap = new map<string, Image*>();
}

ContentManager::~ContentManager()
{
    // Deleting the map will also delete all the map's contents.
    delete imageMap;
}

Image* ContentManager::loadImage(string path)
{
    if(imageMap->count(path) > 0)
    {
        // The resource is currently loaded. Pass an alias.
        return (*imageMap)[path];
    }else{
        // Load the resource.
        Image *image = new Image();
        if(!image->LoadFromFile(path))
            return 0;

        (*imageMap)[path] = image;
        return image;
    }
}
