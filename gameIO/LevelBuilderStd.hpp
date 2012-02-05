#ifndef LEVELBUILDERXML_H
#define LEVELBUILDERXML_H

#include "LevelBuilder.hpp"

class LevelBuilderStd : public LevelBuilder
{
    public:
        // Construct and Destruct
        LevelBuilderStd(World *world, ContentManager *content);
        virtual ~LevelBuilderStd();

        // Buider methods

        bool setBackground(LevelDescription *levelDesc);
        bool addIsland(IslandDescription *islandDesc);
        bool setPlayer(UnitDescription* playerDesc, IslandDescription *islandDesc);
        bool addSheep(UnitDescription *sheepDesc, Island *island);
        bool addRandomSheep();
        bool addSheepdog(UnitDescription *sheepdogDesc, IslandDescription *islandDesc);
        bool addStar(PickupDescription *pickupDesc, IslandDescription *islandDesc,
                     float angle);
        bool setHUD(string &sheepCornerPath, string &starCornerPath,
                    string &sheepIconPath, string &starIconPath,
                    string &fontPath);

    private:
        int sheepCount;
        int starCount;
};

#endif // LEVELBUILDERXML_H
