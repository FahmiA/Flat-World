#ifndef WORLD_H
#define WORLD_H

#include <list>

#include "GameObject.hpp"
#include "../actors/Player.hpp"

using namespace std;

class World: public GameObject
{
    public:
        World();
        virtual ~World();

        void addLevelObject(GameObject *gameObject);
        void setPlayer(Player *player);

        void update(Clock *clock, RenderWindow *window);
        void draw(RenderWindow *window);

    private:
        Player *player;
        list<GameObject*> *levelObjects;
};

#endif // WORLD_H
