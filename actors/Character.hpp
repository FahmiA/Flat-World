#ifndef CHARACTER_H
#define CHARACTER_H

#include "game/World.hpp"
#include "util/TSprite.hpp"
#include "environment/Island.hpp"
#include "util/CoordinateUtil.hpp"
#include "util/SpriteUtil.hpp"
#include "util/Commons.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

/**Defines any object that must be grounded to an island.
 * Charcters are grounded to an island and are able to move left and right
 * as well as jump between islands.
 */
class Character: public GameObject
{
    public:
        /** Creates a new Character.
         * @param x The x position to spawn.
         * @param y the y position to spawn.
         * @param width The width of the character. The sprite will be scaled to the width.
         * @param height The height of the character. The sprite will be scaled to the height.
         * @param speed The movement speed of the Character. Given in pixels-per-second.
         * @param sprite The initialised Sprite object used to display the Character.
         */
        Character(float x, float y, float width, float height, float speed, TSprite *sprite);
        virtual ~Character();

        // Standard update and draw protocol.
        void update(Clock *clock, RenderWindow *window, World *world);
        virtual void draw(RenderWindow *window);

        /** A Template Method to allow subclass-specific behaviour to take place.
         * Always invoked BEFORE the Character (base class) update logic is performed.
         * @param clock The game clock to be used for checking time and frame delays.
         * @param window The RenderWIndow to be used for checking program state.
         * @param world The state of the game world.
         */
        virtual void subUpdate(Clock *clock, RenderWindow *window, World *world) = 0;

        // Position and size getters
        const Vector2f& getPosition();
        void setPosition(float x, float y);
        const Vector2f& getSize();

        float getSpeed();

        /** Gets the rotation of the Charcter in Radians.
         * @return Character rotation.
         */
        float getRotation();

        /** Makes the Character jump, leaving the current island as a result.
         * When the Character (now flying throught the air) reaches another island,
         * the Character will land on the new island.
         */
        void landHop();

        /** moves the Character left on the next update.
         * movement speed is determined by the speed given in the constructor.
         */
        void moveLeft();

        /** moves the Character right on the next update.
         * movement speed is determined by the speed given in the constructor.
         */
        void moveRight();

        /** Checks if the Character is currently jumping between islands.
         * @return True if the Character is island jumping. False otherwise.
         */
        bool isJumping();

        /**Gets the direction that the Character is currently facing.
         * Left and Right are the only two valid directions to face.
         * @return The direction that the Character currently facing.
         */
        Direction getFacingDirection();

        /** Gets the Sprite used to display the Character.
         * @return The Character's Sprite.
         */
        TSprite* getSprite();

        /** Gets the current ground the Character is on.
         * @return Current ground of NULL if no current ground exists (eg: in jump).
         */
        Island* getCurrentGround();

        /** Gets the previous ground the Character was on.
         * @return Previous ground of NULL if no previous ground exists.
         */
        Island* getPreviousGround();

        /** Marks the direction that the sprite image is facing.
         * @direction The direction of the sprite image. Left or Right only.
         */
        void setSpriteDirection(Direction direction);

    protected:
        /** Finds the nearest island and assigns it as the island the Character is on.
         * Also remembers the previous island.
         * @param islands A list of islands to search.
         */
        void findCurrentIsland(list<Island*>* islands);

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

        /** Gets the CoordinateUtil object.
         * Used for performing various coordinate-bases procedures.
         */
        CoordinateUtil& getCoordinateUtil();

        /** Sets the distance the charcter should be from the ground.
         * @param distance Distance between the character and the ground.
         */
        void setDistanceFromGround(float distance);

    private:
        // State variables
        float speed;
        TSprite *sprite;
        Island *currentGround;
        Island *prevGround;
        bool inJump;
        float distanceFromGround;
        float prevAngle; // Radians

        // Actions-to-perform variables
        bool doMoveLeft;
        bool doMoveRight;
        Direction facingDirection; // Direction of the charcter
        Direction spriteDirection; // Direction alignment of sprire image

        // Helper-operations variables
        CoordinateUtil coordUtil;

        void clampToGround(Vector2f &leftCollide, float groundAngleRad);
        bool isAboveGround(Vector2f spritePoint, Island &ground);

        // Debug variables
        ConvexShape angleLine;
        ConvexShape lookLine;

        bool pause;
        RectangleShape bounds;
};

#endif // CHARACTER_H
