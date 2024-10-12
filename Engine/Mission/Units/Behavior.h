#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <string>
#include <unordered_map>

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
        BH_HIT_STAGGER = 2,
        BH_HIT_STATIONARY_TWO_STATE = 3
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
        BH_SPAWN_DO_NOTHING = 0,
        BH_SPAWN_IDLE = 1
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

    enum class Loot
    {
        INVALID = -1,
        BH_LOOT_DO_NOTHING = 0,
        BH_LOOT_DROP_INSTANT = 1,
        BH_LOOT_DROP_FADEAWAY = 2
    };

    const std::unordered_map<std::string, Hit> stringToHitEnum = {
        {"BH_HIT_DO_NOTHING", Hit::BH_HIT_DO_NOTHING},
        {"BH_HIT_DEFAULT", Hit::BH_HIT_DEFAULT},
        {"BH_HIT_STAGGER", Hit::BH_HIT_STAGGER},
        {"BH_HIT_STATIONARY_TWO_STATE", Hit::BH_HIT_STATIONARY_TWO_STATE}
    };

    const std::unordered_map<std::string, Death> stringToDeathEnum = {
        {"BH_DEATH_DO_NOTHING", Death::BH_DEATH_DO_NOTHING},
        {"BH_DEATH_REMOVE_INSTANT", Death::BH_DEATH_REMOVE_INSTANT}
    };

    const std::unordered_map<std::string, Spawn> stringToSpawnEnum = {
        {"BH_SPAWN_DO_NOTHING", Spawn::BH_SPAWN_DO_NOTHING},
        {"BH_SPAWN_IDLE", Spawn::BH_SPAWN_IDLE}
    };

    const std::unordered_map<std::string, Loot> stringToLootEnum = {
        {"BH_LOOT_DO_NOTHING", Loot::BH_LOOT_DO_NOTHING},
        {"BH_LOOT_DROP_INSTANT", Loot::BH_LOOT_DROP_INSTANT},
        {"BH_LOOT_DROP_FADEAWAY", Loot::BH_LOOT_DROP_FADEAWAY}
    };

    Hit convStringToHitEnum(const std::string& enumName) {
        auto it = stringToHitEnum.find(enumName);
        if (it != stringToHitEnum.end())
            return it->second;
        return Hit::INVALID;
    }

    Death convStringToDeathEnum(const std::string& enumName) {
        auto it = stringToDeathEnum.find(enumName);
        if (it != stringToDeathEnum.end())
            return it->second;
        return Death::INVALID;
    }

    Spawn convStringToSpawnEnum(const std::string& enumName) {
        auto it = stringToSpawnEnum.find(enumName);
        if (it != stringToSpawnEnum.end())
            return it->second;
        return Spawn::INVALID;
    }

    Loot convStringToLootEnum(const std::string& enumName) {
        auto it = stringToLootEnum.find(enumName);
        if (it != stringToLootEnum.end())
            return it->second;
        return Loot::INVALID;
    }

};

#endif //BEHAVIOR_H
