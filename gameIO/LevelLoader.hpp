#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "../tinyxml/tinyxml.h"
#include "../game/World.hpp"
#include "../gameIO/ContentManager.hpp"

#include <string>
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

        LevelDescription* loadLevelDescription(TiXmlHandle &node);
        IslandDescription* loadIslandDescription(TiXmlHandle &node);
        SheepDescription* loadSheepDescription(TiXmlHandle &node);

        // Helper methods
        string getString(TiXmlHandle &parent, char* tag);
        int getInt(TiXmlHandle &parent, char* tag);

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
    Sheep *sheep;
};

#endif // LEVELLOADER_H
