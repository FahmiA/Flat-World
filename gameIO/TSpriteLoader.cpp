#include "TSpriteLoader.hpp"

#include <iostream>

TSpriteLoader::TSpriteLoader(ContentManager *content)
{
    this->content = content;
}

TSprite* TSpriteLoader::loadAnimated(string xmlPath)
{
    // Load the XMl document
    TiXmlDocument aniDoc(xmlPath);
    bool loadOK = aniDoc.LoadFile();
    if(!loadOK)
        return 0;

    TiXmlHandle aniHandler(&aniDoc);
    TiXmlHandle root = TiXmlHandle(aniDoc.FirstChildElement("SpriteMapping"));

    string imagePath = getImagePath(root);
    // Get the full image path
    imagePath = xmlPath.substr(0, xmlPath.find_last_of("/") + 1) + imagePath;
    unsigned int transparentColour = getTransparentColour(root);

    Image *aniImage = content->loadImage(imagePath, transparentColour);
    if(aniImage == 0)
        return 0;

    // Build the animated sprite
    TSprite *tSprite = new TSprite(*aniImage);

    // Read all animation frames
    TiXmlHandle spritesRoot = TiXmlHandle(root.FirstChildElement("Sprites"));
    map<int, AnimationFrame*> frames; // Map from frame Ids to frames.
    int spriteIndex = 0;
    TiXmlHandle spriteRoot = spritesRoot.ChildElement(spriteIndex);
    while(spriteRoot.ToElement() != 0)
    {
        AnimationFrame *frame = getFrame(spriteRoot);
        frames[frame->id] = frame;

        spriteRoot = spritesRoot.ChildElement(++spriteIndex);
    }

    // Build animations from animation frames
    TiXmlHandle animationsRoot = TiXmlHandle(root.FirstChildElement("Animations"));
    int animationIndex = 0;
    TiXmlHandle animationRoot = animationsRoot.ChildElement(animationIndex);
    while(animationRoot.ToElement() != 0)
    {
        Animation *animation = getAnimation(animationRoot, frames);
        tSprite->addAnimation(animation);

        animationRoot = animationsRoot.ChildElement(++animationIndex);
    }

    // Return the completed animated sprite
    return tSprite;
}

TSprite* TSpriteLoader::loadStatic(string imagePath)
{
    Image *image = content->loadImage(imagePath);
    if(image == 0)
        return false;

    TSprite *tSprite = new TSprite(*image);

    return tSprite;
}

string TSpriteLoader::getImagePath(TiXmlHandle &root)
{
    string imagePath = "";

    TiXmlElement *textureElement = root.FirstChildElement("Texture").ToElement();
    if(textureElement != 0)
        imagePath = textureElement->Attribute("Path");

    return imagePath;
}

unsigned int TSpriteLoader::getTransparentColour(TiXmlHandle &root)
{
    int transparentColour = 0x00000000;

    TiXmlElement *tColourElement = root.FirstChildElement("TransparentColors")
                                       .FirstChildElement("Color").ToElement();
    if(tColourElement != 0)
        tColourElement->QueryIntAttribute("Value", &transparentColour);

    return (unsigned int)transparentColour;
}

AnimationFrame* TSpriteLoader::getFrame(TiXmlHandle &spriteRoot)
{
    AnimationFrame *frame = new AnimationFrame;

    // Get the frame metadata
    TiXmlElement *spriteElement = spriteRoot.ToElement();
    spriteElement->QueryStringAttribute("Name", &frame->name);
    spriteElement->QueryIntAttribute("Id", &frame->id);

    // Get the frame coordinates
    TiXmlHandle coordElement = spriteRoot.FirstChildElement("Coordinates");

    frame->x = xmlUtil.getInt(coordElement, "X");
    frame->y = xmlUtil.getInt(coordElement, "Y");
    frame->width = xmlUtil.getInt(coordElement, "Width");
    frame->height = xmlUtil.getInt(coordElement, "Height");

    return frame;
}

Animation* TSpriteLoader::getAnimation(TiXmlHandle &animationRoot, map<int, AnimationFrame*> frames)
{
    Animation *animation = new Animation;

    // Get the animation metadata
    TiXmlElement *animationElement = animationRoot.ToElement();
    string result;
    animationElement->QueryStringAttribute("Name", &animation->name);
    animationElement->QueryIntAttribute("FrameRate", &animation->frameRate);
    animationElement->QueryStringAttribute("Loop", &result);
    animation->loop = (result == "True") ? true : false;
    animationElement->QueryStringAttribute("PingPong", &result);
    animation->pingPong = (result == "True") ? true : false;

    // Get the animation frames
    TiXmlHandle framesElement = animationRoot.FirstChildElement("Frames");
    int id = -1;
    for(int i = 0; framesElement.Child(i).ToElement() != 0; i++)
    {
        TiXmlElement *frame = framesElement.ChildElement(i).ToElement();
        frame->QueryIntAttribute("SpriteId", &id);

        if(frames.count(id))
        {
            animation->frames.push_back(frames[id]);
        }else{
            cout << "Could not load sprite " << id << '.' << endl;
        }
    }

    return animation;
}

