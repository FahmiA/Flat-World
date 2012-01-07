#ifndef LEVELBUILDERXML_H
#define LEVELBUILDERXML_H

#include "LevelBuilder.hpp"

class LevelBuilderXML : public LevelBuilder
{
    public:
        // Construct and Destruct
        LevelBuilderXML(World *world, ContentManager *content);
        virtual ~LevelBuilderXML();

        // Buider methods

        bool setBackground(LevelDescription *levelDesc);
        bool addIsland(IslandDescription *islandDesc);
        bool setPlayer(UnitDescription* playerDesc, IslandDescription *islandDesc);
        bool addSheep(UnitDescription *sheepDesc, IslandDescription *islandDesc);
        bool addSheepdog(UnitDescription *sheepdogDesc, IslandDescription *islandDesc);
        bool addStar(PickupDescription *pickupDesc, IslandDescription *islandDesc,
                     float angle);
        bool setHUD(string &sheepCornerPath, string &starCornerPath,
                    string &sheepIconPath, string &starIconPath);

    private:
        int sheepCount;
        int starCount;
};

#endif // LEVELBUILDERXML_H
