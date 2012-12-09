#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>
using namespace std;

#include "util/TSprite.hpp"
#include "game/GameObject.hpp"

class Background : public GameObject
{
    public:
        Background(TSprite star, TSprite cloud);
        virtual ~Background();

        float getStage();
        void setStage(float stage);

        int getStarCount();
        int getCloudCount();

        const Vector2f& getPosition();
        const Vector2f& getSize();

        void update(Clock *clock, RenderWindow *window, World *world);
        void draw(RenderWindow *window);


    private:
        static int MAX_STAR_COUNT;
        static int MAX_CLOUD_COUNT;
        /** Multipled with star and cloud movement speed to give a paralax effect. */
        static float PARALAX_LAG;

        float stage = 0.0f;
        vector<TSprite> stars;
        vector<TSprite> clouds;
        TSprite prototypeStar;
        TSprite prototypeCloud;
        RectangleShape backDrop;
        Vector2f backDropPrevPos;

        int calcStarCount();
        int calcCloudCount();

        float getLiniar(float value, float min, float max);
        Vector2f getRelativeCoords(float x, float y);
        void populate(vector<TSprite> &items, TSprite &prototypeItem,
                      int desiredCOunt);
        void itemUpdate(vector<TSprite> &items, Vector2f &moveDelta);
};

#endif // BACKGROUND_H