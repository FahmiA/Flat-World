#include "ContentManager.hpp"

ContentManager::ContentManager()
{
    imageMap = new map<string, ImgTex>();
    fontMap = new map<string, Font*>();
}

ContentManager::~ContentManager()
{
    // CLear the contents
    clear();

    //Delete the data structures
    for(map<string, ImgTex>::iterator imgTex = imageMap->begin();
        imgTex != imageMap->end();
        imgTex++)
    {
        delete imgTex->second.image;
        delete imgTex->second.texture;
    }
    delete imageMap;
    delete fontMap;
}

ImgTex ContentManager::loadImage(string &path)
{
    ImgTex imgTex{0, 0, false};

    if(imageMap->count(path) > 0)
    {
        // The image is currently loaded. Pass an alias.
        imgTex = (*imageMap)[path];
    }else{
        // Load the image.
        Image *image = new Image();
        if(!image->loadFromFile(path))
        {
            Texture *texture = new Texture();
            if(texture->loadFromImage(*image))
            {
                imgTex.image = image;
                imgTex.texture = texture;
                imgTex.loadOK = true;
                (*imageMap)[path] = imgTex;
            }else{
                delete image;
            }
        }
    }

    return imgTex;
}

ImgTex ContentManager::loadImage(char *path)
{
    string pathString(path);
    return loadImage(pathString);
}

ImgTex ContentManager::loadImage(string &path, unsigned int transparentColour)
{
    ImgTex imgTex{0, 0, false};

    if(imageMap->count(path) > 0)
    {
        // The image is currently loaded. Pass an alias.
        imgTex = (*imageMap)[path];
    }else{
        // Load the image.
        Image *image = new Image();
        if(!image->loadFromFile(path))
        {
            Texture *texture = new Texture();
            if(texture->loadFromImage(*image))
            {
                // Set the transparency
                Color mask(0, 0, 0);
                mask.r = (transparentColour >> 16) & 0xFF;
                mask.g = (transparentColour >> 8) & 0xFF;
                mask.b = transparentColour & 0xFF;
                image->createMaskFromColor(mask);

                imgTex.image = image;
                imgTex.texture = texture;
                imgTex.loadOK = true;
                (*imageMap)[path] = imgTex;
            }else{
                delete image;
            }
        }
    }

    return imgTex;
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
    for(map<string, ImgTex>::iterator imgTex = imageMap->begin();
        imgTex != imageMap->end();
        imgTex++)
    {
        delete imgTex->second.image;
        delete imgTex->second.texture;
    }
    imageMap->clear();
    fontMap->clear();
}
