#ifndef WORLD_H
#define WORLD_H

#include "../game/GameObject.hpp"
#include "../scene/Camera.hpp"

class Player;
class HUD;

#include <list>
using namespace std;

class World
{
    public:
        World();
        virtual ~World();

        void addLevelObject(GameObject *gameObject);
        void removeLevelObject(GameObject *gameObject);
        Player* getPlayer();
        void setPlayer(Player *player);

        list<GameObject*>* getObjects();
        void setCamera(Camera *camera);

        HUD* getHud();
        void setHud(HUD *hud);

        void setBackground(Sprite *background);

        void update(Clock *clock, RenderWindow *window);
        void draw(RenderWindow *window);

    private:
        HUD *hud;
        Player *player;
        list<GameObject*> *levelObjects;
        Sprite *background;

        Camera *gameCamera;
        Camera *hudCamera;
};

#endif // WORLD_H
