#include "LevelBuilderXML.hpp"

#include "../util/SpriteUtil.hpp"
#include "../util/AnimatedSprite.hpp"

#include <stdio.h>
#include <iostream>
using namespace std;

#define SHEEP_WIDTH 40
#define SHEEP_HEIGHT 40
#define SHEEP_SPEED 200

#define DOG_WIDTH 60
#define DOG_HEIGHT 42
#define DOG_SPEED 300

#define PLAYER_WIDTH 68
#define PLAYER_HEIGHT 85
#define PLAYER_SPEED 500

//#define RADIANS_TO_DEGREES(radianAngle) (radianAngle * (180.0f / M_PI))
//#define DEGREES_TO_RADIANS(degreeAngle) (degreeAngle * (M_PI / 180.0f))

LevelBuilderXML::LevelBuilderXML(World *world, ContentManager *content)
{
    this->world = world;
    this->content = content;
}

LevelBuilderXML::~LevelBuilderXML()
{
    //dtor
}

bool LevelBuilderXML::setBackground(LevelDescription *levelDesc)
{
    bool addSuccess = false;

    Sprite *sprite = new Sprite();
    addSuccess = loadSprite(levelDesc->backgroundPath, *sprite);

    if(addSuccess)
    {
        world->setBackground(sprite);
        addSuccess = true;
    }

    return addSuccess;
}

bool LevelBuilderXML::addIsland(IslandDescription *islandDesc)
{
    bool addSuccess = false;
    Sprite *sprite = new Sprite();
    if(!loadSprite(islandDesc->imagePath, *sprite))
    {
        // Error
        cout << "Image for island " << islandDesc->id << " could not be loaded: " << islandDesc->imagePath << endl;
        cout << "\tIsland has not been loaded." << endl;
    }else{
        Island *island = new Island(islandDesc->x, islandDesc->y, islandDesc->width, islandDesc->height, sprite);
        islandDesc->island = island; // Remember the island in the description
        world->addLevelObject(island);
        addSuccess = true;
    }

    return addSuccess;
}

bool LevelBuilderXML::setPlayer(UnitDescription* playerDesc, IslandDescription *islandDesc)
{
    bool addSuccess = false;
    Sprite *playerSprite = new Sprite();

    if(!loadSprite(playerDesc->imagePath, *playerSprite))
    {
        // Error
        cout << "Image for player could not be loaded " << endl;
        cout << "\tGame Exiting" << endl;
    }else if(world->getPlayer() == 0){
        // No player currently exists
        int x, y;
        getPosition(islandDesc->island, playerDesc->startAngle, PLAYER_WIDTH, PLAYER_HEIGHT, &x, &y);
        Player *player = new Player(x, y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, playerSprite);
        world->setPlayer(player);
        addSuccess = true;
    }

    return addSuccess;
}

bool LevelBuilderXML::addSheep(UnitDescription *sheepDesc, IslandDescription *islandDesc)
{
    bool addSuccess = false;
    AnimatedSprite *sprite = new AnimatedSprite();

    if(!loadSprite(sheepDesc->imagePath, *sprite))
    {
        // Error
        cout << "Image for " << sheepDesc->type << " could not be loaded: " << sheepDesc->imagePath << endl;
        cout << '\t' << sheepDesc->type << " has not been loaded." << endl;
    }else{
        Sheep *sheep;
        int x, y;
        getPosition(islandDesc->island, sheepDesc->startAngle, SHEEP_WIDTH, SHEEP_HEIGHT, &x, &y);

        sprite->setSpriteSheet(9, SHEEP_WIDTH, SHEEP_HEIGHT, 1, 0, 40);
        // Walk animation
        sprite->addAnimation(ANIMATE_WALK, 0, 0, 9);
        sprite->play(ANIMATE_WALK);
        sheep = new Sheep(x, y, SHEEP_WIDTH, SHEEP_HEIGHT, SHEEP_SPEED, sprite);

        world->addLevelObject(sheep);
        addSuccess = true;
    }

    return addSuccess;
}

bool LevelBuilderXML::addSheepdog(UnitDescription *sheepdogDesc, IslandDescription *islandDesc)
{
    bool addSuccess = false;
    AnimatedSprite *sprite = new AnimatedSprite();

    if(!loadSprite(sheepdogDesc->imagePath, *sprite))
    {
        // Error
        cout << "Image for " << sheepdogDesc->type << " could not be loaded: " << sheepdogDesc->imagePath << endl;
        cout << '\t' << sheepdogDesc->type << " has not been loaded." << endl;
    }else{
        Dog *sheepdog;
        int x, y;
        getPosition(islandDesc->island, sheepdogDesc->startAngle, SHEEP_WIDTH, SHEEP_HEIGHT, &x, &y);

        sprite->setSpriteSheet(5, DOG_WIDTH, DOG_HEIGHT, 0, 5, 42);
        // Run animation
        sprite->addAnimation(ANIMATE_RUN, 0, 1, 4);
        sprite->play(ANIMATE_RUN);
        sheepdog = new Dog(x, y, DOG_WIDTH, DOG_HEIGHT, DOG_SPEED, sprite);

        world->addLevelObject(sheepdog);
        addSuccess = true;
    }

    return addSuccess;
}

World* LevelBuilderXML::getWorld()
{
    return world;
}

bool LevelBuilderXML::loadSprite(string path, Sprite &sprite)
{
    Image *image = content->loadImage(path);
    if(image == 0)
        return false;

    sprite.SetImage(*image);

    return true;
}

void LevelBuilderXML::getPosition(Island *island, float angleRadians, int characterWidth, int characterHeight, int *characterX, int *characterY)
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
    cout << "Adding unit to island at angle: " << angleRadians << endl;
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
