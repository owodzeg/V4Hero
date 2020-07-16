#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "../../Config.h"
#include "CollidableObject.h"

class Entity : public CollidableObject
{
    public:
    int entityID = -1;

    struct Loot
    {
        int item_id = 0;
        int item_chance = 0;
    };

    std::vector<Loot> loot_table;

    bool dropped_item = false;

    Entity();
    virtual void setEntityID(int new_entityID);
    virtual int getEntityID();
    virtual void doRhythm(std::string current_song, std::string current_drum, int combo, int realcombo, bool advanced_prefever, float beatBounce, float satisfaction);
    virtual void LoadConfig(Config *thisConfigs, std::string unitParamPath);
};

#endif // ENTITY_H
