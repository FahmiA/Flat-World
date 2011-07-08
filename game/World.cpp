#include "World.hpp"

#include "../actors/Player.hpp"

World::World()
{
    this->player = 0;
    levelObjects = new list<GameObject*>();
    camera = 0;
}

World::~World()
{
    delete player;
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
    this->camera = camera;
}

void World::update(Clock *clock, RenderWindow *window)
{
    for(list<GameObject*>::iterator it = levelObjects->begin(); it != levelObjects->end(); it++)
    {
        (*it)->update(clock, window, this);
    }

    if(player != 0)
        player->update(clock, window, this);

    if(camera != 0)
        camera->update(clock, window, this);
}

void World::draw(RenderWindow *window)
{
    for(list<GameObject*>::iterator it = levelObjects->begin(); it != levelObjects->end(); it++)
    {
        (*it)->draw(window);
    }

    if(player != 0)
        player->draw(window);

    if(camera != 0)
        camera->draw(window);
}
