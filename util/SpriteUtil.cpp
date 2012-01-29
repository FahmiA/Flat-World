#include "SpriteUtil.hpp"

#include "NumberUtil.hpp"

SpriteUtil::SpriteUtil() {}

Vector2f* SpriteUtil::rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY)
{
    // Simplified Bresenham line algorithm
    // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    // x0 = fromX   y0 = fromY  x1 = toX    y1 = toY

    // Clamp positions to within the bounds of the sprite
    clamp(fromX, 0, sprite->GetSize().x - 1);
    clamp(fromY, 0, sprite->GetSize().y - 1);
    clamp(toX, 0, sprite->GetSize().x - 1);
    clamp(toY, 0, sprite->GetSize().y - 1);

    // Declare the required variables
    Vector2f *collidePosition = 0;
    int differenceX = abs(toX - fromX);
    int differenceY = abs(toY - fromY);
    int error = differenceX - differenceY;
    int slopeX = (fromX < toX) ? 1 : -1;
    int slopeY = (fromY < toY) ? 1 : -1;
    int error2 = 0;

    while(true)
    {
        // Check pixel (fromX, fromY)
        Color pixel = sprite->GetPixel(fromX, fromY);

        if(pixel.a > 0) // Not transparent
        {
            collidePosition = new Vector2f(fromX, fromY);
            break;
        }

        if(fromX == toX && fromY == toY)
            break;

        error2 = 2 * error;
        if(error2 > -differenceY)
        {
            error = error - differenceY;
            fromX = fromX + slopeX;
        }

        if(error2 < differenceX){
            error = error + differenceX;
            fromY = fromY + slopeY;
        }
    }

    return collidePosition;
}
