#ifndef PLAYER_H
#define PLAYER_H

#include "../game/GameObject.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

class Player: public GameObject
{
    public:
        Player(float x, float y, float width, float height, float speed, Sprite *sprite);

        virtual ~Player();

        void update(Clock *clock, RenderWindow *window, World *world);
        void clampCoordinates(Vector2f &origin, Vector2f &target, Sprite *bounds);

        void draw(RenderWindow *window);

    private:
        float speed;
        Sprite *sprite;
        Shape line;

        Vector2f* rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY);
};

#endif // PLAYER_H
