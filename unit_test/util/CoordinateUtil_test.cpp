#include "gtest/gtest.h"

#include "util/CoordinateUtil.hpp"

#include <math.h>

#define SQUARE_IMAGE_PATH "unit_test/util/filesForTests/square.png"

class CoordinateUtilTest : public ::testing::Test
{
    protected:

        CoordinateUtil *coordUtil;

        virtual void SetUp()
        {
            coordUtil = new CoordinateUtil();
        }

        virtual void TearDown()
        {
            delete coordUtil;
        }
};

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

/*TEST_F(CoordinateUtilTest, isInFOVAcute)
{
    Texture texture;
    if(!texture.loadFromFile(SQUARE_IMAGE_PATH))
        FAIL() << "Image could not be loaded for test: " << SQUARE_IMAGE_PATH;
    // Sprite default size is 20 * 20 pixels
    Character source(texture);
    source.setScale(2.0f, 2.0f);
    source.setOrigin(10, 10);
    Sprite target(texture);
    target.setScale(0.5f, 0.5f);
    source.setOrigin(10, 10);

    int fovDistance = 586;
    float fovAngleR = 0.5; // radians
    bool result;

    // Test for inside FOV
    // Well inside FOV
    source.setPosition(336, 345);
    target.setPosition(491, 297);
    result = coordUtil->isInFOV(source, target, Left, fovDistance, fovAngleR);
    EXPECT_TRUE(result) << "Target should be detected if it is within the FOV.";
    // Upper corner
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(556, 226), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the upper corner of the FOV.";
    // Upper edge
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(575, 271), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the upper edge of the FOV.";
    // Directly infront
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(586, 345), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the distant edge of the FOV.";
    // Lower edge
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(575, 415), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the lower edge of the FOV.";
    // Lower corner
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(556, 462), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it borders the lower corner of the FOV.";
    // Ontop of source
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(336, 345), lookDistance, fovAngle);
    EXPECT_TRUE(result) << "Target should be detected if it is on top of the source.";

    // Test for outside FOV
    // Infront and outside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(243, 334), lookDistance, fovAngle);
    EXPECT_FALSE(result) << "Target should not be detected if it is close but not in the FOV.";
    // Infront and outside FOV
    result = coordUtil->isInFOV(Vector2f(14, 5), 0, Vector2f(1000, 4), lookDistance, fovAngle);
    EXPECT_FALSE(result) << "Target should not be detected if it is infront, but outside, the FOV";
    // Behind, but oitherwise within, FOV
    result = coordUtil->isInFOV(Vector2f(-14, -5), 0, Vector2f(-90, -6), lookDistance, fovAngle);
    EXPECT_FALSE(result) << "Target should not be detected if it is behind, but otherwise inside, the FOV";
}

TEST_F(CoordinateUtilTest, isInFOVReflex)
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
    Texture texture;
    if(!texture.loadFromFile(SQUARE_IMAGE_PATH))
        FAIL() << "Image could not be loaded for test: " << SQUARE_IMAGE_PATH;
    // Sprite default size is 20 * 20 pixels
    Sprite *spriteA = new Sprite(texture);
    spriteA->setOrigin(10, 10);
    Sprite *spriteB = new Sprite(texture);
    bool didCollide = true;

    // Check for no collision with gap
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(100, 100); // Width: 100 - 119, height: 100 - 119
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(false, didCollide) << "Check for no collision with gap";

    // Check for no collision without gap
    spriteA->setPosition(10, 10); // Width: 0 - 19, height: 0 - 19
    spriteB->setPosition(20, 0); // Width: 20 - 39, height: 0 - 19
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(false, didCollide) << "Check for no collision without gap";

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

    // Check with scale
    spriteA->setScale(2, 2); // Width: 40, height: 40
    spriteA->setPosition(20, 20); // Width: 0 - 39, height: 0 - 39
    spriteB->setPosition(39, 0); // Width: 39 - 59, height: 0 - 19
    didCollide = coordUtil->collide(spriteA, spriteB);
    EXPECT_EQ(true, didCollide) << "Check with scale";

    delete spriteA;
    delete spriteB;
}
