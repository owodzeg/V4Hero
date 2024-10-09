#ifndef BEHAVIOR_H
#define BEHAVIOR_H

// Database for entity behaviors and behavior-specific objects

/*
// handlers - environment
void handleHit(); // when inflicted damage from player
void handleDeath(); // when hp goes under 0
void handleNoise(); // when fever is activated
void handleApproach(); // when player gets close
void handleCommand(); // on player command inputted

// handlers - self
void handleDecisions(); // what should the entity do?
void handleSpawn(); // on entity creation
void handleAttack(); // entity's attack
void handleIdle(); // when entity is idling
void handleFlee(); // entity's flee
*/

class Behavior {
public:

    Behavior();

    // Environment-dependent behaviors
    enum class Hit
    {
        INVALID = -1,
        BH_HIT_DO_NOTHING = 0,
        BH_HIT_DEFAULT = 1,
        BH_HIT_STAGGER = 2
    };

    enum class Death
    {
        INVALID = -1,
        BH_DEATH_DO_NOTHING = 0,
        BH_DEATH_REMOVE_INSTANT = 1
    };

    enum class Noise
    {
        INVALID = -1,
        BH_NOISE_DO_NOTHING = 0
    };

    enum class Approach
    {
        INVALID = -1,
        BH_APPROACH_DO_NOTHING = 0
    };

    enum class Command
    {
        INVALID = -1,
        BH_COMMAND_DO_NOTHING = 0
    };

    // Self-controlled behaviors
    enum class Decision
    {
        INVALID = -1,
        BH_DECISION_DO_NOTHING = 0
    };

    enum class Spawn
    {
        INVALID = -1,
        BH_SPAWN_DO_NOTHING = 0
    };

    enum class Attack
    {
        INVALID = -1,
        BH_ATTACK_DO_NOTHING = 0
    };

    enum class Idle
    {
        INVALID = -1,
        BH_IDLE_DO_NOTHING = 0
    };

    enum class Flee
    {
        INVALID = -1,
        BH_FLEE_DO_NOTHING = 0
    };
};

#endif //BEHAVIOR_H
