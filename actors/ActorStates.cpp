#include "ActorStates.hpp"

#include "Character.hpp"
#include "util/AnimatedSprite.hpp"

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
        character->moveLeft();
        // Update the animation
        ((AnimatedSprite*)character->getSprite())->resume();
    }else if(action == right){
        character->moveRight();
        // Update the animation
        ((AnimatedSprite*)character->getSprite())->resume();
    }else{ //Idle
        // Update the animation
        ((AnimatedSprite*)character->getSprite())->play(ANIMATE_WALK);
        ((AnimatedSprite*)character->getSprite())->pause();
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
