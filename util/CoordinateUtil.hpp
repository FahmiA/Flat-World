#ifndef COORDINATE_UTIL_H
#define COORDINATES_UTIL_H

#include <SFML/Graphics.hpp>
using namespace sf;

class CoordinateUtil
{
    public:
        CoordinateUtil();

        bool isGlobalPointInside(Vector2f &point, Sprite &bounds);
        bool isLocalPointInside(Vector2f &point, Sprite &bounds);

        //void clampCoordinates(Vector2f &origin, Vector2f &target, Sprite *bounds);

        //Vector2f& getLineIntersect(Vector2f &line1p1, Vector2f &line1p2, Vector2f &line2p1, Vector2f &line2p2);

        //float getDistance(Vector2f &p1, Vector2f &p2);

    private:
};

#endif // COORDINATE_UTIL_H
