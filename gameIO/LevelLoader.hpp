#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "../tinyxml/tinyxml.h"
#include "../gameIO/ContentManager.hpp"
#include "LevelBuilder.hpp"

#include <map>
#include <list>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

class LevelLoader
{
    public:
        LevelLoader(LevelBuilder *levelBuilder, ContentManager *content);
        virtual ~LevelLoader();

        bool loadWorld(string &xmlPath);

    private:
        // Constant variables
        ContentManager *content;
        LevelBuilder *levelBuilder;

        // Temporary variables
        LevelDescription *level;
        UnitDescription *player;
        map<int, IslandDescription*> islandMap; // Island ID -> Island Description
        list<UnitDescription*> unitList;
        list<PickupDescription*> pickupList;

        // Steps for loading a world
        void clear();
        bool loadXML(string &xmlPath);
        bool buildWorld();

        // Parseing helper methods
        LevelDescription* loadLevelDescription(TiXmlHandle &node);
        IslandDescription* loadIslandDescription(TiXmlHandle &node);
        UnitDescription* loadUnitDescription(TiXmlHandle &node);
        PickupDescription* loadPickupDescription(TiXmlHandle &node);

        // XML helper methods
        string getString(TiXmlHandle &parent, char* tag);
        int getInt(TiXmlHandle &parent, char* tag);
        float getFloat(TiXmlHandle &parent, char* tag);
};


#endif // LEVELLOADER_H
