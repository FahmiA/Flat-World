#include "CoordinateUtil.hpp"

#include<math.h>
using namespace std;

CoordinateUtil::CoordinateUtil() {}

bool CoordinateUtil::isGlobalPointInside(Vector2f &point, Sprite &bounds)
{
    Vector2f rectSize = bounds.GetSize();
    Vector2f rectPosition = bounds.GetPosition();

    if(point.x >= rectPosition.x && point.x <= rectPosition.x + rectSize.x &&
       point.y >= rectPosition.y && point.y <= rectPosition.y + rectSize.y)
       return true;

    return false;
}

bool CoordinateUtil::isLocalPointInside(Vector2f &point, Sprite &bounds)
{
    Vector2f rectSize = bounds.GetSize();

    if(point.x >= 0 && point.x <= rectSize.x &&
       point.y >= 0 && point.y <= rectSize.y)
       return true;

    return false;
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
}

Vector2f* CoordinateUtil::getLineIntersect(Vector2f &line1p1, Vector2f &line1p2, Vector2f &line2p1, Vector2f &line2p2)
{
    // http://en.wikipedia.org/wiki/Line-line_intersection

    float intersect1 = ((((line1p1.x * line1p2.y) - (line1p1.y * line1p2.x)) * (line2p1.x - line2p2.x)) - ((line1p1.x - line1p2.x) * ((line2p1.x * line2p2.y) - (line2p1.y * line2p2.x)))) /
                        (((line1p1.x - line1p2.x) * (line2p1.y - line2p2.y)) - ((line1p1.y - line1p2.y) * (line2p1.x - line2p2.x)));

    float intersect2 = ((((line1p1.x * line1p2.y) - (line1p1.y * line1p2.x)) * (line2p1.y - line2p2.y)) - ((line1p1.y - line1p2.y) * ((line2p1.x * line2p2.y) - (line2p1.y * line2p2.x)))) /
                        (((line1p1.x - line1p2.x) * (line2p1.y - line2p2.y)) - ((line1p1.y - line1p2.y) * (line2p1.x - line2p2.x)));

    Vector2f *intersection = new Vector2f(intersect1, intersect2);
    return intersection;
}

float CoordinateUtil::getDistance(Vector2f &p1, Vector2f &p2)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float distance = (dx * dx) + (dy * dy);

    return distance;
}*/
