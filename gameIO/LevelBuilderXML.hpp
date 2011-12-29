#ifndef LEVELBUILDERXML_H
#define LEVELBUILDERXML_H

#include "../game/World.hpp"
#include "../gameIO/ContentManager.hpp"
#include "../environment/Island.hpp"
#include "../actors/Sheep.hpp"
#include "../actors/Dog.hpp"
#include "../actors/Player.hpp"

struct LevelDescription;
struct IslandDescription;
struct UnitDescription;

class LevelBuilderXML
{
    public:
        // Construct and Destruct
        LevelBuilderXML(World *world, ContentManager *content);
        virtual ~LevelBuilderXML();

        // Buider methods

        /** Sets the background for a level in the World.
         * Only sets a background if one does not already exist.
         * @param levelDesc A description of the level to set the background to.
         * @return True if the background was successfully set. False otherwise.
         */
        bool setBackground(LevelDescription *levelDesc);

        /** Adds a new island to the World.
         * @param islandDesc A description of the island to add.
         * @return True if the island was successfully added. False otherwise.
         */
        bool addIsland(IslandDescription *islandDesc);

        /** Sets the Player unit in the world.
         * Only sets the player if one does not already exist.
         * @param playerDesc A description of the player to set.
         * @param islandDesc A description of the island the player will spawn on.
         * @return True if the player was successfully added to the island. False otherwise.
         */
        bool setPlayer(UnitDescription* playerDesc, IslandDescription *islandDesc);

        /** Adds a new Sheep unit to the world,
         * @param sheepDesc A description of the sheep to add.
         * @param islandDesc A description of the island the sheep will spawn on.
         * @return True if the sheep was successfully added to the island. False otherwise.
         */
        bool addSheep(UnitDescription *sheepDesc, IslandDescription *islandDesc);

         /** Adds a new Sheepdog unit to the world,
         * @param sheepdogDesc A description of the sheepdog to add.
         * @param islandDesc A description of the island the sheepdog will spawn on.
         * @return True if the sheepdog was successfully added to the island. False otherwise.
         */
        bool addSheepdog(UnitDescription *sheepdogDesc, IslandDescription *islandDesc);


        // Retrieve result
        World* getWorld();

    protected:
        bool loadSprite(string path, Sprite &sprite);
        void getPosition(Island *island, float angleRadians, int characterWidth, int characterHeight, int *characterX, int *characterY);

    private:
        World *world;
        ContentManager *content;
};


struct LevelDescription
{
    string name;
    int width;
    int height;
    string backgroundPath;
};

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

struct UnitDescription
{
    string type;
    string imagePath;
    int locationID;
    float startAngle;
};

#endif // LEVELBUILDERXML_H
