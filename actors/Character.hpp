#ifndef CHARACTER_H
#define CHARACTER_H

#include "game/World.hpp"
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
        Character(float x, float y, float width, float height, float speed, Sprite *sprite);
        virtual ~Character();

        // Standard update and draw protocol.
        void update(Clock *clock, RenderWindow *window, World *world);
        void draw(RenderWindow *window);

        /** A Template Method to allow subclass-specific behaviour to take place.
         * Always invoked BEFORE the Character (base class) update logic is performed.
         * @param clock The game clock to be used for checking time and frame delays.
         * @param window The RenderWIndow to be used for checking program state.
         * @param world The state of the game world.
         */
        virtual void subUpdate(Clock *clock, RenderWindow *window, World *world) = 0;

        // Position and size getters
        const Vector2f& getPosition();
        const Vector2f& getSize();

        /** Gets the rotation of the Charcter in Radians.
         * @return Character rotation.
         */
        float getRotation();

        /** Makes the Character jump, leaving the current island as a result.
         * When the Character (now flying throught the air) reaches another island,
         * the Character will land on the new island.
         */
        void landHop();

        /** Moves the Character left on the next update.
         * Movement speed is determined by the speed given in the constructor.
         */
        void moveLeft();

        /** Moves the Character right on the next update.
         * Movement speed is determined by the speed given in the constructor.
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
        Sprite *getSprite();

        /** Gets the current ground the Character is on.
         * @return Current ground of NULL if no current ground exists (eg: in jump).
         */
        Island* getCurrentGround();

        /** Gets the previous ground the Character was on.
         * @return Previous ground of NULL if no previous ground exists.
         */
        Island* getPreviousGround();

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

        void clampToGround(Vector2f &leftCollide, Vector2f &rightCollide);
        bool isAboveGround(Sprite &groundSprite);

        // Debug variables
        Shape angleLine;
        Shape lookLine;
};

// Define common animation states
#define ANIMATE_WALK 0
#define ANIMATE_RUN 1

#endif // CHARACTER_H
