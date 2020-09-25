#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../../Config.h"
#include "../../Input/InputController.h"
#include "../../Dialog/MessageCloud.h"
#include "CollidableObject.h"

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

    struct Loot
    {
        int item_id = 0;
        int item_chance = 0;
    };

    std::vector<Loot> loot_table;

    bool dropped_item = false;

    float curHP = 500;
    float maxHP = 500;

    float mindmg = 1;
    float maxdmg = 1;

    bool custom_dmg = false;

    int layer = 0; ///rendering priority
    int parent = -1; ///if entity is bound to any other entity

    float distance_to_unit = 0;

    float spawn_x = 0; ///base spawn X to calculate withdraw
    float stat_multiplier = 1;

    std::vector<std::string> additional_data; ///additional data passed from mission file that can be used for exclusive cases

    std::vector<MessageCloud> messageclouds;

    sf::Sound cur_sound;

    Entity();
    virtual void setEntityID(int new_entityID);
    virtual int getEntityID();
    virtual void doRhythm(std::string current_song, std::string current_drum, int combo, int realcombo, bool advanced_prefever, float beatBounce, float satisfaction);
    virtual bool doAttack();
    virtual void doMessages(sf::RenderWindow& window, float fps, InputController& inputCtrl); ///manage message clouds
    virtual void die();
    virtual void LoadConfig(Config *thisConfigs, std::string unitParamPath);
    virtual void dropItem();
};

#endif // ENTITY_H
