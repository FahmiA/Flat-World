#include "gtest/gtest.h"

#include "util/CoordinateUtil.hpp"
#include "util/TSprite.hpp"
#include "gameIO/TSpriteLoader.hpp"
#include "actors/Sheep.hpp"

#include <math.h>

#define SQUARE_IMAGE_PATH "unit_test/util/filesForTests/square.png"

class CoordinateUtilTest : public ::testing::Test
{
    protected:

        CoordinateUtil *coordUtil;
        ContentManager *content;
        TSpriteLoader *spriteLoader;


        virtual void SetUp()
        {
            coordUtil = new CoordinateUtil();
            content = new ContentManager();
            spriteLoader = new TSpriteLoader(content);
        }

        virtual void TearDown()
        {
            delete coordUtil;
            delete content;
            delete spriteLoader;
        }
};

TEST_F(CoordinateUtilTest, isLocalPointInside)
{
    bool result;

    result = coordUtil->isLocalPointInside(Vector2f(10.0f, 10.0f), Vector2f(20, 30));
    EXPECT_TRUE(result) << "Inside point";

    result = coordUtil->isLocalPointInside(Vector2f(40.3f, 40.005f), Vector2f(2, 30));
    EXPECT_FALSE(result) << "Outside point";

    result = coordUtil->isLocalPointInside(Vector2f(19.8f, 38.0f), Vector2f(20, 30));
    EXPECT_FALSE(result) << "Near point";

    result = coordUtil->isLocalPointInside(Vector2f(20.0f, 30.0f), Vector2f(20, 30));
    EXPECT_FALSE(result) << "Edge point";

    result = coordUtil->isLocalPointInside(Vector2f(-1.0f, -3.0f), Vector2f(20, 30));
    EXPECT_FALSE(result) << "Negative point";

    result = coordUtil->isLocalPointInside(Vector2f(1081.0f, 581.0f), Vector2f(1172, 577));
    EXPECT_FALSE(result) << "Real test";
}

// TEST_F = Test with Ficture (CoordinateUtilTest is a test ficture)
TEST_F(CoordinateUtilTest, getDistance)
{
    float result;

    result = coordUtil->getDistance(Vector2f(5, 1), Vector2f(10, 1));
    EXPECT_FLOAT_EQ(5, result) << "Distance between x positions not correct";

    result = coordUtil->getDistance(Vector2f(5, 245), Vector2f(5, 246));
    EXPECT_FLOAT_EQ(1, result) << "Distance between y positions not correct";

    result = coordUtil->getDistance(Vector2f(-5, 1), Vector2f(5, -1));
    EXPECT_NEAR(10.198, result, 0.001) << "Distance with some negative values not correct";

    result = coordUtil->getDistance(Vector2f(5, 5), Vector2f(-10, -10));
    EXPECT_NEAR(21.21, result, 0.005) << "Should recieve absolute value of distance";

    result = coordUtil->getDistance(Vector2f(5, 5), Vector2f(5, 5));
    EXPECT_FLOAT_EQ(0, result) << "Distance of 0 expected with two identical coordinates";

    result = coordUtil->getDistance(Vector2f(0, 0), Vector2f(0, 0));
    EXPECT_FLOAT_EQ(0, result) << "Should handle origin coordinates correctly.";
}

TEST_F(CoordinateUtilTest, getAngle)
{
    float angle;

    angle = coordUtil->getAngle(Vector2f(0, 0), 0, Vector2f(10, 0));
    EXPECT_FLOAT_EQ(0,  angle);

    angle = coordUtil->getAngle(Vector2f(10, 10), 0, Vector2f(75, 2));
    EXPECT_NEAR(-0.122f, angle, 0.001);

    angle = coordUtil->getAngle(Vector2f(10, 10), 0.5, Vector2f(75, 2));
    EXPECT_NEAR(-0.622f, angle, 0.001);

    angle = coordUtil->getAngle(Vector2f(10, 10), -1.2, Vector2f(75, 2));
    EXPECT_NEAR(1.078f, angle, 0.001);
}

TEST_F(CoordinateUtilTest, isInFOVAcute)
{
    TSpriteLoader spriteLoader(content);
    TSprite *sourceTexture = spriteLoader.loadStatic(SQUARE_IMAGE_PATH);
    TSprite *targetTexture = spriteLoader.loadStatic(SQUARE_IMAGE_PATH);

    // Sprite default size is 20 * 20 pixels
    Sheep source(0, 0, 40, 40, 100, sourceTexture);
    Sheep target(0, 0, 20, 20, 100, targetTexture);

    source.setPosition(1000, 1000);
    int fovDistance = 500;
    float fovAngleR = 0.5; // radians
    bool result;

    // Test for inside FOV
    // Well inside FOV
    target.setPosition(600, 1000);
    result = coordUtil->isInFOV(source, target, Left, fovAngleR, fovDistance);
    ASSERT_TRUE(result) << "Target should be detected if it is within the FOV.";
    // Upper corner
    target.setPosition(500, 727);
    result = coordUtil->isInFOV(source, target, Left, fovAngleR, fovDistance);
    ASSERT_TRUE(result) << "Target should be detected if it borders the upper corner of the FOV.";
    // Upper edge
    target.setPosition(800, 891);
    result = coordUtil->isInFOV(source, target, Left, fovAngleR, fovDistance);
    ASSERT_TRUE(result) << "Target should be detected if it borders the upper edge of the FOV.";
    // Lower edge
    target.setPosition(800, 1109);
    result = coordUtil->isInFOV(source, target, Left, fovAngleR, fovDistance);
    ASSERT_TRUE(result) << "Target should be detected if it borders the lower edge of the FOV.";
    // Lower corner
    target.setPosition(500, 1273);
    result = coordUtil->isInFOV(source, target, Left, fovAngleR, fovDistance);
    ASSERT_TRUE(result) << "Target should be detected if it borders the lower corner of the FOV.";
    // Ontop of source
    result = coordUtil->isInFOV(source, source, Left, fovAngleR, fovDistance);
    ASSERT_TRUE(result) << "Target should be detected if it is on top of the source.";

    // Test for outside FOV
    // Infront and outside FOV
    target.setPosition(400, 400);
    result = coordUtil->isInFOV(source, target, Left, fovAngleR, fovDistance);
    ASSERT_FALSE(result) << "Target should not be detected if it is close but not in the FOV.";
    // Infront and outside FOV
    target.setPosition(200, 1000);
    result = coordUtil->isInFOV(source, target, Left, fovAngleR, fovDistance);
    ASSERT_FALSE(result) << "Target should not be detected if it is infront, but outside, the FOV";
    // Behind, but oitherwise within, FOV
    target.setPosition(1400, 1000);
    result = coordUtil->isInFOV(source, target, Left, fovAngleR, fovDistance);
    ASSERT_FALSE(result) << "Target should not be detected if it is behind, but otherwise inside, the FOV";
}

