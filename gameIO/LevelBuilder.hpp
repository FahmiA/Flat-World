#ifndef LEVELBUILDER_H
#define LEVELBUILDER_H

#include "game/World.hpp"
#include "gameIO/ContentManager.hpp"
#include "environment/Island.hpp"
#include "actors/Sheep.hpp"
#include "actors/Sheepdog.hpp"
#include "actors/Player.hpp"

#include <set>
using namespace std;

// Prototypes for description structs
struct LevelDescription;
struct IslandDescription;
struct UnitDescription;
struct PickupDescription;

/** Defines an interface for classes that can build a game world.
 * Abstract Builder in the Builder design pattern.
 */
class LevelBuilder
{
    public:
        /** Creates a new LevelBuilder.
         * @param world A reference to the game world to build.
         * @param content The ContentManager to use for loading external assets.
         */
        LevelBuilder(World *world, ContentManager *content);
        virtual ~LevelBuilder();

       // Buider methods

        /** Sets the background for a level in the World.
         * Only sets a background if one does not already exist.
         * @param levelDesc A description of the level to set the background to.
         * @return True if the background was successfully set. False otherwise.
         */
        virtual bool setBackground(LevelDescription *levelDesc) = 0;

        /** Adds a new island to the World.
         * @param islandDesc A description of the island to add.
         * @return True if the island was successfully added. False otherwise.
         */
        virtual bool addIsland(IslandDescription *islandDesc) = 0;

        /** Sets the Player unit in the world.
         * Only sets the player if one does not already exist.
         * @param playerDesc A description of the player to set.
         * @param islandDesc A description of the island the player will appear on.
         * @return True if the player was successfully added to the island. False otherwise.
         */
        virtual bool setPlayer(UnitDescription* playerDesc, IslandDescription *islandDesc) = 0;

        /** Adds a new Sheep unit to the world,
         * @param sheepDesc A description of the sheep to add.
         * @param island The island the sheep will appear on.
         * @return True if the sheep was successfully added to the island. False otherwise.
         */
        virtual bool addSheep(UnitDescription *sheepDesc, Island *island) = 0;

         /** Adds a completely random sheep to the world.
          * Picks a random style and a random spawn location.
          */
        virtual void addRandomSheep() = 0;

         /** Adds a new Sheepdog unit to the world,
          * @param sheepdogDesc A description of the sheepdog to add.
          * @param islandDesc A description of the island the sheepdog will appear on.
          * @return True if the sheepdog was successfully added to the island. False otherwise.
          */
        virtual bool addSheepdog(UnitDescription *sheepdogDesc, IslandDescription *islandDesc) = 0;

        /** Adds a new Star pickup to the world,
         * @param pickupDesc A description of the star to add.
         * @param islandDesc A description of the island the star will appear on.
         * @param angle The angle that determins the position of the star relative to the center of the island.
         * @return True if the star was successfully added to the island. False otherwise.
         */
        virtual bool addStar(PickupDescription *pickupDesc, IslandDescription *islandDesc, float angle) = 0;

        /** Creates a HUD and assigns the HUD to the world.
         * @param sheepCornerPath The path to the sheep corner image.
         * @param starCornerPath The path to the star corner image.
         * @param sheepIconPath The path to the sheep icon image.
         * @param starIconPath The path to the star icon image.
         * @param fontPath The path to the font that HUD text will appear in.
         */
        virtual bool setHUD(string &sheepCornerPath, string &starCornerPath,
                            string &sheepIconPath, string &starIconPath,
                            string &fontPath) = 0;

        /** Gets the ContentManager that is being used,
         * @return the used ContentManager.
         */
        virtual ContentManager* getContentManager();


        /** Retrieves the constructed world.
         * @return Constructed world.
         */
        virtual World* getWorld();

    protected:
        // Helper methods
        virtual void getPosition(Island *island, float angleRadians, int characterWidth, int characterHeight, int *characterX, int *characterY);
        bool loadFont(string &path, Font *&font);

        /** Stores the path to a sheep image.
         * Duplicates will be removed.
         * @param sheepPath The path to the sheep image
         */
        void addSheepPath(string sheepPath);

        /** Gets the ith sheep image path added.
         * @param index The index of the sheep image to get.
         * @return The sheep image path at index or "" if index is not valid.
         */
        string getSheepPath(int index);

        /** Gets the number of sheep image paths added.
         * @return Number of sheep image paths.
         */
        int getSheepPathCount();

    private:
        World *world;
        ContentManager *content;
        set<string> sheepPaths;
};

/** Describes the information needed to create a level
 * Excludes information on level contents.
 */
struct LevelDescription
{
    string name;
    int width;
    int height;
    string backgroundPath;
};

/** Describes the information needed to create an Island. */
struct IslandDescription
{
    int id;
    string imagePath;
    int x;
    int y;
    int width;
    int height;
    Island *island;
};

/** Describes the information needed to create an NPC unit. */
struct UnitDescription
{
    string type;
    string imagePath;
    int locationID;
    float startAngle;
    int width;
    int height;
};

/** A descriptor for a sequence of stars to appear on an island. */
struct PickupDescription
{
    /** The name of the pickup. */
    string type;
    /** The path to the image used to display the pickup, */
    string imagePath;
    /** The ID of the Island to appear on. */
    int locationID;
    /** The angle position on the island which determins the start of the sequence of stars. */
    float startAngle;
    /** The angle position on the island which determins the end of the sequence of stars. */
    float endAngle;
    /** The number of stars to appeaar in the sequence between startAngle and endAngle. */
    int count;
};

#endif // LEVELBUILDER_H
