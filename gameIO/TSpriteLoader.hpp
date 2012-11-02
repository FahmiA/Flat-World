#ifndef TSpriteLOADER_H
#define TSpriteLOADER_H

#include <string>
#include <map>
using namespace std;

#include "tinyxml/tinyxml.h"
#include "gameIO/ContentManager.hpp"
#include "util/TSprite.hpp"
#include "util/XmlUtil.hpp"
#include "util/SpriteUtil.hpp"

/** Loads animations from Sprite Vortex XML files. */
class TSpriteLoader
{
    public:
        /** Creates a new AnimationSpriteLoader.
          * @param content ContentManager for loading the sprite sheet.
          */
        TSpriteLoader(ContentManager *content);

        /** Loads a sprite.
          * @path Path to XML file.
          */
        TSprite* loadAnimated(string xmlPath);

        TSprite* loadStatic(string imagePath);

    private:
        ContentManager *content;

        string getImagePath(TiXmlHandle &root);
        unsigned int getTransparentColour(TiXmlHandle &root);
        AnimationFrame* getFrame(TiXmlHandle &spriteRoot);
        Animation* getAnimation(TiXmlHandle &animationRoot, map<int, AnimationFrame*> frames);

        XMLUtil xmlUtil;
        SpriteUtil spriteUtil;
};

#endif