/*TEST_F(CoordinateUtilTest, isInFOVReflex)
{
    bool result;
    int lookDistance = 586;
    float fovAngle = 2.4; // radians

    // Test for inside FOV
    // Well inside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(491, 297), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it is within the FOV.";
    // Upper corner
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(152, 176), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the upper corner of the FOV.";
    // Upper edge
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(336, 96), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the upper edge of the FOV.";
    // Directly infront
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(586, 345), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the distant edge of the FOV.";
    // Lower edge
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(336, 595), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the lower edge of the FOV.";
    // Lower corner
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(152, 515), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the lower corner of the FOV.";
    // Ontop of source
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(336, 345), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it is on top of the source.";

    // Test for outside FOV
    // Infront and outside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(278, 337), lookDistance, fovAngle);
    EXPECT_FALSE(result) << "Target should not be detected if it is close but not in the FOV.";
    // Infront and outside FOV
    result = coordUtil->isInFOV(Vector2f(14, 5), 0, Vector2f(1105, 4), lookDistance, fovAngle);
    EXPECT_FALSE(result) << "Target should not be detected if it is infront, but outside, the FOV";
}

TEST_F(CoordinateUtilTest, isInFOVFull)
{
    bool result;
    int lookDistance = 586;
    float fovAngle = M_PI; // radians

    // Test for inside FOV
    // Well inside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(491, 297), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it is within the FOV.";
    // Border
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(86, 345), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it border of the FOV.";
}*/

TEST_F(CoordinateUtilTest, collideNoRotation)
{
    // Sprite default size is 20 * 20 pixels
    TSprite *spriteA = spriteLoader->loadStatic(SQUARE_IMAGE_PATH);
    spriteA->setOrigin(10, 10);
    TSprite *spriteB = spriteLoader->loadStatic(SQUARE_IMAGE_PATH);
    bool didCollide = true;

    // Check for no collision with gap
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(100, 100); // Width: 100 - 119, height: 100 - 119
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(false, didCollide) << "Check for no collision with gap";

    // Check for collision on touch
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(20, 0); // Width: 20 - 39, height: 0 - 19
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(true, didCollide) << "Check for collision on touch";

    // Check for collission with complete overlap
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(0, 0); // Width: 0 - 19, height: 0 - 19
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(true, didCollide) << "Check for collission with complete overlap";

    // Check for collission with partial overlap
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(10, 10); // Width: 10 - 29, height: 10 - 29
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(true, didCollide) << "Check for collission with partial overlap";

    // Check for collission with boundary overlap
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(19, 0); // Width: 19 - 28, height: 0 - 19
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(true, didCollide) << "Check for collission with boundary overlap";

    spriteA->setSize(40, 40); // Width: 40, height: 40
    spriteA->setPosition(20, 20); // Width: 0 - 39, height: 0 - 39
    spriteB->setPosition(39, 0); // Width: 39 - 59, height: 0 - 19
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(true, didCollide) << "Check with scale";

    delete spriteA;
    delete spriteB;
}

TEST_F(CoordinateUtilTest, collideWithRotation)
{
    // Sprite default size is 20 * 20 pixels
    TSprite *spriteA = spriteLoader->loadStatic(SQUARE_IMAGE_PATH);
    spriteA->setOrigin(10, 10);
    TSprite *spriteB = spriteLoader->loadStatic(SQUARE_IMAGE_PATH);
    bool didCollide = true;

    // Check for collision with one sprite rotated
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(20, 0); // Width: 20 - 39, height: 0 - 19
    spriteB->setRotation(M_PI / 4.0f);
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(true, didCollide) << "Check for collision with one sprite rotated";

    // Check for collision with two sprites rotated
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(28, 0); // Width: 27 - 36, height: 0 - 19
    spriteA->setRotation(M_PI / 4.0f);
    spriteB->setRotation(M_PI / 4.0f);
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(true, didCollide) << "Check for collision with two sprites rotated";

    // Check for no collision
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(30, 0); // Width: 30 - 49, height: 0 - 19
    spriteB->setRotation(M_PI / 4.0f);
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(false, didCollide) << "Check for no collision";
}
