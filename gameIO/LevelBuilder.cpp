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
    float fromX = island->getSize().x / 2;
    float fromY = island->getSize().y / 2;
    float toX = fromX + (cos(angleRadians) * raytraceDistance);
    float toY = fromY + (sin(angleRadians) * raytraceDistance);

    //cout << cos(angleRadians) << endl;
    // Ray-trace
    Vector2f *localSpawnPos = spriteUtil.rayTrace(island->getSprite(), (int)fromX, (int)fromY, (int)toX, (int)toY);
    cout << "Adding object to island at angle: " << angleRadians << endl;
    printf("  Raytrace: (x, y): (%.3f, %0.3f) -> (%.3f, %.3f)\n", fromX, fromY, toX, toY);
    printf("  Spawn:    (x, y): (%.3f, %0.3f)\n",  localSpawnPos->x,  localSpawnPos->y);
    if(localSpawnPos == 0)
    {
        cout << "\tPosition not set, using default" << endl;
        localSpawnPos = new Vector2f(toX/2, toY/2);
    }

    // Update the charcter position
    Vector2f globalSpawnPos = island->getSprite()->TransformToGlobal(*localSpawnPos);
    *characterX = globalSpawnPos.x;
    *characterY = globalSpawnPos.y;

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
