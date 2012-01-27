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
TEST_F(CoordinateUtilTest, distance)
{
    float result;

    result = coordUtil->distance(Vector2f(5, 1), Vector2f(10, 1));
    EXPECT_FLOAT_EQ(result, 5) << "Distance between x positions not correct";

    result = coordUtil->distance(Vector2f(5, 245), Vector2f(5, 246));
    EXPECT_FLOAT_EQ(result, 1) << "Distance between y positions not correct";

    result = coordUtil->distance(Vector2f(-5, 1), Vector2f(5, -1));
    EXPECT_NEAR(result, 10.198, 0.001) << "Distance with some negative values not correct";

    result = coordUtil->distance(Vector2f(5, 5), Vector2f(-10, -10));
    EXPECT_NEAR(result, 21.21, 0.005) << "Should recieve absolute value of distance";

    result = coordUtil->distance(Vector2f(5, 5), Vector2f(5, 5));
    EXPECT_FLOAT_EQ(result, 0) << "Distance of 0 expected with two identical coordinates";

    result = coordUtil->distance(Vector2f(0, 0), Vector2f(0, 0));
    EXPECT_FLOAT_EQ(result, 0) << "Should handle origin coordinates correctly.";
}

TEST_F(CoordinateUtilTest, isInFOVAcute)
{
    bool result;
    int lookDistance = 586;
    float fovAngle = 0.5; // radians

    // Test for inside FOV
    // Well inside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(491, 297), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it is within the FOV.";
    // Upper corner
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(556, 226), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the upper corner of the FOV.";
    // Upper edge
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(575, 271), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the upper edge of the FOV.";
    // Directly infront
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(586, 345), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the distant edge of the FOV.";
    // Lower edge
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(575, 415), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the lower edge of the FOV.";
    // Lower corner
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(556, 462), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the lower corner of the FOV.";
    // Ontop of source
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(336, 345), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it is on top of the source.";

    // Test for outside FOV
    // Infront and outside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(243, 334), lookDistance, fovAngle);
    ASSERT_FALSE(result) << "Target should not be detected if it is close but not in the FOV.";
    // Infront and outside FOV
    result = coordUtil->isInFOV(Vector2f(14, 5), Vector2f(1000, 4), lookDistance, fovAngle);
    ASSERT_FALSE(result) << "Target should not be detected if it is infront, but outside, the FOV";
    // Behind, but oitherwise within, FOV
    result = coordUtil->isInFOV(Vector2f(-14, -5), Vector2f(-90, -6), lookDistance, fovAngle);
    ASSERT_FALSE(result) << "Target should not be detected if it is behind, but otherwise inside, the FOV";
}

TEST_F(CoordinateUtilTest, isInFOVReflex)
{
    bool result;
    int lookDistance = 586;
    float fovAngle = 2.4; // radians

    // Test for inside FOV
    // Well inside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(491, 297), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it is within the FOV.";
    // Upper corner
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(152, 177), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the upper corner of the FOV.";
    // Upper edge
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(336, 96), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the upper edge of the FOV.";
    // Directly infront
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(586, 345), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the distant edge of the FOV.";
    // Lower edge
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(336, 595), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the lower edge of the FOV.";
    // Lower corner
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(152, 515), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it borders the lower corner of the FOV.";
    // Ontop of source
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(336, 345), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it is on top of the source.";

    // Test for outside FOV
    // Infront and outside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(278, 337), lookDistance, fovAngle);
    ASSERT_FALSE(result) << "Target should not be detected if it is close but not in the FOV.";
    // Infront and outside FOV
    result = coordUtil->isInFOV(Vector2f(14, 5), Vector2f(1105, 4), lookDistance, fovAngle);
    ASSERT_FALSE(result) << "Target should not be detected if it is infront, but outside, the FOV";
}

TEST_F(CoordinateUtilTest, isInFOVFull)
{
     bool result;
    int lookDistance = 586;
    float fovAngle = M_PI; // radians

    // Test for inside FOV
    // Well inside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(491, 297), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it is within the FOV.";
    // Border
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(86, 345), lookDistance, fovAngle);
    ASSERT_TRUE(result) << "Target should be detected if it border of the FOV.";

    // Test for outside FOV
    // Infront and outside FOV
    result = coordUtil->isInFOV(Vector2f(336, 345), Vector2f(278, 337), lookDistance, fovAngle);
    ASSERT_FALSE(result) << "Target should not be detected if it is close but not in the FOV.";
}
