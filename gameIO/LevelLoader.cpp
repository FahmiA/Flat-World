#include "LevelLoader.hpp"

#include "../util/SpriteUtil.hpp"

#define TAG_LEVEL "level"
#define TAG_DESCRIPTION "description"
#define TAG_ISLAND "island"
#define TAG_SHEEP "sheep"
#define TAG_NAME "name"
#define TAG_WIDTH "width"
#define TAG_HEIGHT "height"
#define TAG_BACKGROUND "background"
#define TAG_PLAYER_START "playerStartID"
#define TAG_LOCATION "locationID"
#define TAG_ID "id"
#define TAG_IMAGE "image"
#define TAG_X "x"
#define TAG_Y "y"

#define SHEEP_WIDTH 103
#define SHEEP_HEIGHT 102
#define SHEEP_SPEED 200

#define PLAYER_WIDTH 68
#define PLAYER_HEIGHT 85
#define PLAYER_SPEED 500


LevelLoader::LevelLoader(string &xmlPath, ContentManager *content)
{
    this->xmlPath = xmlPath;
    this->content = content;
}

LevelLoader::~LevelLoader()
{
    //dtor
}

bool LevelLoader::loadWorld(World* world)
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

    // Declare some storage variables for use later
    LevelDescription *level;
    map<int, IslandDescription*> islandMap;
    list<SheepDescription*> sheepList;

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
        }else if(tag == TAG_SHEEP) {
            SheepDescription *sheep = loadSheepDescription(parent);
            sheepList.push_back(sheep);
        }
    }

    // Construct the level contents
    // Add the background
    Sprite *backgroundSprite = new Sprite();
    Sprite *sprite = new Sprite();
    if(loadSprite(level->backgroundPath, *sprite))
    {
        world->setBackground(sprite);
    }
    // Add the islands
    fillWorldWithIslands(world, islandMap);

    // Add the player
    fillWorldWithPlayer(world, *islandMap[level->playerStartIsland]);

    // Add the sheep
    fillWorldWithSheep(world, sheepList, islandMap);

    delete level;

    return true;
}

LevelDescription* LevelLoader::loadLevelDescription(TiXmlHandle &node)
{
    LevelDescription *level = new LevelDescription();
    level->name = getString(node, TAG_NAME);
    level->width = getInt(node, TAG_WIDTH);
    level->height = getInt(node, TAG_HEIGHT);
    level->backgroundPath = getString(node, TAG_BACKGROUND);
    level->playerStartIsland = getInt(node, TAG_PLAYER_START);

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

SheepDescription* LevelLoader::loadSheepDescription(TiXmlHandle &node)
{
    SheepDescription *sheep = new SheepDescription();
    sheep->imagePath = getString(node, TAG_IMAGE);
    sheep->locationID = getInt(node, TAG_LOCATION);

    return sheep;
}

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

void LevelLoader::fillWorldWithSheep(World *world, list<SheepDescription*> &sheepList, map<int, IslandDescription*> &islandMap)
{
    for(list<SheepDescription*>::iterator it = sheepList.begin(); it != sheepList.end(); it++)
    {
        SheepDescription *sheepDesc = (*it);
        IslandDescription *islandDesc = islandMap[sheepDesc->locationID];

        Sprite *sprite = new Sprite();
        if(!loadSprite(sheepDesc->imagePath, *sprite))
        {
            // Error
            cout << "Image for sheep could not be loaded: " << sheepDesc->imagePath << endl;
            cout << "\tSheep has not been loaded." << endl;
        }else{
            int x, y;
            getPosition(islandDesc->island, SHEEP_WIDTH, SHEEP_HEIGHT, &x, &y);
            Sheep *sheep = new Sheep(x, y, SHEEP_WIDTH, SHEEP_HEIGHT, SHEEP_SPEED, sprite);
            world->addLevelObject(sheep);
        }
    }
}

void LevelLoader::fillWorldWithPlayer(World *world, IslandDescription &playerStartIsland)
{
    Sprite *playerSprite = new Sprite();
    if(!loadSprite(string("media/textures/CharacterBoy.png"), *playerSprite))
    {
        // Error
        cout << "Image for player could not be loaded " << endl;
        cout << "\tGame Exiting" << endl;
        exit(1);
    }else{
        int x, y;
        getPosition(playerStartIsland.island, PLAYER_WIDTH, PLAYER_HEIGHT, &x, &y);
        Player *player = new Player(x, y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, playerSprite);
        world->setPlayer(player);
    }
}

void LevelLoader::getPosition(Island *island, int characterWidth, int characterHeight, int *characterX, int *characterY)
{
    SpriteUtil spriteUtil;
    Randomizer random;

    int angle = random.Random(0, 360);
    int raytraceDistance = max(island->getSize().x, island->getSize().y) * 2;

    int fromX = island->getSize().x/2;
    int fromY = island->getSize().y/2;
    int toX = fromX + (cos(angle) * raytraceDistance);
    int toY = fromY + (sin(angle) * raytraceDistance);

    Vector2f *position = spriteUtil.rayTrace(island->getSprite(), fromX, fromY, toX, toY);

    //*characterX = island->getPosition().x;
    //*characterY = island->getPosition().y;
    *characterX = island->getPosition().x + position->x/2;
    *characterY = island->getPosition().y + position->y/2 ;

    delete position;
}

string LevelLoader::getString(TiXmlHandle &parent, char* tag)
{
    return parent.FirstChildElement(tag).ToElement()->GetText();
}

int LevelLoader::getInt(TiXmlHandle &parent, char* tag)
{
    string text = getString(parent, tag);
    return atoi(text.c_str());
}

bool LevelLoader::loadSprite(string path, Sprite &sprite)
{
    Image *image = content->loadImage(path);
    if(image == 0)
        return false;

    sprite.SetImage(*image);

    return true;
}
