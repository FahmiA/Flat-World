#include "Dog.hpp"

#include "../game/ID.hpp"
#include "../util/CoordinateUtil.hpp"
#include "Player.hpp"

#include <iostream>
using namespace std;

#define DOG_MEMORY_S 3

Dog::Dog(float x, float y, float width, float height, float speed, Sprite *sprite)
    : Character(x, y, width, height, speed, sprite)
{
    setID(ID_DOG);

    state = new WonderState;
    timeSincePlayerSeen = -1;
}

Dog::~Dog()
{
    delete state;
}

void Dog::subUpdate(float velocityX, float velocityY, Clock *clock, RenderWindow *window, World *world)
{
    // Execute AI
    seekPlayer(clock->GetElapsedTime(), *(world->getPlayer()));

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
    CoordinateUtil coordUtil;
    bool canSeePlayer = coordUtil.isInFOV(currentPosition, currentAngle, playerPosition,
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
