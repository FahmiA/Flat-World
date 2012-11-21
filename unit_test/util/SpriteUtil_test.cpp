#include "gtest/gtest.h"

#include "util/SpriteUtil.hpp"

#define SQUARE_IMAGE_PATH "unit_test/util/filesForTests/square.png"

class SpriteUtilTest : public ::testing::Test
{
    protected:

        SpriteUtil *spriteUtil;

        virtual void SetUp()
        {
            spriteUtil = new SpriteUtil();
        }

        virtual void TearDown()
        {
            delete spriteUtil;
        }
};

TEST_F(SpriteUtilTest, rayTraceCommonCase)
{
    // Load an image of a square
    Image image;
    if(!image.loadFromFile(SQUARE_IMAGE_PATH))
        FAIL() << "Image could not be loaded for test: " << SQUARE_IMAGE_PATH;
    TSprite sprite(image);

    // Run the tests

    // from inside-void to inside-solid, horizontal
    Vector2f *result = spriteUtil->rayTrace(sprite, 18, 10, 12, 10);
    EXPECT_EQ(14, result->x) << "from inside-void to inside-solid, horizontal";
    EXPECT_EQ(10, result->y) << "from inside-void to inside-solid, horizontal";
    delete result;

    // from inside-void to inside-solid, vertical
    result = spriteUtil->rayTrace(sprite, 14, 0, 14, 14);
    EXPECT_EQ(14, result->x) << "from inside-void to inside-solid, vertical";
    EXPECT_EQ(5, result->y) << "from inside-void toinside-solid, vertical";
    delete result;

    // from inside-void to inside-solid, diagonal
    result = spriteUtil->rayTrace(sprite, 15, 4, 14, 6);
    EXPECT_EQ(14, result->x) << "from inside-void to inside-solid, diagonal";
    EXPECT_EQ(6, result->y) << "from inside-void to inside-solid, diagonal";
    delete result;

    // from inside-void to inside-void
    result = spriteUtil->rayTrace(sprite, 18, 5, 15, 12);
    EXPECT_EQ(0, result) << "from inside-void to inside-void";
    if(result) delete result;

    // from inside-solid to inside-solid
    result = spriteUtil->rayTrace(sprite, 7, 8, 13, 10);
    EXPECT_EQ(7, result->x) << "from inside-solid to inside-solid";
    EXPECT_EQ(8, result->y) << "from inside-solid to inside-solid";
    delete result;
}

TEST_F(SpriteUtilTest, rayTraceInvertedCase)
{
    // Load an image of a square
    Image image;
    if(!image.loadFromFile(SQUARE_IMAGE_PATH))
        FAIL() << "Image could not be loaded for test: " << SQUARE_IMAGE_PATH;
    TSprite sprite(image);

    // Run the tests
    Vector2f *result;
    Vector2f *nullVector = 0;

    // from inside-solid to inside-void, horizontal
    result = spriteUtil->rayTrace(sprite, 12, 10, 18, 10, false);
    EXPECT_EQ(15, result->x) << "from inside-solid to inside-void, horizontal";
    EXPECT_EQ(10, result->y) << "from inside-solid to inside-void, horizontal";
    delete result;

    // from inside-solid to inside-void, vertical
    result = spriteUtil->rayTrace(sprite, 14, 14, 14, 0, false);
    EXPECT_EQ(14, result->x) << "from inside-solid to inside-void, vertical";
    EXPECT_EQ(4, result->y) << "from inside-solid toinside-void, vertical";
    delete result;

    // from inside-solid to inside-void, diagonal
    result = spriteUtil->rayTrace(sprite, 14, 6, 15, 4, false);
    EXPECT_EQ(15, result->x) << "from inside-solid to inside-void, diagonal";
    EXPECT_EQ(4, result->y) << "from inside-solid to inside-void, diagonal";
    delete result;

    // from inside-void to inside-void
    result = spriteUtil->rayTrace(sprite, 15, 12, 18, 5, false);
    EXPECT_EQ(15, result->x) << "from inside-void to inside-void";
    EXPECT_EQ(12, result->y) << "from inside-void to inside-void";
    delete result;

    // from inside-solid to inside-solid
    result = spriteUtil->rayTrace(sprite, 7, 8, 13, 10, false);
    EXPECT_EQ(0, result) << "from inside-solid to inside-solid";
    if(result) delete result;

    // from out of bounds to within bounds
    result = spriteUtil->rayTrace(sprite, 30, 10, 10, 10, false);
    ASSERT_NE(nullVector, result) << "from out of bounds to within bounds: Want result.";
    EXPECT_EQ(19, result->x) << "from out of bounds to within bounds";
    EXPECT_EQ(10, result->y) << "from out of bounds to within bounds";
    if(result) delete result;
}

TEST_F(SpriteUtilTest, rayTraceCornerCase)
{
    // Load an image of a square
    Image image;
    if(!image.loadFromFile(SQUARE_IMAGE_PATH))
        FAIL() << "Image could not be loaded for test: " << SQUARE_IMAGE_PATH;
    TSprite sprite(image);

    // Run the tests

    // to: inside-void, from: outside, clamp from
    Vector2f *result = spriteUtil->rayTrace(sprite, 100, 12, 3, 12);
    EXPECT_EQ(14, result->x) << "to: inside-void, from outside, clamp from";
    EXPECT_EQ(12, result->y) << "to: inside-void, from outside, clamp from";
    delete result;

    // to: inside-void, from: outside, clamp from
    result = spriteUtil->rayTrace(sprite, 100, 12, 17, 12);
    EXPECT_EQ(0, result) << "to: inside-void, from: outside, clamp from";
    if(result) delete result;

    // to: inside-void, from: outside, clamp from
    result = spriteUtil->rayTrace(sprite, 21, 33, 3, 8);
    EXPECT_EQ(7, result->x) << "to: inside-void, from outside, clamp from";
    EXPECT_EQ(14, result->y) << "to: inside-void, from outside, clamp from";
    if(result) delete result;

     // to: inside-void, from: outside, clamp to
    result = spriteUtil->rayTrace(sprite, 3, 12, 20, -1);
    EXPECT_EQ(5, result->x) << "to: inside-void, from outside, clamp to";
    EXPECT_EQ(10, result->y) << "to: inside-void, from outside, clamp to";
    delete result;

    // to: inside-void, from: outside, clamp to
    result = spriteUtil->rayTrace(sprite, 18, 15, 100, 100);
    EXPECT_EQ(0, result) << "to: inside-void, from: outside, clamp to";
    if(result) delete result;

    // to: outside, from: outside, clamp from and to
    result = spriteUtil->rayTrace(sprite, 99, 100, -99, 99);
    EXPECT_EQ(0, result) << "to: outside, from: outside, clamp from and to";
    if(result) delete result;
}

/*TEST_F(SpriteUtilTest, getSize)
{
    Texture texture;
    if(!texture.loadFromFile(SQUARE_IMAGE_PATH))
        FAIL() << "Image could not be loaded for test: " << SQUARE_IMAGE_PATH;
    Sprite sprite(texture);

    // Check default size
    Vector2f size = spriteUtil->getSize(&sprite);
    EXPECT_EQ(20, size.x) << "Sprite unscaled width should match image width";
    EXPECT_EQ(20, size.y) << "Sprite unscaled height should match image height";

    // Check scaled size
    sprite.setScale(1.5, 0.8);
    size = spriteUtil->getSize(&sprite);
    EXPECT_EQ(30, size.x) << "Sprite scaled width should match scaled image width";
    EXPECT_EQ(16, size.y) << "Sprite scaled height should match scaled image height";
}*/
