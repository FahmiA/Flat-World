#ifndef CHARACTER_H
#define CHARACTER_H

#include "../game/World.hpp"
#include "../environment/Island.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

class Character: public GameObject
{
    public:
        Character(float x, float y, float width, float height, float speed, Sprite *sprite);

        virtual ~Character();

        void update(Clock *clock, RenderWindow *window, World *world);
        void draw(RenderWindow *window);

        virtual void subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world) = 0;

        const Vector2f& getPosition();
        const Vector2f& getSize();
        float getRotation();

    protected:
        Vector2f* rayTrace(Sprite *sprite, int fromX, int fromY, int toX, int toY);
        void landHop();
        void moveLeft();
        void moveRight();
        Sprite *getSprite();

    private:
        float speed;
        Sprite *sprite;
        Shape line;
        Island *currentGround;
        Island *prevGround;
        bool inJump;

        bool doMoveLeft;
        bool doMoveRight;
};

#endif // CHARACTER_H
