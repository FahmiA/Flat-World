#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "../tinyxml/tinyxml.h"
#include "../game/World.hpp"
#include "../gameIO/ContentManager.hpp"
#include "../environment/Island.hpp"
#include "../actors/Sheep.hpp"
#include "../actors/Player.hpp"

#include <map>
#include <list>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;

struct LevelDescription;
struct IslandDescription;
struct SheepDescription;

class Island;
class Sheep;

class LevelLoader
{
    public:
        LevelLoader(string &xmlPath, ContentManager *content);
        virtual ~LevelLoader();

        bool loadWorld(World* world);

    private:
        string xmlPath;
        ContentManager *content;

        // Parseing helper methods
        LevelDescription* loadLevelDescription(TiXmlHandle &node);
        IslandDescription* loadIslandDescription(TiXmlHandle &node);
        SheepDescription* loadSheepDescription(TiXmlHandle &node);
        /** Loads Islands into a world. */
        void fillWorldWithIslands(World *world, map<int, IslandDescription*> &islandMap);
        /** Loads Sheep into a world. */
        void fillWorldWithSheep(World *world, list<SheepDescription*> &sheepList, map<int, IslandDescription*> &islandMap);
        /** Loads a single Player into a world. */
        void fillWorldWithPlayer(World *world, IslandDescription &playerStartIsland);
        /** Determins the position of an object on an Island. */
        void getPosition(Island *island, int characterWidth, int characterHeight, int *characterX, int *characterY);

        // XML helper methods
        string getString(TiXmlHandle &parent, char* tag);
        int getInt(TiXmlHandle &parent, char* tag);
        /** Loads an aribitrary image into a Sprite. */
        bool loadSprite(string path, Sprite &sprite);
};

struct LevelDescription
{
    string name;
    int width;
    int height;
    string backgroundPath;
    int playerStartIsland;
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

struct SheepDescription
{
    string imagePath;
    int locationID;
};

#endif // LEVELLOADER_H
