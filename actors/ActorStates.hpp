#ifndef ACTORSTATES_H
#define ACTORSTATES_H

#include "Character.hpp"

/** Defines a behavioural state an actor (NPC) can be in (see the "State" design pattern).*/
class ActorState
{
    public:
        virtual ~ActorState();

        virtual void performAction(Character *character, Clock *clock) = 0;
};

/** A 'wondering' state where the character moves about randomly with no distinct objective.*/
#define UPDATE_DELAY 0.5f
class WonderState: public ActorState
{
    public:
        void performAction(Character *character, Clock *clock);

    private:
        float timeSinceLastUpdate;

        enum Action {Idle, Left, Right};
        Action action;
};

/** An aggressie 'chase' state where the character attempts to intersect the target.*/
class ChaseState: public ActorState
{
    public:
        /** Creates a new ChaseState with the target to chase.
         * @param target The target to chase.
         */
        ChaseState(Character *target);

        void performAction(Character *character, Clock *clock);

    private:
        Character *target;
};

#endif // ACTORSTATES_H

