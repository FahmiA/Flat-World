#include "ActorStates.hpp"

ActorState::~ActorState() {}

void WonderState::performAction(Character *character, Clock *clock)
{
    timeSinceLastUpdate += clock->GetElapsedTime();

    // Update the action when the delay has elapsed
    if(timeSinceLastUpdate > UPDATE_DELAY)
    {
        timeSinceLastUpdate = 0;

        Randomizer random;
        action = (Action)random.Random(0, 2);
    }

    // Perform the active action
    Sprite *sprite = character->getSprite();
    if(action == left)
    {
        sprite->FlipX(false);
        character->moveLeft();
    }else if(action == right){
        sprite->FlipX(true);
        character->moveRight();
    }
}

void ChaseState::performAction(Character *character, Clock *clock)
{
    //character->getSprite()->FlipX(false);
    character->moveLeft();
}
