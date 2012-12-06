#include "Background.hpp"

#include "util/NumberUtil.hpp"
#include "game/ID.hpp"
#include "game/World.hpp"
#include "actors/Player.hpp"
#include "util/Debug.hpp"

#include<iostream>
using namespace std;

int Background::MAX_STAR_COUNT = 20;
int Background::MAX_CLOUD_COUNT = 20;

Background::Background(TSprite star, TSprite cloud)
{
    setID(ID_BACKGROUND);
    prototypeStar = star;
    prototypeCloud = cloud;

    backDrop.setSize(Vector2f(2000.0f, 2000.0f));
    backDrop.setOrigin(1000.0f, 1000.0f);

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
    backDrop.setPosition(world->getPlayer()->getPosition());
    backDrop.setFillColor(Color::Black);

    Vector2f backDropDelta(backDrop.getPosition().x - backDropPrevPos.x,
                           backDrop.getPosition().y - backDropPrevPos.y);

    for(int i = 0; i < stars.size(); i++)
        stars.at(i).move(backDropDelta.x, backDropDelta.y);

     for(int i = 0; i < clouds.size(); i++)
        clouds.at(i).move(backDropDelta.x, backDropDelta.y);

    backDropPrevPos = backDrop.getPosition();
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
    return (int)(getLiniar(0.5f - stage, 0.0f, 0.5f) * MAX_STAR_COUNT);
}

int Background::calcCloudCount()
{
    return (int)(getLiniar(stage - 0.5f, 0.0f, 0.5f) * MAX_CLOUD_COUNT);
}

float Background::getLiniar(float value, float min, float max)
{
    float result =  value / (max - min);
    clamp<float>(result, 0.0f, 1.0f);

    return result;
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
            int x = (int)(rand() % (int)backDropSize.x);
            int y = (int)(rand() % (int)backDropSize.y);
            x += backDropPos.x - (backDropSize.x / 2);
            y += backDropPos.y - (backDropSize.y / 2);
            newItem.setPosition(x, y); // TODO: Give random position in view
            cout << "Added item at " << PRINT_V(newItem.getPosition()) << endl;
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
