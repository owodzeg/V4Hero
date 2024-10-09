#ifndef ENTITY_H
#define ENTITY_H

#include "../../Dialog/MessageCloud.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "AnimatedObject.h"
#include "Behavior.h"

class Entity : public AnimatedObject
{
public:
    int entityID = -1;
    int orderID = -1;

    enum EntityTypes
    {
        DUMMY = -1,
        PASSIVE = 0,
        HOSTILE = 1,
        NEUTRAL = 2
    };

    enum EntityCategories
    {
        NO_CATEGORY = -1,
        ANIMAL = 0,
        BUILDING_REGULAR = 1,
        ENEMYUNIT = 2,
        MISC = 3,
        NATURE = 4,
        OBSTACLE_GROUND = 5,
        OBSTACLE_ROCK = 6,
        OBSTACLE_IRON = 7,
        OBSTACLE_WOOD = 8,
        BUILDING_IRON = 9
    };

    // package of behavior-specific contents
    Behavior behavior;

    Behavior::Hit bh_hit;
    Behavior::Death bh_death;
    Behavior::Noise bh_noise;
    Behavior::Approach bh_approach;
    Behavior::Command bh_command;
    Behavior::Decision bh_decision;
    Behavior::Spawn bh_spawn;
    Behavior::Attack bh_attack;
    Behavior::Idle bh_idle;
    Behavior::Flee bh_flee;

    int entityType = DUMMY;
    int entityCategory = NO_CATEGORY;

    struct Loot {
        std::vector<int> order_id;
    };

    std::vector<Loot> loot_table;

    float curHP = 100;
    float maxHP = 100;

    bool dead = false;
    bool forRemoval = false;

    float mindmg = 1;
    float maxdmg = 1;

    int layer = 0;   ///rendering priority
    int parent = -1; ///if entity is bound to any other entity

    nlohmann::json additional_data; ///additional data passed from mission file that can be used for exclusive cases

    std::vector<MessageCloud> messageclouds;

    Entity();
    void LoadEntity(const std::string& path);

    void setEntityID(int new_entityID);
    int getEntityID();

    sf::FloatRect getHitbox();

    // handlers - environment
    void handleHit(float damage); // when inflicted damage from player
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

    // communication
    void handleRhythmMessages();

    void Draw();
};

#endif // ENTITY_H
