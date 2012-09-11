#ifndef LEVELBUILDERXML_H
#define LEVELBUILDERXML_H

#include "LevelBuilder.hpp"
#include "gameIO/AnimatedSpriteLoader.hpp"

/** Constructs a standard game world.
 * Concrete Builder in the Builder design pattern.
 */
class LevelBuilderStd : public LevelBuilder
{
    public:
        /** Creates a new LevelBuilderStd.
         * @param world A reference to the game world to build.
         * @param content The ContentManager to use for loading external assets.
         */
        LevelBuilderStd(World *world, ContentManager *content);
        virtual ~LevelBuilderStd();

        // Buider methods

        bool setBackground(LevelDescription *levelDesc);
        bool addIsland(IslandDescription *islandDesc);
        bool setPlayer(UnitDescription* playerDesc, IslandDescription *islandDesc);
        bool addSheep(UnitDescription *sheepDesc, Island *island);
        void addRandomSheep();
        bool addSheepdog(UnitDescription *sheepdogDesc, IslandDescription *islandDesc);
        bool addStar(PickupDescription *pickupDesc, IslandDescription *islandDesc,
                     float angle);
        bool setHUD(string &sheepCornerPath, string &starCornerPath,
                    string &sheepIconPath, string &starIconPath,
                    string &fontPath);

    private:
        int sheepCount;
        int starCount;

        AnimatedSpriteLoader *aniLoader;
};

#endif // LEVELBUILDERXML_H
