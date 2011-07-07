#ifndef WORLD_H
#define WORLD_H

#include "../game/GameObject.hpp"
#include "../actors/Player.hpp"
#include "../scene/Camera.hpp"

class Player;

#include <list>
using namespace std;

class World
{
    public:
        World();
        virtual ~World();

        void addLevelObject(GameObject *gameObject);
        Player* getPlayer();
        void setPlayer(Player *player);

        list<GameObject*>* getObjects();
        void setCamera(Camera *camera);

        void update(Clock *clock, RenderWindow *window);
        void draw(RenderWindow *window);

    private:
        Player *player;
        list<GameObject*> *levelObjects;

        Camera *camera;
};

#endif // WORLD_H
