#include "gtest/gtest.h"

#include "gameIO/ContentManager.hpp"
#include "util/SpriteUtil.hpp"
#include "actors/Character.hpp"

#include <iostream>
#include <stdlib.h>
using namespace std;


/** A Character that has no behaviour (therefor, static).
 * This Character is used for testing the Character class.
 * As Character is an abstract class, this concrete class ensures
 * that Character can be instantiated.
 */
class StaticCharacter : public Character
{
    public:
        StaticCharacter(float x, float y, float width, float height,
                        float speed, Sprite *sprite) :
                        Character(x, y, width, height, speed, sprite)
        {
            subUpdateCalled = 0;
        }

        void subUpdate(Clock *clock, RenderWindow *window, World *world)
        {
            subUpdateCalled = 1;
            // Does nothing
        }

        bool isSubUpdateCalled()
        {
            return subUpdateCalled;
        }

        // Expose protected methods as public for testing
        void findCurrentIsland(list<Island*>* islands)
        {
            Character::findCurrentIsland(islands);
        }

        void steer(float elapsedTime)
        {
            Character::steer(elapsedTime);
        }

        void lockToIsland(float elapsedTime)
        {
            Character::lockToIsland(elapsedTime);
        }

    private:
        bool subUpdateCalled;
};

class CharacterTest : public ::testing::Test
{
    protected:
        StaticCharacter *character;
        list<Island*> *islands;

        ContentManager content;
        SpriteUtil spriteUtil;

        virtual void SetUp()
        {
            // Load the test character
            float x = 200;
            float y = 200;
            float width = 10;
            float height = 20;
            float speed = 200;

            string characterSpritePath = "unit_test/actors/filesForTests/Character.png";
            Sprite *characterSprite = new Sprite();
            bool loadSuccess = spriteUtil.loadSprite(characterSpritePath, characterSprite, &content);
            if(!loadSuccess)
                cout << "CharacterTest.SetUp: Could not load image: " << characterSpritePath << endl;
            character = new StaticCharacter(x, y, width, height,
                                            speed, characterSprite);

            // Load island 1
            islands = new list<Island*>();
            x = 160;
            y = 202;
            width = 185;
            height = 60;

            string islandPath = "unit_test/actors/filesForTests/Island1.png";
            Sprite *islandSprite = new Sprite();
            loadSuccess = spriteUtil.loadSprite(islandPath, islandSprite, &content);
            if(!loadSuccess)
                cout << "CharacterTest.SetUp: Could not load image: " << islandPath << endl;
            Island *island1 = new Island(x, y, width, height, islandSprite);
            islands->push_back(island1);

            // Load island 2
            x = 140;
            y = 0;
            width = 185;
            height = 60;

            Island *island2 = new Island(x, y, width, height, islandSprite);
            islands->push_back(island2);


        }

        virtual void TearDown()
        {

            if(character != 0)
                delete character;

            if(islands != 0)
                delete islands;

            content.clear();
        }
};

TEST_F(CharacterTest, locatesNearbyIsland)
{
    FAIL() << "Not yet implemented";
}

TEST_F(CharacterTest, doesntLocateDistantIsland)
{
    FAIL() << "Not yet implemented";
}

TEST_F(CharacterTest, jumpsAgainstGravity)
{
    FAIL() << "Not yet implemented";
}

TEST_F(CharacterTest, movesLeftAndRight)
{
    FAIL() << "Not yet implemented";
}

TEST_F(CharacterTest, pulledWithGravity)
{
    FAIL() << "Not yet implemented";
}

TEST_F(CharacterTest, attachesToStraitLand)
{
    FAIL() << "Not yet implemented";
}

TEST_F(CharacterTest, attachesToGentleAngledLand)
{
    FAIL() << "Not yet implemented";
}

TEST_F(CharacterTest, attachesToSteepAngledLand)
{
    FAIL() << "Not yet implemented";
}

TEST_F(CharacterTest, movesAboveGroundWhenUnderground)
{
    FAIL() << "Not yet implemented";
}
