#include "ActorStates.hpp"

#include "Character.hpp"
#include "../util/AnimatedSprite.hpp"

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
        // Update the animation
        ((AnimatedSprite*)character->getSprite())->resume();
    }else if(action == right){
        sprite->FlipX(true);
        character->moveRight();
        // Update the animation
        ((AnimatedSprite*)character->getSprite())->resume();
    }else{
        // Update the animation
        //TODO: Causes crash
        //((AnimatedSprite*)character->getSprite())->play(ANIMATE_WALK);
        //((AnimatedSprite*)character->getSprite())->pause();
    }
}

void ChaseState::performAction(Character *character, Clock *clock)
{
    //character->getSprite()->FlipX(false);
    character->moveLeft();
}
