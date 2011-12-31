#ifndef STAR_H
#define STAR_H

#include "../game/GameObject.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

class Star : public GameObject
{
    public:
        Star(float x, float y, float width, float height, Sprite *sprite);
        virtual ~Star();

        void update(Clock *clock, RenderWindow *window, World *world);
        void draw(RenderWindow *window);

        const Vector2f& getPosition();
        const Vector2f& getSize();
        float getRotation();
        Sprite* getSprite();

    protected:

    private:
        Sprite *sprite;
};

#endif // STAR_H
