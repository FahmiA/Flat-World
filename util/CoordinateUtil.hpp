#ifndef COORDINATE_UTIL_H
#define COORDINATES_UTIL_H

#include "../game/GameObject.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

class CoordinateUtil
{
    public:
        CoordinateUtil();

        bool isGlobalPointInside(Vector2f &point, Sprite &bounds);
        bool isLocalPointInside(Vector2f &point, Sprite &bounds);

        float getDistance(const Vector2f &p1, const Vector2f &p2);

        float getAngle(const Vector2f &sourcePos, float sourceAngle, const Vector2f &targetPos);

        bool collide(Sprite *object1, Sprite *object2);

        /** Checks if a target object is in the triangle bound by the source object's field of view (FOV).
         * @param source Position of the source object (the "looker")
         * @param sourceAngle Angle relative to the horizontal axis of the source object (the "looker")
         * @param target Position of the target object (the one being looked at)
         * @param lookDistance How far the source object can see
         * @param fovAngle The angle of the source's FOV (in radians)
         * @return True if target isn in source's FOV. False otherwise
         */
        bool isInFOV(const Vector2f &source, float sourceAngle, const Vector2f &target,
                     int lookDistance, float fovAngle);

        //void clampCoordinates(Vector2f &origin, Vector2f &target, Sprite *bounds);

        //Vector2f& getLineIntersect(Vector2f &line1p1, Vector2f &line1p2, Vector2f &line2p1, Vector2f &line2p2);

        //float getDistance(Vector2f &p1, Vector2f &p2);

    private:
};

#endif // COORDINATE_UTIL_H
