#include "LevelLoader.hpp"

#define TAG_LEVEL "level"
#define TAG_DESCRIPTION "description"
#define TAG_ISLAND "island"
#define TAG_UNIT "unit"
#define TAG_PICKUP "pickup"

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
#define TAG_START_ANGLE "startAngle"
#define TAG_END_ANGLE "endAngle"
#define TAG_COUNT "count"

#define ATR_TYPE "type"
#define ATR_SHEEP "sheep"
#define ATR_SHEEPDOG "sheepdog"
#define ATR_PLAYER "player"
#define ATR_STAR "star"


LevelLoader::LevelLoader(LevelBuilder *levelBuilder, ContentManager *content)
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
    pickupList.clear();
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
        }else if(tag == TAG_PICKUP){
            TiXmlHandle parent = TiXmlHandle(node);
            PickupDescription *pickupDesc = loadPickupDescription(parent);

            if(pickupDesc->type == ATR_STAR)
            {
                // Add the star
                pickupList.push_back(pickupDesc);
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
            levelBuilder->addSheep(unitDesc, islandDesc->island);
        }else if(unitDesc->type.compare(ATR_SHEEPDOG) == 0){
            levelBuilder->addSheepdog(unitDesc, islandDesc);
        }
    }

    // Add the pickups
    for(list<PickupDescription*>::iterator it = pickupList.begin(); it != pickupList.end(); it++)
    {
        PickupDescription *pickupDesc = (*it);
        islandDesc = islandMap[pickupDesc->locationID];

        int pickupCount = pickupDesc->count;
        float sequenceDistance = pickupDesc->endAngle - pickupDesc->startAngle;
        float gap = sequenceDistance / pickupCount;

        for(int i = 0; i < pickupCount; i++)
        {
            float angle = pickupDesc->startAngle + (gap * i);

            if(pickupDesc->type.compare(ATR_STAR) == 0)
            {
                levelBuilder->addStar(pickupDesc, islandDesc, angle);
            }
        }
    }

    // Set the HUD
    string sheepCornerPath = "media\\textures\\HUD\\cloud.png";
    string starCornerPath = "media\\textures\\HUD\\moon.png";
    string sheepIconPath = "media\\textures\\HUD\\sheep_icon.png";
    string starIconPath = "media\\textures\\HUD\\star_icon.png";
    string fontPath = "media\\fonts\\Jura-Bold.ttf";
    levelBuilder->setHUD(sheepCornerPath, starCornerPath,
                         sheepIconPath, starIconPath,
                         fontPath);

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

    TiXmlAttribute *typeAttribute = node.ToElement()->FirstAttribute();
    unit->type = typeAttribute->ValueStr();
    unit->startAngle = getFloat(node, TAG_ANGLE);
    //cout << unit->startAngle << endl;
    // TODO: If no start angle exists, give a random angle.

    return unit;
}

PickupDescription* LevelLoader::loadPickupDescription(TiXmlHandle &node)
{
    PickupDescription *pickupDesc = new PickupDescription();

    // Get the type attribute
    TiXmlAttribute *typeAttribute = node.ToElement()->FirstAttribute();
    pickupDesc->type = typeAttribute->ValueStr();

    // Get the remaining attributes
    pickupDesc->imagePath = getString(node, TAG_IMAGE);
    pickupDesc->locationID = getInt(node, TAG_LOCATION);
    pickupDesc->startAngle = getFloat(node, TAG_START_ANGLE);
    pickupDesc->endAngle = getFloat(node, TAG_END_ANGLE);
    pickupDesc->count = getInt(node, TAG_COUNT);

    return pickupDesc;
}

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
