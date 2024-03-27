#ifndef ENTITY_H
#define ENTITY_H

#include "../../Dialog/MessageCloud.h"
#include "../../Input/InputController.h"
#include "CollidableObject.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

class Entity : public CollidableObject
{
public:
    int entityID = -1;
    int spawnOrderID = -1;
    bool dead = false;

    float floorY = 0;

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

    int entityType = DUMMY;
    int entityCategory = NO_CATEGORY;

    struct Loot {
        std::vector<int> order_id;
    };

    std::vector<Loot> loot_table;

    bool dropped_item = false;

    float curHP = 500;
    float maxHP = 500;

    float mindmg = 1;
    float maxdmg = 1;

    bool custom_dmg = false;

    int layer = 0;   ///rendering priority
    int parent = -1; ///if entity is bound to any other entity

    float distance_to_unit = 0;

    float spawn_x = 0; ///base spawn X to calculate withdraw
    float stat_multiplier = 1;

    nlohmann::json additional_data; ///additional data passed from mission file that can be used for exclusive cases

    std::vector<MessageCloud> messageclouds;

    bool cloneable = false;
    int respawnTime = 0;
    sf::Clock respawn_clock;

    bool force_spawn = false;
    int force_spawn_lvl = 0;

    bool force_drop = false;
    string force_drop_item;
    int force_drop_mission_lvl = 0;

    float view_range = 750;

    sf::Sound cur_sound;

    Entity();
    virtual void setEntityID(int new_entityID);
    virtual int getEntityID();
    virtual bool willDrop(vector<int> item_id);
    // TO-DO: THIS IS BAD! Make a way for the entities to reach to the global sound objects instead of passing so many arguments when they are not needed!!!
    virtual void doRhythm(std::string current_song, std::string current_drum, int combo, int realcombo, bool advanced_prefever, float beatBounce, float satisfaction);
    virtual bool doAttack();
    virtual void doMessages(sf::RenderWindow& window, float fps, InputController& inputCtrl); ///manage message clouds
    virtual void die();
    virtual void LoadConfig(std::string unitParamPath);
    virtual void parseAdditionalData(nlohmann::json additional_data);
    virtual void dropItem();
};

#endif // ENTITY_H
