#include "World.hpp"

#include "../actors/Player.hpp"
#include "../GUI/HUD.hpp"
#include "../scene/StaticCamera.hpp"

World::World()
{
    hud = 0;
    player = 0;
    levelObjects = new list<GameObject*>();
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
}

void World::addLevelObject(GameObject *gameObject)
{
    levelObjects->push_back(gameObject);
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

void World::setBackground(Sprite *background)
{
    this->background = background;
}

void World::update(Clock *clock, RenderWindow *window)
{
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

    if(background != 0)
        window->Draw(*background);

    for(list<GameObject*>::iterator it = levelObjects->begin(); it != levelObjects->end(); it++)
    {
        (*it)->draw(window);
    }

    if(player != 0)
        player->draw(window);

    // Render HUD content with the HUD camera
    hudCamera->activate(window);

    if(hud != 0)
        hud->draw(window);
}
