#include "CoordinateUtil.hpp"

#include "NumberUtil.hpp"

#include<math.h>
using namespace std;

CoordinateUtil::CoordinateUtil() {}

bool CoordinateUtil::isGlobalPointInside(Vector2f &point, Sprite &bounds)
{
    Vector2f rectSize = bounds.GetSize();
    Vector2f rectPosition = bounds.GetPosition();

    if(point.x >= rectPosition.x && point.x < rectPosition.x + rectSize.x &&
       point.y >= rectPosition.y && point.y < rectPosition.y + rectSize.y)
       return true;

    return false;
}

bool CoordinateUtil::isLocalPointInside(Vector2f &point, Sprite &bounds)
{
    Vector2f rectSize = bounds.GetSize();
    // Consider for scaling
    float rectSizeX = rectSize.x / bounds.GetScale().x;
    float rectSizeY = rectSize.y / bounds.GetScale().y;

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
    angle += sourceAngle;
    return angle;
}

bool CoordinateUtil::collide(Sprite *object1, Sprite *object2)
{
    // http://stackoverflow.com/questions/306316/determine-if-two-rectangles-overlap-each-other
    //TODO: Take rotation into account
    float ob1OffsetX = object1->GetCenter().x * object1->GetScale().x;
    float ob1OffsetY = object1->GetCenter().y * object1->GetScale().y;
    int ob1Width = object1->GetSize().x;
    int ob1Height = object1->GetSize().y;
    int ob1X = object1->GetPosition().x - ob1OffsetX;
    int ob1Y = object1->GetPosition().y - ob1OffsetY;

    float ob2OffsetX = object2->GetCenter().x * object2->GetScale().x;
    float ob2OffsetY = object2->GetCenter().y * object2->GetScale().y;
    int ob2Width = object2->GetSize().x;
    int ob2Height = object2->GetSize().y;
    int ob2X = object2->GetPosition().x - ob2OffsetX;
    int ob2Y = object2->GetPosition().y - ob2OffsetY;

    bool xOverlap = valueInRange(ob1X, ob2X, ob2X + ob2Width) ||
                    valueInRange(ob2X, ob1X, ob1X + ob1Width);
    bool yOverlap = valueInRange(ob1Y, ob2Y, ob2Y + ob2Height) ||
                    valueInRange(ob2Y, ob1Y, ob1Y + ob1Height);

    return xOverlap && yOverlap;
}

bool CoordinateUtil::isInFOV(const Vector2f &source, float sourceAngle, const Vector2f &target,
                             int lookDistance, float fovAngle)
{
    float distance = getDistance(source, target);
    bool insideFOV = false;

    // Check that target is within 'seeing distance' of source
    if(distance < lookDistance)
    {
        float angle = getAngle(source, sourceAngle, target);
        if(angle <= fovAngle && angle >= -fovAngle)
        {
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
