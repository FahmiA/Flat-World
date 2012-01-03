#ifndef HUD_H
#define HUD_H

#include "../game/World.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

/** The Heads-Up-Display (HUD).
  * An overlay on top of the game that displays, to the user, information
  * about the state of the game. This includes the number of sheep captured
  * and the number of stars captured, as well as the total number of sheep
  * and stars in the current level.
  */
class HUD : public GameObject
{
    public:
        HUD(Sprite *sheepCorner, Sprite *starCorner,
            Sprite *sheepIcon, Sprite *starIcon,
            int sheepTotal, int starTotal);
        virtual ~HUD();

        void addSheep(int amount);
        void addStars(int amount);

        int getSheepTotal();
        int getSheepCount();

        int getStarTotoal();
        int getStarCount();

        void update(Clock *clock, RenderWindow *window, World *world);
        void draw(RenderWindow *window);

        const Vector2f& getPosition();
        const Vector2f& getSize();

    private:
        int sheepTotal;
        int sheepCount;

        int starTotal;
        int starCount;

        Sprite *sheepCorner;
        Sprite *starCorner;
        Sprite *sheepIcon;
        Sprite *starIcon;

        Vector2f *position;
        Vector2f *size;
};

#endif // HUD_H
