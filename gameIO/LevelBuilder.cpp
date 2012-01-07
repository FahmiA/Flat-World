#include "LevelBuilder.hpp"

#include "../util/SpriteUtil.hpp"

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

bool LevelBuilder::loadSprite(string &path, Sprite &sprite)
{
    Image *image = content->loadImage(path);
    if(image == 0)
        return false;

    sprite.SetImage(*image);

    return true;
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

    // Convert the angle from degrees to radians
    //float angleRadians = DEGREES_TO_RADIANS(angleDegrees);

    // Want to ray-trace beyond the border of the island
    float raytraceDistance = max(island->getSize().x, island->getSize().y);

    // Get the ray-trace 'beam' coordinates
    float toX = island->getSize().x / 2;
    float toY = island->getSize().y / 2;
    float fromX = toX + (cos(angleRadians) * raytraceDistance);
    float fromY = toY + (sin(angleRadians) * raytraceDistance);

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

World* LevelBuilder::getWorld()
{
    return world;
}

ContentManager* LevelBuilder::getContentManager()
{
    return content;
}

