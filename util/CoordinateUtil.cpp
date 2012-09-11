#include "CoordinateUtil.hpp"

#include "actors/Character.hpp"
#include "util/Collision.hpp"
#include "NumberUtil.hpp"
#include "SpriteUtil.hpp"

#include <math.h>
#include <iostream>
using namespace std;

CoordinateUtil::CoordinateUtil() {}

bool CoordinateUtil::isGlobalPointInside(Vector2f &point, Sprite &bounds)
{
    Vector2f rectSize = SpriteUtil::getSize(&bounds);
    Vector2f rectPosition = bounds.getPosition();

    if(point.x >= rectPosition.x && point.x < rectPosition.x + rectSize.x &&
       point.y >= rectPosition.y && point.y < rectPosition.y + rectSize.y)
       return true;

    return false;
}

bool CoordinateUtil::isLocalPointInside(Vector2f &point, Sprite &bounds)
{
    Vector2f rectSize = SpriteUtil::getSize(&bounds);
    // Consider for scaling
    float rectSizeX = rectSize.x / bounds.getScale().x;
    float rectSizeY = rectSize.y / bounds.getScale().y;

    if(point.x >= 0 && point.x < rectSizeX &&
       point.y >= 0 && point.y < rectSizeY)
       return true;

    return false;
}

float CoordinateUtil::getDistance(const Vector2f &p1, const Vector2f &p2)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float distance = (dx * dx) + (dy * dy);
    distance = sqrt(distance);

    return distance;
}

float CoordinateUtil::getAngle(const Vector2f &sourcePos, float sourceAngle, const Vector2f &targetPos)
{
    // Get the angle relative to the horizontal axis
    float angle = atan2(targetPos.y - sourcePos.y, targetPos.x - sourcePos.x);
    // Augment the angle based on the source's angle
    angle -= sourceAngle;
    return angle;
}

bool CoordinateUtil::collide(Sprite *object1, Sprite *object2)
{
    return Collision::BoundingBoxTest(*object1, *object2);
}

bool CoordinateUtil::isInFOV(Character &source, Character &target, Direction direction,
                             float fovAngleR, float fovDistance)
{
    // Get the Sprite positions
    Vector2f sourceCenter(source.getSize().x / 2, source.getSize().y / 2);
    Vector2f targetCenter(target.getSize().x / 2, target.getSize().y / 2);

    // Transform the target position into the source's coordinate space
    Transform targetGlobalTransform = target.getSprite()->getTransform();
    Transform sourceLocalTransform = source.getSprite()->getInverseTransform();
    targetCenter = targetGlobalTransform.transformPoint(targetCenter);
    targetCenter = sourceLocalTransform.transformPoint(targetCenter);

    // Perform the FOV check
    bool insideFOV = false;
    float distance = getDistance(sourceCenter, targetCenter);
    if(distance < fovDistance)
    {
        // Check the angle between the source and the target
        float angleR = atan2(targetCenter.y - sourceCenter.y, targetCenter.x - sourceCenter.x);

        // TODO: Only works if source if facing left
        if(direction == Left)
        {
            if(angleR > -M_PI - fovAngleR && angleR < -M_PI + fovAngleR)
                insideFOV = true;
        }else{ // direction == Right
            if(angleR > -fovAngleR && angleR < fovAngleR)
                insideFOV = true;
        }

    }

    return insideFOV;
}

/*void CoordinateUtil::clampCoordinates(Vector2f &origin, Vector2f &target, Sprite *bounds)
{
    // Check if the points are inside the bounds
    Vector2f originPoint(0, 0);
    bool originInside = isPointInside(originPoint, bounds->GetSize(), origin);
    bool targetInside = isPointInside(originPoint, bounds->GetSize(), target);

    //float lookAngle = atan2(target.y - origin.y, target.x - origin.x);
    // Initialize some variables
    Vector2f boundsTopLeft(0, 0);
    Vector2f boundsTopRight(bounds->GetSize().x, 0);
    Vector2f boundsBottomLeft(0, bounds->GetSize().y);
    Vector2f boundsBottomRight(bounds->GetSize().x, bounds->GetSize().y);

    if(!originInside)
    {
        // Get the intercections with the boundries of the rectangle.
        Vector2f *topIntersect = getLineIntersect(origin, target, boundsTopLeft, boundsTopRight);
        Vector2f *bottomIntersect = getLineIntersect(origin, target, boundsBottomLeft, boundsBottomRight);
        Vector2f *leftIntersect = getLineIntersect(origin, target, boundsTopLeft, boundsBottomLeft);
        Vector2f *rightIntersect = getLineIntersect(origin, target, boundsTopRight, boundsBottomRight);
        Vector2f *intersects[4] = {topIntersect, bottomIntersect, leftIntersect, rightIntersect};

        // Get the closest line
        Vector2d *closestIntersect = 0;
        float closestIntersectDist = 0;
        bool hasClosest = false;
        for(int i = 0; i < intersects.length; i++)
        {
            if(intersects[i] != 0)
            {
                float dist = getDistance(origin, intersects[i]);
                if(!hasClosest || dist < closestIntersectDist)
                {
                    closestIntersect = intersects[i];
                    hasClosest = true;
                    closestIntersectDist = dist;
                }
            }
        }

        // Clamp the origin
        origin.x = closestIntersect.x;
        origin.y = closestIntersect.y;
    }

    if(!targetInside)
    {

    }
}*/

/*Vector2f* CoordinateUtil::getLineIntersect(Vector2f &line1p1, Vector2f &line1p2, Vector2f &line2p1, Vector2f &line2p2)
{
    // http://en.wikipedia.org/wiki/Line-line_intersection

    float intersect1 = ((((line1p1.x * line1p2.y) - (line1p1.y * line1p2.x)) * (line2p1.x - line2p2.x)) - ((line1p1.x - line1p2.x) * ((line2p1.x * line2p2.y) - (line2p1.y * line2p2.x)))) /
                        (((line1p1.x - line1p2.x) * (line2p1.y - line2p2.y)) - ((line1p1.y - line1p2.y) * (line2p1.x - line2p2.x)));

    float intersect2 = ((((line1p1.x * line1p2.y) - (line1p1.y * line1p2.x)) * (line2p1.y - line2p2.y)) - ((line1p1.y - line1p2.y) * ((line2p1.x * line2p2.y) - (line2p1.y * line2p2.x)))) /
                        (((line1p1.x - line1p2.x) * (line2p1.y - line2p2.y)) - ((line1p1.y - line1p2.y) * (line2p1.x - line2p2.x)));

    Vector2f *intersection = new Vector2f(intersect1, intersect2);
    return intersection;
}*/
