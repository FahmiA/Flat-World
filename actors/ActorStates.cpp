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

        action = (Action)(rand() % 3);
    }

    // Perform the active action
    AnimatedSprite *sprite = character->getSprite();
    if(action == Left)
    {
        character->moveLeft();
        // Update the animation
        sprite->play("Run");
    }else if(action == Right){
        character->moveRight();
        // Update the animation
        sprite->play("Run");
    }else{ //Idle
        // Update the animation
        sprite->play("Idle");
    }
}

ChaseState::ChaseState(Character *target)
{
    this->target = target;
}

void ChaseState::performAction(Character *character, Clock *clock)
{
    // Move the character
    if(character->getFacingDirection() == Left)
        character->moveLeft();
    else
        character->moveRight();

    // Animate the character
    AnimatedSprite *sprite = character->getSprite();
    sprite->play("Run");
}
