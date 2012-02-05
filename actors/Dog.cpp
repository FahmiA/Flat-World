#include "Dog.hpp"

#include "game/ID.hpp"
#include "Player.hpp"
#include "util/CoordinateUtil.hpp"
#include "gameIO/LevelBuilder.hpp"

#include <iostream>
using namespace std;

#define DOG_MEMORY_S 3

Dog::Dog(float x, float y, float width, float height,
         float speed, Sprite *sprite, LevelBuilder *levelBuilder)
    : Character(x, y, width, height, speed, sprite)
{
    setID(ID_DOG);

    this->levelBuilder = levelBuilder;

    state = new WonderState;
    timeSincePlayerSeen = -1;
}

Dog::~Dog()
{
    delete state;
}

void Dog::subUpdate(Clock *clock, RenderWindow *window, World *world)
{
    Player &player = *(world->getPlayer());
    // Execute AI
    seekPlayer(clock->GetElapsedTime(), player);

    // Perform collision
    checkPlayerCollide(*world);

    // Dog attack behaviour
    /*if(coordUtil.collide(getSprite(), world->getPlayer()->getSprite()))
    {
        // Push player back

        // Remove one Sheep from player

        // Spawn one sheep into world
    }*/

    // Update the state
    state->performAction(this, clock);

    // Update the sprite animation
    ((AnimatedSprite*)getSprite())->update(clock);
}

void Dog::seekPlayer(float elapsedTime, Player &player)
{
    // Change to a chassing state when the player is in the field of view (FOV)
    float fovAngle = 0.5;
    int fovDepth = 200;
    const Vector2f currentPosition = getSprite()->TransformToGlobal(getSprite()->GetCenter());
    float currentAngle = getSprite()->GetRotation() * M_PI / 180;

    // Reverse the angle if looking to the left
    if(getFacingDirection() == Left)
        currentAngle = currentAngle - M_PI;
    const Vector2f playerPosition = player.getSprite()->TransformToGlobal(player.getSprite()->GetCenter());

    // Check if the player is in the FOV
    bool canSeePlayer = getCoordinateUtil().isInFOV(currentPosition, currentAngle, playerPosition,
                                          fovDepth, fovAngle);

    // If the dog has been seen recently (last seen time is not invalid), update the last seen time.
    if(timeSincePlayerSeen >= 0)
        timeSincePlayerSeen += elapsedTime;

    if(canSeePlayer)
    {
        // Player is sighted! Chase the player
        if(timeSincePlayerSeen < 0)
        {
            cout << "DOG ANGREY!" << endl;
            // TODO: Crash on DOG ANGREY!!
            delete state;
            state = new ChaseState(&player);

            timeSincePlayerSeen = 0;
        }
    }else if(timeSincePlayerSeen > DOG_MEMORY_S){
        // Player has been lost. Resume normal behaviour.
        cout << "DOG SAD" << endl;

        delete state;
        state = new WonderState();

        timeSincePlayerSeen = -1;
    }
}

void Dog::checkPlayerCollide(World &world)
{
    // For perfromance, only check if we are in the atack state.
    if(timeSincePlayerSeen >= 0)
    {
        CoordinateUtil coordUtil;
        Player &player = *(world.getPlayer());
        if(coordUtil.collide(getSprite(), player.getSprite()))
        {
            // Indicate that an attack was made
            player.pushBack(getFacingDirection());

            // Penalise the player by one sheep
            levelBuilder->addRandomSheep();
        }
    }
}
