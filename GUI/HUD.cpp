#include "HUD.hpp"

#include "util/NumberUtil.hpp"
#include "util/SpriteUtil.hpp"

#include <algorithm>
#include <sstream>
using namespace std;

HUD::HUD(AnimatedSprite *sheepCorner, AnimatedSprite *starCorner,
            AnimatedSprite *sheepIcon, AnimatedSprite *starIcon,
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
    sheepCorner->setSize(width / 3.0f, height / 3.0f);
    int sheepCornerX = sheepCorner->getPosition().x;
    int sheepCornerY = sheepCorner->getPosition().y;
    int sheepCornerWidth = sheepCorner->getSize().x;
    int sheepCornerHeight = sheepCorner->getSize().y;
    sheepCorner->setPosition(-sheepCornerWidth / 4, -sheepCornerHeight / 4);

    // Set the size and position of the sheep icon
    float top_align = sheepCornerY + sheepCornerHeight / 4.5;
    sheepIcon->setSize(icon_width, icon_width);
    sheepIcon->setPosition(sheepCornerX + sheepCornerWidth / 2.5, top_align);

    // Set the size and position of the star corner
    starCorner->setSize(width / 3, width / 3);
    int starCornerWidth = starCorner->getSize().x;
    int starCornerHeight = starCorner->getSize().y;
    starCorner->setPosition(width - (starCornerWidth * 0.76), -starCornerHeight / 2);

    // Set the size and position of the star icon
    starIcon->setSize(icon_width, icon_height);
    starIcon->setPosition(width - (starCornerWidth * 0.27), top_align);

    // Add the text
    sheepText = new Text(get_ratio_string(sheepCount, sheepTotal));
    sheepText->setFont(*textFont);
    sheepText->setColor(Color::Black);
    sheepText->setPosition(sheepCornerX + sheepCornerWidth / 10, top_align + 15);

    starText = new Text(get_ratio_string(starCount, starTotal));
    starText->setFont(*textFont);
    starText->setColor(Color::Black);
    starText->setPosition(width - (starCornerWidth * 0.55), top_align + 15);
}

void HUD::addSheep(int amount)
{
    // Increment the sheep count
    sheepCount += abs(amount);

    // Ensure that sheepCount does not exceed the maximum number of sheep
    clamp(sheepCount, 0, sheepTotal);

    // Update the visible text
    sheepText->setString(get_ratio_string(sheepCount, sheepTotal));
}

void HUD::addStars(int amount)
{
    // Increment the star count
    starCount += abs(amount);

    // Ensure that starCount does not exceed the maximum number of stars
    clamp(starCount, 0, starTotal);

    // Update the visible text
    starText->setString(get_ratio_string(starCount, starTotal));
}

void HUD::removeSheep(int amount)
{
    // Increment the sheep count
    sheepCount -= abs(amount);

    // Ensure that sheepCount does not fall below the minimum number of sheep
    clamp(sheepCount, 0, sheepTotal);

    // Update the visible text
    sheepText->setString(get_ratio_string(sheepCount, sheepTotal));
}

void HUD::removeStars(int amount)
{
    // Increment the star count
    starCount -= abs(amount);

    // Ensure that starCount does not fall below the minimum number of stars
    clamp(starCount, 0, starTotal);

    // Update the visible text
    starText->setString(get_ratio_string(starCount, starTotal));
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
    sheepCorner->draw(window);
    sheepIcon->draw(window);

    sheepCorner->draw(window);
    sheepCorner->draw(window);

    starCorner->draw(window);
    starIcon->draw(window);

    window->draw(*sheepText);
    window->draw(*starText);
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
