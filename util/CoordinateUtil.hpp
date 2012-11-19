#ifndef COORDINATE_UTIL_H
#define COORDINATE_UTIL_H

#include <SFML/Graphics.hpp>
using namespace sf;

#include "util/TSprite.hpp"
#include "util/Commons.hpp"

class Character;

/** Utility methods for working with coordinates. */
class CoordinateUtil
{
    public:
        /** Creates a new CoordinateUtil object. */
        CoordinateUtil();

        /** Checks if a point in the global space is inside a given Sprite.
         * @param point The position in the global space.
         * @param bounds The Sprite to check if the global point is inside,
         * @return True if global point is inside bounds. False otherwise.
         */
        bool isGlobalPointInside(Vector2f &point, TSprite &bounds);

         /** Checks if a point in a Sprite's local space is inside the given Sprite.
         * @param point The position in the Sprite's local space.
         * @param bounds The bounds to check if the local point is inside,
         * @return True if local point is inside bounds. False otherwise.
         */
        bool isLocalPointInside(Vector2f point, Vector2f bounds);

        /** Gets the Euclidian distance between two positions.
         * @param p1 The first position.
         * @param p2 The second position.
         * @return The absolute distance between the first and second positions.
         */
        float getDistance(const Vector2f &p1, const Vector2f &p2);

        /** Gets the angle in radians from one position to another.
         * @param sourcePos The origin to measure the angle from.
         * @param sourceAngle The angle in which the sourcePos is rotated to (use 0 for horizontal axis).
         * @param targetPos The position to measure the angle to.
         * @return The angle form sourcePos to targetPos in radians.
         */
        float getAngle(const Vector2f &sourcePos, float sourceAngle, const Vector2f &targetPos);

        /** Checks if two Sprite objects are collidiing with each other.
         * @param object1 The first Sprite object.
         * @param object2 The second Sprite object.
         * @return True if object1 and object2 overlap. False otherwise.
         */
        bool collide(TSprite *object1, TSprite *object2);

        /** Checks if a target object is in the area bound by the source object's field of view (FOV).
         * @return True if target isn in source's FOV. False otherwise
         */
        bool isInFOV(Character &source, Character &target, Direction direction,
                             float fovAngleR, float fovDistance);
        //void clampCoordinates(Vector2f &origin, Vector2f &target, Sprite *bounds);

        //Vector2f& getLineIntersect(Vector2f &line1p1, Vector2f &line1p2, Vector2f &line2p1, Vector2f &line2p2);
};

#endif // COORDINATE_UTIL_H
