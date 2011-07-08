#ifndef SHEEP_H
#define SHEEP_H

#include "../game/World.hpp"
#include "../environment/Island.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

class Sheep: public GameObject
{
    public:
        Sheep();
        virtual ~Sheep();
    protected:
    private:
};

#endif // SHEEP_H
