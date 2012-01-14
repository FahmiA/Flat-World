#include "LevelBuilderXML.hpp"

#include "../util/AnimatedSprite.hpp"
#include "../pickups/Star.hpp"
#include "../GUI/HUD.hpp"

#include <stdio.h>
#include <iostream>
using namespace std;

#define SHEEP_WIDTH 40
#define SHEEP_HEIGHT 40
#define SHEEP_SPEED 200

#define DOG_WIDTH 60
#define DOG_HEIGHT 42
#define DOG_SPEED 300

#define STAR_WIDTH 40
#define STAR_HEIGHT 40

#define PLAYER_WIDTH 68
#define PLAYER_HEIGHT 85
#define PLAYER_SPEED 500

//#define RADIANS_TO_DEGREES(radianAngle) (radianAngle * (180.0f / M_PI))
//#define DEGREES_TO_RADIANS(degreeAngle) (degreeAngle * (M_PI / 180.0f))

LevelBuilderXML::LevelBuilderXML(World *world, ContentManager *content)
    : LevelBuilder(world, content)
{
    sheepCount = 0;
    starCount = 0;
}

LevelBuilderXML::~LevelBuilderXML() { }

bool LevelBuilderXML::setBackground(LevelDescription *levelDesc)
{
    bool addSuccess = false;

    Sprite *sprite = new Sprite();
    addSuccess = loadSprite(levelDesc->backgroundPath, *sprite);

    if(addSuccess)
    {
        getWorld()->setBackground(sprite);
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
        getWorld()->addLevelObject(island);
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
    }else if(getWorld()->getPlayer() == 0){
        // No player currently exists
        int x, y;
        getPosition(islandDesc->island, playerDesc->startAngle, PLAYER_WIDTH, PLAYER_HEIGHT, &x, &y);
        Player *player = new Player(x, y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, playerSprite);
        getWorld()->setPlayer(player);
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

        getWorld()->addLevelObject(sheep);
        sheepCount++;
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
        getPosition(islandDesc->island, sheepdogDesc->startAngle, DOG_WIDTH, DOG_HEIGHT, &x, &y);

        sprite->setSpriteSheet(5, DOG_WIDTH, DOG_HEIGHT, 0, 5, 42);
        // Run animation
        sprite->addAnimation(ANIMATE_RUN, 0, 1, 4);
        sprite->play(ANIMATE_RUN);
        sheepdog = new Dog(x, y, DOG_WIDTH, DOG_HEIGHT, DOG_SPEED, sprite);

        getWorld()->addLevelObject(sheepdog);
        addSuccess = true;
    }

    return addSuccess;
}

bool LevelBuilderXML::addStar(PickupDescription *pickupDesc, IslandDescription *islandDesc, float angle)
{
    bool addSuccess = false;
    Sprite *sprite = new Sprite();

    if(!loadSprite(pickupDesc->imagePath, *sprite))
    {
        // Error
        cout << "Image for " << pickupDesc->type << " could not be loaded: " << pickupDesc->imagePath << endl;
        cout << '\t' << pickupDesc->type << " has not been loaded." << endl;
    }else{
        Star *star;
        int x, y;
        getPosition(islandDesc->island, angle, STAR_WIDTH, STAR_HEIGHT, &x, &y);

        star = new Star(x, y, STAR_WIDTH, STAR_HEIGHT, sprite);

        getWorld()->addLevelObject(star);
        starCount++;
        addSuccess = true;
    }

    return addSuccess;
}

bool LevelBuilderXML::setHUD(string &sheepCornerPath, string &starCornerPath,
                             string &sheepIconPath, string &starIconPath,
                             string &fontPath)
{
    bool success = true;

    Sprite *sheepCorner = new Sprite();
    Sprite *starCorner = new Sprite();
    Sprite *sheepIcon = new Sprite();
    Sprite *starIcon = new Sprite();
    Font *textFont = new Font();

    if(!loadSprite(sheepCornerPath, *sheepCorner))
        success = false;
    if(success  && !loadSprite(starCornerPath, *starCorner))
        success = false;
    if(success  && !loadSprite(sheepIconPath, *sheepIcon))
        success = false;
    if(success  && !loadSprite(starIconPath, *starIcon))
        success = false;
    if(success && !loadFont(fontPath, *textFont))
        success = false;

    if(success)
    {
        HUD *hud = new HUD(sheepCorner, starCorner, sheepIcon,
                           starIcon, sheepCount, starCount,
                           textFont);
        getWorld()->setHud(hud);
    }

    return success;
}
