#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>
#include "util/TSprite.hpp"

class Background
{
    public:
        Background(TSprite star, TSprite cloud);
        virtual ~Background();

        float getStage();
        void setStage(float stage);

        int getStarCount();
        int getCloudCount();

    private:
        static int MAX_STAR_COUNT;
        static int MAX_CLOUD_COUNT;

        float stage = 0.0f;
        vector<TSprite> stars;
        vector<TSprite> clouds;
        TSprite star;
        TSprite cloud;

        int calcStarCount();
        int calcCloudCount();

        float getLiniar(float value, float min, float max);
};

#endif // BACKGROUND_H
