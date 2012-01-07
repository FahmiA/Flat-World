#include "HUD.hpp"

#include "../util/NumberUtil.hpp"

#include <algorithm>
using namespace std;

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

    setSize(800, 600);
}

HUD::~HUD()
{
    delete position;
    delete size;
}

void HUD::setSize(int width, int height)
{
    // Set the size and position of the sheep corner
    sheepCorner->Resize(width/3, height/3);
    sheepCorner->SetX(-sheepCorner->GetSize().x/4);
    sheepCorner->SetY(-sheepCorner->GetSize().y/4);

    // Set the size and position of the star corner
    starCorner->Resize(width/3, width/3);
    starCorner->SetX(width);
    starCorner->SetY(-starCorner->GetSize().y/4);
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
    // Nothing needs to be updated
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
