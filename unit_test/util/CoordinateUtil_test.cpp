#include <limits.h>
#include <math.h>
#include "gtest/gtest.h"

#include "../../util/CoordinateUtil.hpp"

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
    EXPECT_FLOAT_EQ(0, angle);

    angle = coordUtil->getAngle(Vector2f(10, 10), 0, Vector2f(75, 2));
    EXPECT_NEAR(-0.122f, angle, 0.001);

    angle = coordUtil->getAngle(Vector2f(10, 10), 0.5, Vector2f(75, 2));
    EXPECT_NEAR(0.378f, angle, 0.001);

     angle = coordUtil->getAngle(Vector2f(10, 10), -1.2, Vector2f(75, 2));
    EXPECT_NEAR(-1.322f, angle, 0.001);
}

TEST_F(CoordinateUtilTest, isInFOVAcute)
{
    bool result;
    int lookDistance = 586;
    float fovAngle = 0.5; // radians

    // Test for inside FOV
    // Well inside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), 0, Vector2f(491, 297), lookDistance, fovAngle);
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
}
