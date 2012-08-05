#include "LevelBuilder.hpp"

#include "util/SpriteUtil.hpp"
#include "util/NumberUtil.hpp"

#include <stdio.h>
#include <iostream>
using namespace std;

LevelBuilder::LevelBuilder(World *world, ContentManager *content)
{
    this->world = world;
    this->content = content;
}

LevelBuilder::~LevelBuilder()
{
    //dtor
}

void LevelBuilder::getPosition(Island *island, float angleRadians, int characterWidth, int characterHeight, int *characterX, int *characterY)
{
    SpriteUtil spriteUtil;

    // Produce a random angle if one does not exist.
    if(angleRadians == -1)
    {
         Randomizer random;
         angleRadians = random.Random(0, M_2_PI);
    }

    // Want to ray-trace beyond the border of the island
    float raytraceDistance = max(island->getSize().x, island->getSize().y);

    // Get the ray-trace 'beam' coordinates
    float toX = island->getSize().x / 2;
    float toY = island->getSize().y / 2;
    float fromX = toX + (cos(angleRadians) * raytraceDistance);
    float fromY = toY + (sin(angleRadians) * raytraceDistance);

    float scaleX = island->getSprite()->GetScale().x;
    float scaleY = island->getSprite()->GetScale().y;
    toX /= scaleX;
    toY /= scaleY;
    fromX /= scaleX;
    fromY /= scaleY;

    // Ray-trace
    Vector2f *localSpawnPos = spriteUtil.rayTrace(island->getSprite(), (int)fromX, (int)fromY, (int)toX, (int)toY, true);

    cout << "Adding object to island at angle: " << angleRadians << endl;
    if(localSpawnPos == 0)
    {
        cout << "\tPosition not set, using default" << endl;
        localSpawnPos = new Vector2f(toX, toY);
    }
    printf("\tRaytrace: (x, y): (%.3f, %0.3f) -> (%.3f, %.3f)\n", fromX, fromY, toX, toY);
    printf("\tSpawn:    (x, y): (%.3f, %0.3f)\n",  localSpawnPos->x,  localSpawnPos->y);

    // Update the charcter position
    Vector2f globalSpawnPos = island->getSprite()->TransformToGlobal(*localSpawnPos);
    *characterX = globalSpawnPos.x;// + characterWidth;
    *characterY = globalSpawnPos.y;// + characterHeight;

    delete localSpawnPos;
}

bool LevelBuilder::loadSprite(string &path, Sprite *sprite)
{
    return spriteUtil.loadSprite(path, sprite, content);
}

bool LevelBuilder::loadFont(string &path, Font *&font)
{
    return spriteUtil.loadFont(path, font, content);
}

/*SpriteUtil& LevelBuilder::getSpriteUtil()
{
    return spriteUtil;
}*/

World* LevelBuilder::getWorld()
{
    return world;
}

ContentManager* LevelBuilder::getContentManager()
{
    return content;
}

void LevelBuilder::addSheepPath(string sheepPath)
{
    sheepPaths.insert(sheepPath);
}

string LevelBuilder::getSheepPath(int index)
{
    clamp(index, 0, sheepPaths.size() - 1);

    int count = 0;
    string sheepPath = "";
    for(set<string>::iterator it = sheepPaths.begin(); it != sheepPaths.end(); it++)
    {
        if(count == index)
        {
            sheepPath = *it;
            break;
        }else{
            count++;
        }
    }
    return sheepPath;
}

int LevelBuilder::getSheepPathCount()
{
    return sheepPaths.size();
}
