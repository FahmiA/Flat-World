#include "gtest/gtest.h"

#include "util/TSprite.hpp"
#include "util/Debug.hpp"
#include "environment/Background.hpp"

class BackgroundTest : public ::testing::Test
{
    protected:
        Background *background;

        virtual void SetUp()
        {
            TSprite star;
            TSprite cloud;
            background = new Background(star, cloud);
        }

        virtual void TearDown()
        {
            if(background != 0)
                delete background;
        }
};

TEST_F(BackgroundTest, constructor)
{
    EXPECT_FLOAT_EQ(background->getStage(), 0);
}

TEST_F(BackgroundTest, getStage)
{
    background->setStage(0.1);
    EXPECT_FLOAT_EQ(background->getStage(), 0.1);

    background->setStage(0.0f);
    background->setStage(0.8f);
    EXPECT_FLOAT_EQ(background->getStage(), 0.8);
}

TEST_F(BackgroundTest, setStageClamp)
{
    background->setStage(-0.2f);
    EXPECT_FLOAT_EQ(background->getStage(), 0);

    background->setStage(1.2f);
    EXPECT_FLOAT_EQ(background->getStage(), 1);
}

TEST_F(BackgroundTest, setStageStars)
{
    background->setStage(0.0f);
    int midnightCount = background->getStarCount();

    background->setStage(0.25f);
    int beforeDawnCount = background->getStarCount();

    background->setStage(0.5f);
    int sunRiseCount = background->getStarCount();

    EXPECT_GT(midnightCount, beforeDawnCount);
    EXPECT_GT(beforeDawnCount, sunRiseCount);
}

TEST_F(BackgroundTest, setStageClouds)
{
    background->setStage(1.0f);
    int middayCount = background->getCloudCount();

    background->setStage(0.75f);
    int mourningCount = background->getCloudCount();

    background->setStage(0.5f);
    int sunRiseCount = background->getStarCount();

    EXPECT_GT(middayCount, mourningCount);
    EXPECT_GT(mourningCount, sunRiseCount);
}
