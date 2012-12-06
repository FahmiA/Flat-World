#ifndef WORLD_H
#define WORLD_H

#include "game/GameObject.hpp"
#include "scene/Camera.hpp"
#include "util/TSprite.hpp"

class Player;
class HUD;
class Island;
class Background;

#include <list>
using namespace std;

/** Defines the state of the world.
 * Holds all object in the game world and updates and draws them too.
 * Objects can query the World for information about game state.
 */
class World
{
    public:
        /** Creates a new game world. */
        World();
        virtual ~World();

        /** Adds a new miscellaneous object to the world.
         * All object added here are treated the same.
         * @param gameObject The game object to add to the world.
         */
        void addLevelObject(GameObject *gameObject);

        /** Removes an object from the world.
         * Only objects added through addLevelObject() can be removed.
         */
        void removeLevelObject(GameObject *gameObject);

        /** Adds an Island to the world.
         * Adding an Island using this method as opposed of using addLevelObject()
         * ensures that Islands are not drawn over forground objects.
         * @param island The island to add.
         */
        void addIsland(Island *island);

        /** Removes and island from the world.
         * Only islands added through addIsland() can be removed.
         */
        void removeIsland(Island *island);

        /** Gets the Player.
         * @return The Player.
         */
        Player* getPlayer();

        /** Sets the Player.
         * Only one Player is permited. If an older Player exists,
         * the older Player is replaced with the new Player.
         * Note: The older player will not be deleted.
         * @param player The Player to add to the world.
         */
        void setPlayer(Player *player);

        /** Gets all miscellaneous game objects added using addLevelObject().
         * @return A list of game objects in the world.
         */
        list<GameObject*>* getObjects();

        /** Gets all islands added using addIsland().
         * @return A list islands in the world.
         */
        list<Island*>* getIslands();

        /** Sets the default Camera to use during gameplay.
         * @param camera The Camera to use.
         */
        void setCamera(Camera *camera);

        void setBackground(Background *background);
        Background* getBackground();

        /** Gets the Heads-Up-Display (HUD).
         * @return The HUD.
         */
        HUD* getHud();

        /** Sets the Heads-Up-Display (HUD).
         * Only one HUD is permited. If an older HUD exists,
         * the older HUD is replaced with the new HUD.
         * Note: The older HUD will not be deleted.
         * @param hud The HUD to display.
         */
        void setHud(HUD *hud);

        /** Gets the number of sheep that the player has currently captured.
         * @return The nnumber of captued sheep.
         */
        int getSheepCaptured();

        void update(Clock *clock, RenderWindow *window);
        void draw(RenderWindow *window);

    private:
        HUD *hud;
        Player *player;
        list<GameObject*> *levelObjects;
        list<Island*> *islands;
        Background *background;

        Camera *gameCamera;
        Camera *hudCamera;
};

#endif // WORLD_H
