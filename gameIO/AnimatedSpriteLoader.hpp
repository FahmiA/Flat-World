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

class AnimatedSpriteLoader
{
    public:
        AnimatedSpriteLoader(ContentManager *content);
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

