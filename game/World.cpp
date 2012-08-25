#include "World.hpp"

#include "actors/Player.hpp"
#include "GUI/HUD.hpp"
#include "scene/StaticCamera.hpp"

World::World()
{
    hud = 0;
    player = 0;
    levelObjects = new list<GameObject*>();
    islands = new list<Island*>();
    gameCamera = 0;
    hudCamera = new StaticCamera();
    background = 0;
}

World::~World()
{
    delete hud;
    delete player;
    delete background;
    delete levelObjects;
    delete islands;
}

void World::addLevelObject(GameObject *gameObject)
{
    levelObjects->push_back(gameObject);
}

void World::removeLevelObject(GameObject *gameObject)
{
    levelObjects->remove(gameObject);
}

void World::addIsland(Island *island)
{
    islands->push_back(island);
}

void World::removeIsland(Island *island)
{
    islands->remove(island);
}

Player* World::getPlayer()
{
    return player;
}

void World::setPlayer(Player *player)
{
    this->player = player;
}

list<GameObject*>* World::getObjects()
{
    return levelObjects;
}

list<Island*>* World::getIslands()
{
    return islands;
}

void World::setCamera(Camera *camera)
{
    gameCamera = camera;
}

HUD* World::getHud()
{
    return hud;
}

void World::setHud(HUD *hud)
{
    this->hud = hud;
}

int World::getSheepCaptured()
{
    int count = -1;
    if(hud != 0)
        count = hud->getSheepCount();

    return count;
}

void World::setBackground(Sprite *background)
{
    this->background = background;
}

void World::update(Clock *clock, RenderWindow *window)
{
    for(list<Island*>::iterator it = islands->begin(); it != islands->end(); it++)
    {
        (*it)->update(clock, window, this);
    }

    for(list<GameObject*>::iterator it = levelObjects->begin(); it != levelObjects->end(); it++)
    {
        (*it)->update(clock, window, this);
    }

    if(player != 0)
        player->update(clock, window, this);

    if(gameCamera != 0)
        gameCamera->update(clock, window, this);

    if(hud != 0)
        hud->update(clock, window, this);
}

void World::draw(RenderWindow *window)
{
    // Render game content with the game camera
    gameCamera->activate(window);

    // draw the backround
    if(background != 0)
        window->draw(*background);

    // draw the islands
    for(list<Island*>::iterator it = islands->begin(); it != islands->end(); it++)
    {
        (*it)->draw(window);
    }

    // draw the game objects
    for(list<GameObject*>::iterator it = levelObjects->begin(); it != levelObjects->end(); it++)
    {
        (*it)->draw(window);
    }

    // draw the player
    if(player != 0)
        player->draw(window);

    // Render HUD content with the HUD camera
    hudCamera->activate(window);

    // draw the HUD
    if(hud != 0)
        hud->draw(window);
}
