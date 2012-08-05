#ifndef ANIMATEDSPRITELOADER_H
#define ANIMATEDSPRITELOADER_H

#include <string>
#include <map>
using namespace std;

#include "tinyxml/tinyxml.h"
#include "gameIO/ContentManager.hpp"
#include "util/AnimatedSprite.hpp"
#include "util/XmlUtil.hpp"
#include "util/SpriteUtil.hpp"

/** Loads animations from Sprite Vortex XML files. */
class AnimatedSpriteLoader
{
    public:
        /** Creates a new AnimationSpriteLoader.
          * @param content ContentManager for loading the sprite sheet.
          */
        AnimatedSpriteLoader(ContentManager *content);

        /** Loadsa a sprite.
          * @path Path to XML file.
          */
        AnimatedSprite* loadFromXML(string &path);

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

