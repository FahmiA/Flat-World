#ifndef HUD_H
#define HUD_H

#include "game/World.hpp"

#include "util/AnimatedSprite.hpp"

/** The Heads-Up-Display (HUD).
  * An overlay on top of the game that displays, to the user, information
  * about the state of the game. This includes the number of sheep captured
  * and the number of stars captured, as well as the total number of sheep
  * and stars in the current level.
  */
class HUD : public GameObject
{
    public:
        /** Creates a HUD.
         * @param sheepCorner The background Sprite to display behind sheep-related information,
         * @param starCorner The background Sprite to display behind star-related information,
         * @param sheepIcon The Sprite that represents sheep.
         * @param starIcon The Sprite that represents stars.
         * @param sheepTotal The total number of sheep in the current level,
         * @param starTotal The total number of stars in the current level.
         * @param textFont The font to use to display textual information.
         */
        HUD(AnimatedSprite *sheepCorner, AnimatedSprite *starCorner,
            AnimatedSprite *sheepIcon, AnimatedSprite *starIcon,
            int sheepTotal, int starTotal,
            Font *textFont);
        virtual ~HUD();

        /** Sets the size of the HUD.
         * Ssould be the size of the game window.
         */
        void setSize(int width, int height);

        /** Mark a number of sheep as having been captured by the player.
         * @param amount Number of sheep captured.
         */
        void addSheep(int amount);

        /** Mark a number of stars as having been captured by the player.
         * @param amount Number of stars captured.
         */
        void addStars(int amount);

        /** Mark a number of sheep as having been lost by the player.
         * Lost sheep are sheep that were captured but the player has lost,
         * @param amount Number of sheep lost.
         */
        void removeSheep(int amount);


        /** Mark a number of stars as having been lost by the player.
         * Lost stars are stars that were captured but the player has lost,
         * @param amount Number of stars lost.
         */
        void removeStars(int amount);

        /** Gets the total number of sheep in the current level.
         * @return Sheep count.
         */
        int getSheepTotal();

        /** Gets the number of sheep the player has currently captured.
         * @return Sheep count.
         */
        int getSheepCount();

        /** Gets the total number of starts in the current level.
         * @return Star count.
         */
        int getStarTotal();

        /** Gets the number of stars the player has currently captured.
         * @return Star count.
         */
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

        AnimatedSprite *sheepCorner;
        AnimatedSprite *starCorner;
        AnimatedSprite *sheepIcon;
        AnimatedSprite *starIcon;

        Text *sheepText;
        Text *starText;
        Font *textFont;

        Vector2f *position;
        Vector2f *size;

        /** Creates a ratio string in the form of
         * "count / total".
         * @param count The "count: part of the string.
         * @param total The "total" part of the string.
         * @return A string in the form of "count / total".
         */
        string get_ratio_string(int count, int total);


};

#endif // HUD_H
