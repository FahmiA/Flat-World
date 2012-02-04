#ifndef CHARACTER_H
#define CHARACTER_H

#include "game/World.hpp"
#include "environment/Island.hpp"
#include "util/CoordinateUtil.hpp"
#include "util/SpriteUtil.hpp"
#include "util/Commons.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

class Character: public GameObject
{
    public:
        Character(float x, float y, float width, float height, float speed, Sprite *sprite);

        virtual ~Character();

        void update(Clock *clock, RenderWindow *window, World *world);
        void draw(RenderWindow *window);

        virtual void subUpdate(Clock *clock, RenderWindow *window, World *world) = 0;

        const Vector2f& getPosition();
        const Vector2f& getSize();
        float getRotation();

        void landHop();
        void moveLeft();
        void moveRight();
        Direction getFacingDirection();
        Sprite *getSprite();

    protected:
        /** Gets the CoordinateUtil object.
         * Used for performing various coordinate-bases procedures.
         */
        CoordinateUtil& getCoordinateUtil();

        /** Gets the SpriteUtil object.
         * Used for performing various sprite-based procedures.
         */
        SpriteUtil& getSpriteUtil();

        /** Sets the distance the charcter should be from the ground.
         * @param distance Distance between the character and the ground.
         */
        void setDistanceFromGround(float distance);

    private:
        // State variables
        float speed;
        Sprite *sprite;
        Shape line;
        Island *currentGround;
        Island *prevGround;
        bool inJump;
        float distanceFromGround;

        // Actions-to-perform variables
        bool doMoveLeft;
        bool doMoveRight;
        Direction facingDirection;

        // Helper-operations variables
        CoordinateUtil coordUtil;
        SpriteUtil spriteUtil;

        // Methods
        /** Finds the nearest island and assigns it as the island the Character is on.
         * Also remembers the previous island.
         * @param world The world containing the islands to search,
         */
        void findCurrentIsland(World *world);

        /** Steers the Character based on assigned movement commands.
         * @param elapsedTime The time since the previous frame.
         */
        void steer(float elapsedTime);

        /** Sticks the Character to the border of the current island.
         * The Character's position and angle are set to match the conditions of the
         * current position of the Character on the island.
         * @param elapsedTime The time since the previous frame.
         */
        void lockToIsland(float elapsedTime);

};

// Define common animation states
#define ANIMATE_WALK 0
#define ANIMATE_RUN 1

#endif // CHARACTER_H
