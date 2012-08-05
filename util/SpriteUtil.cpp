#include "SpriteUtil.hpp"

#include "NumberUtil.hpp"

SpriteUtil::SpriteUtil() {}

Vector2f* SpriteUtil::rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY, bool seekEmpty)
{
    // Simplified Bresenham line algorithm
    // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    // x0 = fromX   y0 = fromY  x1 = toX    y1 = toY

    /* Note: If ray trace path is out of bounds, it will still be computed.
       This is expensive as it is computing extra pixels, but I don't think
       it's much cheaper to find the intersection of the ray trace path
       with the rectangle to clamp and reduce line length.*/

    // Clamp positions to within the bounds of the sprite
    int spriteMaxX = (int)((sprite->GetSize().x / sprite->GetScale().x) - 1.0f);
    int spriteMaxY = (int)((sprite->GetSize().y / sprite->GetScale().y) - 1.0f);

    // Declare the required variables
    Vector2f *collidePosition = 0;
    int differenceX = abs(toX - fromX);
    int differenceY = abs(toY - fromY);
    int error = differenceX - differenceY;
    int slopeX = (fromX < toX) ? 1 : -1;
    int slopeY = (fromY < toY) ? 1 : -1;
    int error2 = 0;
    Color pixel; // Pixel to look at
    bool hasPixel;

    while(true)
    {
        // Check pixel (fromX, fromY)
        if(fromX >= 0 && fromX <= spriteMaxX &&
           fromY >= 0 && fromY <= spriteMaxY)
        {
            pixel = sprite->GetPixel(fromX, fromY);
            hasPixel = true;
        }else{
            hasPixel = false;
        }

        if(hasPixel)
        {
            if((seekEmpty && pixel.a > 0) || // Solid
               (!seekEmpty && pixel.a == 0)) // Transparent
            {
                collidePosition = new Vector2f(fromX, fromY);
                break;
            }
        }

        if(fromX == toX && fromY == toY)
            break;

        error2 = 2 * error;
        if(error2 > -differenceY)
        {
            error = error - differenceY;
            fromX = fromX + slopeX;
        }

        if(error2 < differenceX)
        {
            error = error + differenceX;
            fromY = fromY + slopeY;
        }
    }

    return collidePosition;
}

bool SpriteUtil::loadSprite(string &path, Sprite *sprite, ContentManager *content)
{
    Image *image = content->loadImage(path);
    if(image == 0)
        return false;

    sprite->SetImage(*image);

    return true;
}

bool SpriteUtil::loadFont(string &path, Font *&font, ContentManager *content)
{
    font = content->loadFont(path);

    if(font == 0)
        return false;

    return true;
}
