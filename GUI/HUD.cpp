#include "HUD.hpp"

#include "../util/NumberUtil.hpp"

#include <math.h>

HUD::HUD(Sprite *sheepCorner, Sprite *starCorner,
            Sprite *sheepIcon, Sprite *starIcon,
            int sheepTotal, int starTotal)
{
    this->sheepCorner = sheepCorner;
    this->starCorner = starCorner;
    this->sheepIcon = sheepIcon;
    this->starIcon = starIcon;
    this->sheepTotal = sheepTotal;
    this->starTotal = starTotal;

    position = new Vector2f(0, 0);

    int maxWidth = max(sheepCorner->GetPosition().x + sheepCorner->GetSize().x,
                       starCorner->GetPosition().x + starCorner->GetSize().x);
    int maxHeight = max(sheepCorner->GetPosition().y + sheepCorner->GetSize().y,
                       starCorner->GetPosition().y + starCorner->GetSize().y);
    size = new Vector2f(maxWidth, maxHeight);
}

HUD::~HUD()
{
    delete position;
    delete size;
}

void HUD::addSheep(int amount)
{
    // Increment the sheep count
    sheepCount += abs(amount);

    // Ensure that sheepCount does not exceed the maximum number of sheep
    clamp(sheepCount, 0, sheepTotal);
}

void HUD::addStars(int amount)
{
    // Increment the star count
    starCount += abs(amount);

    // Ensure that starCount does not exceed the maximum number of stars
    clamp(starCount, 0, starTotal);
}

void HUD::update(Clock *clock, RenderWindow *window, World *world)
{
}

void HUD::draw(RenderWindow *window)
{
    window->Draw(*sheepCorner);
    window->Draw(*starCorner);
}

const Vector2f& HUD::getPosition()
{
    return *position;
}

const Vector2f& HUD::getSize()
{
    return *size;
}
