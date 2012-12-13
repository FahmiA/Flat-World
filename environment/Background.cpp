#include "Background.hpp"

#include "util/NumberUtil.hpp"
#include "util/CoordinateUtil.hpp"
#include "util/Debug.hpp"
#include "game/ID.hpp"
#include "game/World.hpp"
#include "actors/Player.hpp"


#include<iostream>
using namespace std;

int Background::MAX_STAR_COUNT = 20;
int Background::MAX_CLOUD_COUNT = 20;
float Background::PARALAX_LAG = 0.2f;

Background::Background(TSprite star, TSprite cloud)
{
    setID(ID_BACKGROUND);
    prototypeStar = star;
    prototypeCloud = cloud;

    backDrop.setSize(Vector2f(2000.0f, 2000.0f));
    backDrop.setOrigin(1000.0f, 1000.0f);
    backDropColour = Color::Blue;

    setStage(stage);
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

    int starCount = calcStarCount();
    int cloudCount = calcCloudCount();
    populate(stars, prototypeStar, starCount);
    populate(clouds, prototypeCloud, cloudCount);
}

int Background::getStarCount()
{
    return stars.size();
}

int Background::getCloudCount()
{
    return clouds.size();
}

const Vector2f& Background::getPosition()
{
    return Vector2f();
}

const Vector2f& Background::getSize()
{
    return Vector2f();
}

void Background::update(Clock *clock, RenderWindow *window, World *world)
{
    // Update the backdrop position
    backDrop.setPosition(world->getPlayer()->getPosition());
    backDropColour.b = pow(stage, 3) * 255;
    backDrop.setFillColor(backDropColour);

    // Calculate the amount of backdrop/player movement since the last frame
    Vector2f backDropDelta(backDrop.getPosition().x - backDropPrevPos.x,
                           backDrop.getPosition().y - backDropPrevPos.y);

    // Update the star positions
    itemUpdate(stars, backDropDelta);

    // Update the cloud positions
    itemUpdate(clouds, backDropDelta);

    // Remember the backdrop position for next frame
    backDropPrevPos = backDrop.getPosition();
}

void Background::itemUpdate(vector<TSprite> &items, Vector2f &moveDelta)
{
    static CoordinateUtil coordUtil;
    for(int i = 0; i < items.size(); i++)
    {
        //moveDelta.y = -30;
        TSprite &item = items.at(i);
        item.move(moveDelta.x * PARALAX_LAG,
                  moveDelta.y * PARALAX_LAG);

        // Re-position if outlying
        if(!coordUtil.isGlobalPointInside(item.getPosition(), backDrop))
        {
            Vector2f itemPos = item.getPosition();
            Vector2f newPos(itemPos);

            if(itemPos.x <= backDrop.getPosition().x - (backDrop.getSize().x / 2))
            {
                newPos = getRelativeCoords(backDrop.getSize().x,
                                           rand() % (int)backDrop.getSize().y);
            }else if(itemPos.x > backDrop.getPosition().x + (backDrop.getSize().x / 2))
            {
                newPos = getRelativeCoords(0,
                                           rand() % (int)backDrop.getSize().y);
            }else if(itemPos.y <= backDrop.getPosition().y - (backDrop.getSize().y / 2))
            {
                newPos = getRelativeCoords(rand() % (int)backDrop.getSize().x,
                                           backDrop.getSize().y);
            }else //if(itemPos.y > backDrop.getPosition().y + (backDrop.getSize().y / 2))
            {
                newPos = getRelativeCoords(rand() % (int)backDrop.getSize().x,
                                           0);
            }

            item.setPosition(newPos.x, newPos.y);
        }
    }
}

void Background::draw(RenderWindow *window)
{
    window->draw(backDrop);

    for(int i = 0; i < stars.size(); i++)
        stars.at(i).draw(window);

    for(int i = 0; i < clouds.size(); i++)
        clouds.at(i).draw(window);
}

int Background::calcStarCount()
{
    return (int)(getLiniar(1.0f - stage) * MAX_STAR_COUNT);
}

int Background::calcCloudCount()
{
    return (int)(getLiniar(stage) * MAX_CLOUD_COUNT);
}

float Background::getLiniar(float value)
{
    clamp<float>(value, 0.0f, 1.0f);
    return value;
}

Vector2f Background::getRelativeCoords(float x, float y)
{
    Vector2f backDropSize = backDrop.getSize();
    Vector2f backDropPos = backDrop.getPosition();

    Vector2f newCoords;
    newCoords.x = x + backDropPos.x - (backDropSize.x / 2.0f);
    newCoords.y= y + backDropPos.y - (backDropSize.y / 2.0f);

    return newCoords;
}

void Background::populate(vector<TSprite> &items, TSprite &prototypeItem,
                          int desiredCOunt)
{
    int itemCount = items.size();
    if(itemCount < desiredCOunt)
    {
        // Add more items
        Vector2f backDropSize = backDrop.getSize();
        Vector2f backDropPos = backDrop.getPosition();
        for(int i = 0; i < desiredCOunt - itemCount; i++)
        {
            TSprite newItem(prototypeItem);
            Vector2f newPos = getRelativeCoords(rand() % (int)backDropSize.x,
                                                rand() % (int)backDropSize.y);

            newItem.setPosition(newPos.x, newPos.y);
            items.push_back(newItem);
        }
    }else if(itemCount > desiredCOunt) {
        // Remove items
        for(int i = 0; i < itemCount - desiredCOunt; i++)
        {
            int indexToRemove = (int)(rand() * (itemCount - i));
            items.erase(items.begin() + indexToRemove);
        }
    }
}
