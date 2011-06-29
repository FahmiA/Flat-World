#include "World.hpp"

World::World()
{
    this->player = 0;
    levelObjects = new list<GameObject*>();
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

void World::setPlayer(Player *player)
{
    this->player = player;
}

list<GameObject*>* World::getObjects()
{
    return levelObjects;
}

void World::update(Clock *clock, RenderWindow *window, World *world)
{
    for(list<GameObject*>::iterator it = levelObjects->begin(); it != levelObjects->end(); it++)
    {
        (*it)->update(clock, window, this);
    }

    if(player != 0)
        player->update(clock, window, this);

}

void World::draw(RenderWindow *window)
{
    for(list<GameObject*>::iterator it = levelObjects->begin(); it != levelObjects->end(); it++)
    {
        (*it)->draw(window);
    }

    if(player != 0)
        player->draw(window);
}
