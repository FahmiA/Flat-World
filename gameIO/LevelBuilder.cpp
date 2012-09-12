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
    // Produce a random angle if one does not exist.
    if(angleRadians == -1)
    {
         angleRadians = rand() % 6; // 6 is the closest int to M_2_PI
    }

    // Want to ray-trace beyond the border of the island
    FloatRect islandBounds = island->getSprite()->getLocalBounds();
    float raytraceDistance = max(islandBounds.width, islandBounds.height);

    // Get the ray-trace 'beam' coordinates
    float toX =  islandBounds.width / 1.5f; // Quick fix for island_1 which is transparent in the center
    float toY =  islandBounds.height / 1.5f;
    float fromX = toX + (cos(angleRadians) * raytraceDistance);
    float fromY = toY + (sin(angleRadians) * raytraceDistance);

    /*float scaleX = island->getSprite()->getScale().x;
    float scaleY = island->getSprite()->getScale().y;
    toX /= scaleX;
    toY /= scaleY;
    fromX /= scaleX;
    fromY /= scaleY;*/

    // Ray-trace
    Vector2f *localSpawnPos = SpriteUtil::rayTrace(*island->getImage(), (int)fromX, (int)fromY, (int)toX, (int)toY, true);

    cout << "Adding object to island at angle: " << angleRadians << endl;
    if(localSpawnPos == 0)
    {
        cout << "\tPosition not set, using default" << endl;
        localSpawnPos = new Vector2f(toX, toY);
    }
    /*float scaleX = island->getSprite()->getScale().x;
    float scaleY = island->getSprite()->getScale().y;
    localSpawnPos->x /= scaleX;
    localSpawnPos->y /= scaleY;*/
    printf("\tRaytrace: (x, y): (%.3f, %0.3f) -> (%.3f, %.3f)\n", fromX, fromY, toX, toY);
    printf("\tSpawn:    (x, y): (%.3f, %0.3f)\n",  localSpawnPos->x,  localSpawnPos->y);

    // Update the charcter position
    Transform islandGlobalTransform = island->getSprite()->getTransform();
    Vector2f globalSpawnPos = islandGlobalTransform.transformPoint(*localSpawnPos);
    *characterX = globalSpawnPos.x;// + characterWidth;
    *characterY = globalSpawnPos.y;// + characterHeight;

    delete localSpawnPos;
}

bool LevelBuilder::loadFont(string &path, Font *&font)
{
    return SpriteUtil::loadFont(path, font, content);
}

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
