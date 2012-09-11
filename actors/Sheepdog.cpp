#include "Sheepdog.hpp"

#include "game/ID.hpp"
#include "Player.hpp"
#include "GUI/HUD.hpp"
#include "util/CoordinateUtil.hpp"
#include "util/AnimatedSprite.hpp"
#include "gameIO/LevelBuilder.hpp"
#include "util/Debug.hpp"

#include <iostream>
#include <math.h>
using namespace std;

#define DOG_MEMORY_S 3

Sheepdog::Sheepdog(float x, float y, float width, float height,
         float speed, AnimatedSprite *sprite, LevelBuilder *levelBuilder)
    : AnimatedCharacter(x, y, width, height, speed, sprite)
{
    setID(ID_DOG);

    this->levelBuilder = levelBuilder;

    state = new WonderState;
    timeSincePlayerSeen = -1;
}

Sheepdog::~Sheepdog()
{
    delete state;
}

void Sheepdog::subUpdate(Clock *clock, RenderWindow *window, World *world)
{
    AnimatedCharacter::subUpdate(clock, window, world);
    Player &player = *(world->getPlayer());
    // Execute AI
    seekPlayer(clock->getElapsedTime().asSeconds(), player);

    // Perform collision
    checkPlayerCollide(*world);

    // Update the state
    state->performAction(this, clock);
}

void Sheepdog::draw(RenderWindow *window)
{
    Character::draw(window);
}

void Sheepdog::seekPlayer(float elapsedTime, Player &player)
{
    // Change to a chassing state when the player is in the field of view (FOV)
    float fovAngleR = M_PI / 2.0;
    float fovDistance = 200.0f;
    Transform myGlobalTransform = getSprite()->getTransform();
    const Vector2f currentPosition = myGlobalTransform.transformPoint(getSprite()->getOrigin());
    float currentAngle = getRotation();

    // Reverse the angle if looking to the left
    Transform targetGlobalTransform = player.getSprite()->getTransform();
    const Vector2f playerPosition = targetGlobalTransform.transformPoint(player.getSprite()->getOrigin());

    // Check if the player is in the FOV
    bool canSeePlayer = getCoordinateUtil().isInFOV(*this, player, getFacingDirection(), fovAngleR, fovDistance);

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

void Sheepdog::checkPlayerCollide(World &world)
{
    // For perfromance, only check if we are in the atack state.
    if(timeSincePlayerSeen >= 0)
    {
        CoordinateUtil coordUtil;
        Player &player = *(world.getPlayer());
        if(coordUtil.collide(getSprite(), player.getSprite()))
        {
            // Only push the Player back if the player is not already being pushed back
            if(player.getPushDirection() == None)
            {
                 // Indicate that an attack was made
                player.pushBack(getFacingDirection());

                // Penalise the player by one sheep
                if(world.getSheepCaptured() > 0)
                {
                    levelBuilder->addRandomSheep();
                    world.getHud()->removeSheep(1);
                }
            }
        }
    }
}
