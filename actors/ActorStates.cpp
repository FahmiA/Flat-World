#include "ActorStates.hpp"

#include "Character.hpp"
#include "util/AnimatedSprite.hpp"

ActorState::~ActorState() {}

void WonderState::performAction(Character *character, Clock *clock)
{
    timeSinceLastUpdate += clock->getElapsedTime().asSeconds();

    // Update the action when the delay has elapsed
    if(timeSinceLastUpdate > UPDATE_DELAY)
    {
        timeSinceLastUpdate = 0;

        action = (Action)(rand() % 2);
    }

    // Perform the active action
    Sprite *sprite = character->getSprite();
    if(action == Left)
    {
        character->moveLeft();
        // Update the animation
        ((AnimatedSprite*)sprite)->play("Run");
    }else if(action == Right){
        character->moveRight();
        // Update the animation
        ((AnimatedSprite*)sprite)->play("Run");
    }else{ //Idle
        // Update the animation
        ((AnimatedSprite*)sprite)->play("Idle");
    }
}

ChaseState::ChaseState(GameObject *target)
{
    this->target = target;
}

void ChaseState::performAction(Character *character, Clock *clock)
{
    if(target->getPosition().x < character->getPosition().x)
        character->moveLeft();
    else
        character->moveRight();
}
