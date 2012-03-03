#include "gtest/gtest.h"

#include "gameIO/ContentManager.hpp"
#include "util/SpriteUtil.hpp"
#include "actors/Character.hpp"

#include <iostream>
#include <stdlib.h>
using namespace std;

// http://code.google.com/p/googletest/

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
        Island *island1;
        Island *island2;

        ContentManager content;
        SpriteUtil spriteUtil;

        virtual void SetUp()
        {
            // Load the test character
            float x = 205;
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
            x = 248.5;
            y = 232;
            width = 185;
            height = 60;

            string islandPath = "unit_test/actors/filesForTests/Island1.png";
            Sprite *island1Sprite = new Sprite();
            loadSuccess = spriteUtil.loadSprite(islandPath, island1Sprite, &content);
            if(!loadSuccess)
                cout << "CharacterTest.SetUp: Could not load image: " << islandPath << endl;
            island1 = new Island(x, y, width, height, island1Sprite);
            islands->push_back(island1);

            // Load island 2
            x = 140;
            y = 0;
            width = 185;
            height = 60;

            Sprite *island2Sprite = new Sprite();
            loadSuccess = spriteUtil.loadSprite(islandPath, island1Sprite, &content);
            if(!loadSuccess)
                cout << "CharacterTest.SetUp: Could not load image: " << islandPath << endl;
            island2 = new Island(x, y, width, height, island2Sprite);
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
    // Check with no current ground
    character->findCurrentIsland(islands);
    Island *currentGround = character->getCurrentGround();
    Island *previousGround = character->getPreviousGround();

    EXPECT_EQ(island1, currentGround) << "New ground should be found.";
    EXPECT_EQ(NULL, previousGround) << "No previous ground should exist.";

    // Check with closest ground as current ground
    character->findCurrentIsland(islands);
    EXPECT_EQ(island1, currentGround) << "Current ground should not change.";
    EXPECT_EQ(NULL, previousGround) << "No previous ground should exist.";

    // Check with island2 as the new closest ground
    island1->setPosition(1000, 1000);
    island2->setPosition(160, 202);

    character->landHop(); // Detach from current ground
    character->findCurrentIsland(islands);

    currentGround = character->getCurrentGround();
    previousGround = character->getPreviousGround();
    EXPECT_EQ(island2, currentGround) << "New ground should be found.";
    EXPECT_EQ(island1, previousGround) << "Previous ground should be old current ground.";
}

TEST_F(CharacterTest, doesntLocateDistantIsland)
{
    island1->setPosition(1000, 1000);
    island2->setPosition(500, 500);

    character->findCurrentIsland(islands);

    Island *currentGround = character->getCurrentGround();
    Island *previousGround = character->getPreviousGround();

    EXPECT_EQ(NULL, currentGround) << "Distant island should not be found.";
    EXPECT_EQ(NULL, previousGround) << "No previous ground should exist.";
}

TEST_F(CharacterTest, jumpsAgainstGravity)
{
    // Locate the current island
    character->findCurrentIsland(islands);

    // Jump away from the current island
    character->landHop();

    // Test that the Character is jumping
    EXPECT_TRUE(character->isJumping()) << "Character should be jumping";

    // Test for island properties
    Island *currentGround = character->getCurrentGround();
    Island *previousGround = character->getPreviousGround();
    EXPECT_EQ(NULL, currentGround) << "Should be no current ground during an island hop.";
    EXPECT_EQ(island1, previousGround) << "No previous ground should be set to mold current ground.";

    // Test that position changes while in jump
    Vector2f oldPosition = character->getPosition();
    character->steer(0.03f);
    Vector2f newPosition = character->getPosition();

    EXPECT_FLOAT_EQ(oldPosition.x, newPosition.x) << "X-position should not change (vertical jump)";
    EXPECT_NE(oldPosition.y, newPosition.y) << "Y-position should have changed.";
}

TEST_F(CharacterTest, movesLeftAndRight)
{
    character->findCurrentIsland(islands);

    // Move left
    character->moveLeft();
    EXPECT_EQ(Left, character->getFacingDirection()) << "Sould be facing left.";

    Vector2f oldPosition = character->getPosition();
    character->steer(0.03f);
    Vector2f newPosition = character->getPosition();
    EXPECT_LT(newPosition.x, oldPosition.x) << "Should move left.";

    // Move right
    character->moveRight();
    EXPECT_EQ(Right, character->getFacingDirection()) << "Sould be facing right.";

    oldPosition = newPosition;
    character->steer(0.03f);
    newPosition = character->getPosition();
    EXPECT_GT(newPosition.x, oldPosition.x) << "Should move right.";

    // Repeat to check state change

    // Move left
    character->moveLeft();
    character->moveLeft(); // Moving left twice should not toggle direction
    EXPECT_EQ(Left, character->getFacingDirection()) << "Sould be facing left.";

     // Move right
    character->moveRight();
    character->moveRight(); // Moving right twice should not toggle direction
    EXPECT_EQ(Right, character->getFacingDirection()) << "Sould be facing right.";
}

TEST_F(CharacterTest, pulledWithGravity)
{
    character->findCurrentIsland(islands);

    Vector2f oldPosition = character->getPosition();
    character->lockToIsland(0.03);
    Vector2f newPosition = character->getPosition();

    EXPECT_GT(newPosition.y, oldPosition.y) << "Character should be pulled down with gravity";
}

TEST_F(CharacterTest, attachesToStraitLand)
{
    character->findCurrentIsland(islands);
    character->lockToIsland(0.05f);

    Vector2f position = character->getPosition();

    // At close range, the character should snap to the island's surface
    EXPECT_EQ(205, position.x) << "X-position should be on ground";
    EXPECT_EQ(205, position.y) << "Y-position should be on ground";
    EXPECT_EQ(0, character->getRotation()) << "Should not be rotated";
}

TEST_F(CharacterTest, attachesToGentleAngledLand)
{
    // Move the island so the player lands on a hill
    island1->setPosition(230, 235);

    character->findCurrentIsland(islands);
    character->lockToIsland(0.0f); // The player is already close enough

    Vector2f position = character->getPosition();

    // At close range, the character should snap to the island's surface
    EXPECT_EQ(205, position.x) << "X-position should be on ground";
    EXPECT_EQ(203, position.y) << "Y-position should be on ground";
    EXPECT_NEAR(0.3, character->getRotation(), 0.05) << "Should be rotated to angle of hill";
}

TEST_F(CharacterTest, attachesToSteepAngledLand)
{
    // Move the island so the player lands on a hill
    island1->setPosition(169, 239);

    character->findCurrentIsland(islands);
    character->lockToIsland(0.0f); // The player is already close enough

    Vector2f position = character->getPosition();

    // At close range, the character should snap to the island's surface
    EXPECT_EQ(209, position.x) << "X-position should be on ground";
    EXPECT_EQ(209, position.y) << "Y-position should be on ground";
    EXPECT_NEAR(5.7, character->getRotation(), 0.05) << "Should be rotated to angle of hill";
}

TEST_F(CharacterTest, movesAboveGroundWhenUnderground)
{
    // Move the island so the player lands on a hill
    island1->setPosition(228, 225);

    character->findCurrentIsland(islands);
    character->lockToIsland(0.0f); // The player is already close enough

    Vector2f position = character->getPosition();

    // At close range, the character should snap to the island's surface
    EXPECT_EQ(209, position.x) << "X-position should be on ground";
    EXPECT_EQ(197, position.y) << "Y-position should be on ground";
    EXPECT_NEAR(0.3, character->getRotation(), 0.05) << "Should be rotated to angle of hill";
}
