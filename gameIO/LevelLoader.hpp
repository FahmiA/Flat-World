#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "tinyxml/tinyxml.h"
#include "gameIO/ContentManager.hpp"
#include "LevelBuilder.hpp"
#include "util/XMLUtil.hpp"

#include <map>
#include <list>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

/** Constructs a game world based on an XML document.
 * Director in the Builder design pattern.
 */
class LevelLoader
{
    public:
        /** Creates a new LevelLoader.
         * @param levelBuilder The LevelBuilder to use to construct the desired type of world.
         * @param content The ContentManager to use for loading external assets.
         */
        LevelLoader(LevelBuilder *levelBuilder, ContentManager *content);
        virtual ~LevelLoader();

        /** Constructs a game world as described by an XML document. */
        bool loadWorld(string &xmlPath);

    private:
        // Constant variables
        ContentManager *content;
        LevelBuilder *levelBuilder;
        XMLUtil xmlUtil;

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
};


#endif // LEVELLOADER_H
