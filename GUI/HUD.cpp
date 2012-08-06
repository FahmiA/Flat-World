#include "HUD.hpp"

#include "util/NumberUtil.hpp"

#include <algorithm>
#include <sstream>
using namespace std;

HUD::HUD(Sprite *sheepCorner, Sprite *starCorner,
            Sprite *sheepIcon, Sprite *starIcon,
            int sheepTotal, int starTotal,
            Font *textFont)
{
    this->sheepCorner = sheepCorner;
    this->starCorner = starCorner;
    this->sheepIcon = sheepIcon;
    this->starIcon = starIcon;
    this->sheepTotal = sheepTotal;
    this->starTotal = starTotal;
    this->textFont = textFont;

    sheepCount = 0;
    sheepTotal = 0;
    starCount = 0;
    starTotal = 0;

    position = new Vector2f(0, 0);
    size = new Vector2f(800, 600);

    setSize(800, 600);
}

HUD::~HUD()
{
    delete sheepText;
    delete starText;

    delete position;
    delete size;
}

void HUD::setSize(int width, int height)
{
    float icon_width = width / 12.0f;
    float icon_height = icon_width;

    // Set the size and position of the sheep corner
    sheepCorner->Resize(width/3, height/3);
    int sheepCornerX = sheepCorner->getPosition().x;
    int sheepCornerY = sheepCorner->getPosition().y;
    int sheepCornerWidth = sheepCorner->GetSize().x;
    int sheepCornerHeight = sheepCorner->GetSize().y;
    sheepCorner->SetX(-sheepCornerWidth/4);
    sheepCorner->SetY(-sheepCornerHeight/4);

    // Set the size and position of the sheep icon
    float top_align = sheepCornerY + sheepCornerHeight/4.5;
    sheepIcon->Resize(icon_width, icon_width);
    sheepIcon->SetX(sheepCornerX + sheepCornerWidth/2.5);
    sheepIcon->SetY(top_align);

    // Set the size and position of the star corner
    starCorner->Resize(width/3, width/3);
    int starCornerX = starCorner->GetPosition().x;
    int starCornerY = starCorner->GetPosition().y;
    int starCornerWidth = starCorner->GetSize().x;
    int starCornerHeight = starCorner->GetSize().y;
    starCorner->SetX(width - (starCornerWidth * 0.76));
    starCorner->SetY(-starCornerHeight/2);

    // Set the size and position of the star icon
    starIcon->Resize(icon_width, icon_width);
    starIcon->SetX(width - (starCornerWidth * 0.27));
    starIcon->SetY(top_align);

    // Add the text
    sheepText = new sf::String(get_ratio_string(sheepCount, sheepTotal));
    sheepText->SetFont(*textFont);
    sheepText->SetColor(Color::Black);
    sheepText->SetX(sheepCornerX + sheepCornerWidth/10);
    sheepText->SetY(top_align + 15);

    starText = new sf::String(get_ratio_string(starCount, starTotal));
    starText->SetFont(*textFont);
    starText->SetColor(Color::Black);
    starText->SetX(width - (starCornerWidth * 0.55));
    starText->SetY(top_align + 15);
}

void HUD::addSheep(int amount)
{
    // Increment the sheep count
    sheepCount += abs(amount);

    // Ensure that sheepCount does not exceed the maximum number of sheep
    clamp(sheepCount, 0, sheepTotal);

    // Update the visible text
    sheepText->SetText(get_ratio_string(sheepCount, sheepTotal));
}

void HUD::addStars(int amount)
{
    // Increment the star count
    starCount += abs(amount);

    // Ensure that starCount does not exceed the maximum number of stars
    clamp(starCount, 0, starTotal);

    // Update the visible text
    starText->SetText(get_ratio_string(starCount, starTotal));
}

void HUD::removeSheep(int amount)
{
    // Increment the sheep count
    sheepCount -= abs(amount);

    // Ensure that sheepCount does not fall below the minimum number of sheep
    clamp(sheepCount, 0, sheepTotal);

    // Update the visible text
    sheepText->SetText(get_ratio_string(sheepCount, sheepTotal));
}

void HUD::removeStars(int amount)
{
    // Increment the star count
    starCount -= abs(amount);

    // Ensure that starCount does not fall below the minimum number of stars
    clamp(starCount, 0, starTotal);

    // Update the visible text
    starText->SetText(get_ratio_string(starCount, starTotal));
}

int HUD::getSheepTotal()
{
    return sheepTotal;
}

int HUD::getSheepCount()
{
    return sheepCount;
}

int HUD::getStarTotal()
{
    return starTotal;
}

int HUD::getStarCount()
{
    return starCount;
}

void HUD::update(Clock *clock, RenderWindow *window, World *world)
{
    // Nothing needs to be updated
}

void HUD::draw(RenderWindow *window)
{
    window->Draw(*sheepCorner);
    window->Draw(*sheepIcon);

    window->Draw(*starCorner);
    window->Draw(*starIcon);

    window->Draw(*sheepText);
    window->Draw(*starText);
}

const Vector2f& HUD::getPosition()
{
    return *position;
}

const Vector2f& HUD::getSize()
{
    return *size;
}

string HUD::get_ratio_string(int count, int total)
{
    stringstream result;
    result << count << " / " << total;

    return result.str();
}
