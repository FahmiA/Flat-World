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

/*bool CoordinateUtil::collide(Sprite *object1, Sprite *object2)
{
    float ob1OffsetX = object1->getOrigin().x * object1->getScale().x;
    float ob1OffsetY = object1->getOrigin().y * object1->getScale().y;
    float ob1Width = object1->GetSize().x;
    float ob1Height = object1->GetSize().y;

    Vector2f ob1TopLeft = Vector2(object1->getPosition().x - ob1OffsetX,
                                  object1->getPosition().y - ob1OffsetY);
    Vector2f ob1TopRight = Vector2(ob1TopLeft.x + ob1Width,
                                   ob1TopLeft.y);
    Vector2f ob1BottomLeft = Vector2(ob1TopLeft.x,
                                     ob1TopLeft.y + ob1Height);
    Vector2f ob1BottomRight = Vector2(ob1TopLeft.x + ob1Width,
                                      ob1TopLeft.y + ob1Height);

    float ob2OffsetX = object2->getOrigin().x * object2->getScale().x;
    float ob2OffsetY = object2->getOrigin().y * object2->getScale().y;
    float ob2Width = object2->GetSize().x;
    float ob2Height = object2->GetSize().y;

    Vector2f ob2TopLeft = Vector2(object2->getPosition().x - ob2OffsetX,
                                  object2->getPosition().y - ob2OffsetY);
    Vector2f ob2TopRight = Vector2(ob2TopLeft.x + ob2Width,
                                   ob2TopLeft.y);
    Vector2f ob2BottomLeft = Vector2(ob2TopLeft.x,
                                     ob2TopLeft.y + ob2Height);
    Vector2f ob2BottomRight = Vector2(ob2TopLeft.x + ob2Width,
                                      ob2TopLeft.y + ob2Height);

    bool hasCollided;

    // Check
    hasCollided = getLineIntersect()
}*/

bool CoordinateUtil::collide(Sprite *object1, Sprite *object2)
{
    // http://stackoverflow.com/questions/306316/determine-if-two-rectangles-overlap-each-other
    //TODO: Take rotation into account
    float ob1OffsetX = object1->getOrigin().x * object1->getScale().x;
    float ob1OffsetY = object1->getOrigin().y * object1->getScale().y;
    int ob1Width = SpriteUtil::getSize(object1).x;
    int ob1Height = SpriteUtil::getSize(object1).y;
    int ob1X = object1->getPosition().x - ob1OffsetX;
    int ob1Y = object1->getPosition().y - ob1OffsetY;

    float ob2OffsetX = object2->getOrigin().x * object2->getScale().x;
    float ob2OffsetY = object2->getOrigin().y * object2->getScale().y;
    int ob2Width = SpriteUtil::getSize(object2).x;
    int ob2Height = SpriteUtil::getSize(object2).y;
    int ob2X = object2->getPosition().x - ob2OffsetX;
    int ob2Y = object2->getPosition().y - ob2OffsetY;

    bool xOverlap = valueInRange(ob1X, ob2X, ob2X + ob2Width - 1) ||
                    valueInRange(ob2X, ob1X, ob1X + ob1Width - 1);
    bool yOverlap = valueInRange(ob1Y, ob2Y, ob2Y + ob2Height - 1) ||
                    valueInRange(ob2Y, ob1Y, ob1Y + ob1Height - 1);

    return xOverlap && yOverlap;
}

/*bool CoordinateUtil::collide(Sprite *object1, Sprite *object2)
{
    return Collision::BoundingBoxTest(*object1, *object2);
}*/

float CoordinateUtil::getDotProduct(const Vector2f &a, const Vector2f &b)
{
    return (a.x * b.x) + (a.y * b.y);
}

bool CoordinateUtil::isInFOV(Character &source, Character &target, Direction direction,
                             float fovAngleR, float fovDistance)
{
    bool insideFOV = false;

    // Get the Sprite positions
    Vector2f sourceCenter(source.getSize().x / 2, source.getSize().y / 2);
    Vector2f targetCenter(target.getSize().x / 2, target.getSize().y / 2);

    // Transform the target position into the source's coordinate space
    Transform targetGlobalTransform = target.getSprite()->getTransform();
    Transform sourceLocalTransform = source.getSprite()->getInverseTransform();
    targetCenter = targetGlobalTransform.transformPoint(targetCenter);
    targetCenter = sourceLocalTransform.transformPoint(targetCenter);

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

float CoordinateUtil::getDistance(Vector2f &p1, Vector2f &p2)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float distance = sqrt((dx * dx) + (dy * dy));

    return distance;
}
