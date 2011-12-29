#include "LevelLoader.hpp"

#define TAG_LEVEL "level"
#define TAG_DESCRIPTION "description"
#define TAG_ISLAND "island"
#define TAG_UNIT "unit"
#define TAG_NAME "name"
#define TAG_WIDTH "width"
#define TAG_HEIGHT "height"
#define TAG_ANGLE "angle"
#define TAG_BACKGROUND "background"
#define TAG_PLAYER_START "playerStartID"
#define TAG_LOCATION "locationID"
#define TAG_ID "id"
#define TAG_IMAGE "image"
#define TAG_X "x"
#define TAG_Y "y"

#define ATR_TYPE "type"
#define ATR_SHEEP "sheep"
#define ATR_SHEEPDOG "sheepdog"
#define ATR_PLAYER "player"


LevelLoader::LevelLoader(LevelBuilderXML *levelBuilder, ContentManager *content)
{
    this->levelBuilder = levelBuilder;
    this->content = content;

    level = 0;
    player = 0;
}

LevelLoader::~LevelLoader()
{
    clear();
}

bool LevelLoader::loadWorld(string &xmlPath)
{
    // Clear temporary variables
    clear();

    // Load descriptions from the XML file
    bool xmlLoaded = loadXML(xmlPath);

    // Build the world
    bool worldBuilt =  buildWorld();
    cout << "xmlLoaded: " << xmlLoaded  << endl;
    cout << "worldBuilt: " << worldBuilt << endl;
    if(xmlLoaded && worldBuilt)
        return true;

    return false;
}

void LevelLoader::clear()
{
    if(level != 0)
    {
        delete level;
        level = 0;
    }

    if(player != 0)
    {
        delete player;
        player = 0;
    }

    islandMap.clear();

    unitList.clear();
}

bool LevelLoader::loadXML(string &xmlPath)
{
    // Open the xml file for reading
    TiXmlDocument levelDoc(xmlPath);
    bool loadOK = levelDoc.LoadFile();

    if(!loadOK)
        return false;

    TiXmlHandle levelDocHandle(&levelDoc);
    TiXmlElement *element;
    TiXmlHandle root(0);

    // Load the root
    element = levelDocHandle.FirstChildElement(TAG_LEVEL).Element();
    if(!element)
        return false;
    root = TiXmlHandle(element);

    // Load the level contents
    TiXmlElement* node = root.FirstChild().ToElement();
    for(node; node; node = node->NextSiblingElement())
    {
        string tag = node->ValueStr();
        TiXmlHandle parent = TiXmlHandle(node);

        //cout << "Node: " << tag << endl;
        if(tag == TAG_DESCRIPTION)
        {
            level = loadLevelDescription(parent);
        }else if(tag == TAG_ISLAND){
            IslandDescription *island = loadIslandDescription(parent);
            islandMap[island->id] = island;
        }else if(tag == TAG_UNIT) {
            UnitDescription *unit = loadUnitDescription(parent);
            if(unit->type == ATR_PLAYER)
            {
                // Set the player
                player = unit;
            }else if(unit->type == ATR_SHEEP || unit->type == ATR_SHEEPDOG)
            {
                // Add the sheep or sheepdog
                unitList.push_back(unit);
            }
        }
    }

    return true;
}

bool LevelLoader::buildWorld()
{
    if(player == 0 || level == 0)
        return false;

    IslandDescription *islandDesc;

    // Set the background
    levelBuilder->setBackground(level);

    // Add the islands
    for(map<int,IslandDescription*>::iterator it = islandMap.begin(); it != islandMap.end(); it++)
    {
        islandDesc = (*it).second;
        levelBuilder->addIsland(islandDesc);
    }

    // Set the player
    islandDesc = islandMap[player->locationID];
    levelBuilder->setPlayer(player, islandDesc);

    // Add the NPC units
    for(list<UnitDescription*>::iterator it = unitList.begin(); it != unitList.end(); it++)
    {
        UnitDescription *unitDesc = (*it);
        islandDesc = islandMap[unitDesc->locationID];

        if(unitDesc->type.compare(ATR_SHEEP) == 0)
        {
           levelBuilder->addSheep(unitDesc, islandDesc);
        }else if(unitDesc->type.compare(ATR_SHEEPDOG) == 0){
             levelBuilder->addSheepdog(unitDesc, islandDesc);
        }
    }

    return true;
}

LevelDescription* LevelLoader::loadLevelDescription(TiXmlHandle &node)
{
    LevelDescription *level = new LevelDescription();
    level->name = getString(node, TAG_NAME);
    level->width = getInt(node, TAG_WIDTH);
    level->height = getInt(node, TAG_HEIGHT);
    level->backgroundPath = getString(node, TAG_BACKGROUND);

    return level;
}

