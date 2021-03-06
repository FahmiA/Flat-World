#include "ActorStates.hpp"

#include "Character.hpp"
#include "util/TSprite.hpp"

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
    TSprite *sprite = character->getSprite();
    if(action == Left)
    {
        character->moveLeft();
        // Update the animation
        sprite->play(TSprite::ANIMATE_RUN);
    }else if(action == Right){
        character->moveRight();
        // Update the animation
        sprite->play(TSprite::ANIMATE_RUN);
    }else{ //Idle
        // Update the animation
        sprite->play(TSprite::ANIMATE_IDLE);
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
    TSprite *sprite = character->getSprite();
    sprite->play(TSprite::ANIMATE_RUN);
}
