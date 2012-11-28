#include "Background.hpp"

#include "util/NumberUtil.hpp"

int Background::MAX_STAR_COUNT = 10;
int Background::MAX_CLOUD_COUNT = 10;

Background::Background(TSprite star, TSprite cloud)
{
    //ctor
}

Background::~Background()
{
    //dtor
}

float Background::getStage()
{
    return stage;
}

void Background::setStage(float stage)
{
    clamp<float>(stage, 0.0f, 1.0f);
    this->stage = stage;
}

int Background::getStarCount()
{
    return 0;
}

int Background::getCloudCount()
{
    return 0;
}

int Background::calcStarCount()
{
    return getLiniar(0.5f - stage, 0.0f, 0.5f);
}

int Background::calcCloudCount()
{
    return getLiniar(0.5f - stage, 0.0f, 0.5f);
}

float Background::getLiniar(float value, float min, float max)
{
    float result =  (max - min) / value;
    clamp<float>(result, 0.0f, 1.0f);

    return result;
}