IslandDescription* LevelLoader::loadIslandDescription(TiXmlHandle &node)
{
    IslandDescription *island = new IslandDescription();
    island->id = getInt(node, TAG_ID);
    island->imagePath = getString(node, TAG_IMAGE);
    island->x = getInt(node, TAG_X);
    island->y = getInt(node, TAG_Y);
    island->width = getInt(node, TAG_WIDTH);
    island->height = getInt(node, TAG_HEIGHT);

    return island;
}

UnitDescription* LevelLoader::loadUnitDescription(TiXmlHandle &node)
{
    UnitDescription *unit = new UnitDescription();
    unit->imagePath = getString(node, TAG_IMAGE);
    unit->locationID = getInt(node, TAG_LOCATION);

    TiXmlAttribute* typeAttribute = node.ToElement()->FirstAttribute();
    unit->type = typeAttribute->ValueStr();
    unit->startAngle = getFloat(node, TAG_ANGLE);
    //cout << unit->startAngle << endl;
    // TODO: If no start angle exists, give a random angle.

    return unit;
}
/*
void LevelLoader::fillWorldWithIslands(World *world, map<int, IslandDescription*> &islandMap)
{
    for(map<int,IslandDescription*>::iterator it = islandMap.begin(); it != islandMap.end(); it++)
    {
        //cout << (*it).first << " => " << (*it).second << endl;
        IslandDescription *islandDesc = (*it).second;

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
        }
    }
}

void LevelLoader::fillWorldWithUnits(World *world, list<UnitDescription*> &unitList, map<int, IslandDescription*> &islandMap)
{
    for(list<UnitDescription*>::iterator it = unitList.begin(); it != unitList.end(); it++)
    {
        UnitDescription *unitDesc = (*it);
        IslandDescription *islandDesc = islandMap[unitDesc->locationID];

        AnimatedSprite *sprite = new AnimatedSprite();
        if(!loadSprite(unitDesc->imagePath, *sprite))
        {
            // Error
            cout << "Image for " << unitDesc->type << " could not be loaded: " << unitDesc->imagePath << endl;
            cout << '\t' << unitDesc->type << " has not been loaded." << endl;
        }else{
            Character *unit = 0;
            int x, y;
            getPosition(islandDesc->island, unitDesc->startAngle, SHEEP_WIDTH, SHEEP_HEIGHT, &x, &y);

            if(unitDesc->type.compare(ATR_SHEEP) == 0)
            {
                sprite->setSpriteSheet(9, SHEEP_WIDTH, SHEEP_HEIGHT, 1, 0, 40);
                // Walk animation
                sprite->addAnimation(ANIMATE_WALK, 0, 0, 9);
                sprite->play(ANIMATE_WALK);
                unit = new Sheep(x, y, SHEEP_WIDTH, SHEEP_HEIGHT, SHEEP_SPEED, sprite);
            }else if(unitDesc->type.compare(ATR_SHEEPDOG) == 0)
            {
                sprite->setSpriteSheet(5, DOG_WIDTH, DOG_HEIGHT, 0, 5, 42);
                // Run animation
                sprite->addAnimation(ANIMATE_RUN, 0, 1, 4);
                sprite->play(ANIMATE_RUN);
                unit = new Dog(x, y, DOG_WIDTH, DOG_HEIGHT, DOG_SPEED, sprite);
            }

            if(unit != 0)
                world->addLevelObject(unit);
        }
    }
}

void LevelLoader::fillWorldWithPlayer(World *world, IslandDescription &playerStartIsland, UnitDescription* player)
{
    Sprite *playerSprite = new Sprite();
    //if(!loadSprite(string("media/textures/CharacterBoy.png"), *playerSprite))

    if(!loadSprite(player->imagePath, *playerSprite))
    {
        // Error
        cout << "Image for player could not be loaded " << endl;
        cout << "\tGame Exiting" << endl;
        exit(1);
    }else{
        int x, y;
        getPosition(playerStartIsland.island, player->startAngle, PLAYER_WIDTH, PLAYER_HEIGHT, &x, &y);
        Player *player = new Player(x, y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, playerSprite);
        world->setPlayer(player);
    }
}

void LevelLoader::getPosition(Island *island, float angleRadians, int characterWidth, int characterHeight, int *characterX, int *characterY)
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
}*/

string LevelLoader::getString(TiXmlHandle &parent, char* tag)
{
    TiXmlElement* element = parent.FirstChildElement(tag).ToElement();
    if(element)
        return element->GetText();
    else
        return "-1\0";
}

int LevelLoader::getInt(TiXmlHandle &parent, char* tag)
{
    string text = getString(parent, tag);
    return atoi(text.c_str());
}

float LevelLoader::getFloat(TiXmlHandle &parent, char* tag)
{
    string text = getString(parent, tag);
    return atof(text.c_str());
}
