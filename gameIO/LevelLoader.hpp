#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "../tinyxml/tinyxml.h"
#include "../game/World.hpp"
#include "../gameIO/ContentManager.hpp"
#include "LevelBuilderXML.hpp"

#include <map>
#include <list>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

class Island;
class Sheep;

class LevelLoader
{
    public:
        LevelLoader(LevelBuilderXML *levelBuilder, ContentManager *content);
        virtual ~LevelLoader();

        bool loadWorld(string &xmlPath);

    private:
        // Constant variables
        ContentManager *content;
        LevelBuilderXML *levelBuilder;

        // Temporary variables
        LevelDescription *level;
        UnitDescription *player;
        map<int, IslandDescription*> islandMap; // Island ID -> Island Description
        list<UnitDescription*> unitList;

        // Steps for loading a world
        void clear();
        bool loadXML(string &xmlPath);
        bool buildWorld();

        // Parseing helper methods
        LevelDescription* loadLevelDescription(TiXmlHandle &node);
        IslandDescription* loadIslandDescription(TiXmlHandle &node);
        UnitDescription* loadUnitDescription(TiXmlHandle &node);

        // XML helper methods
        string getString(TiXmlHandle &parent, char* tag);
        int getInt(TiXmlHandle &parent, char* tag);
        float getFloat(TiXmlHandle &parent, char* tag);
};


#endif // LEVELLOADER_H
