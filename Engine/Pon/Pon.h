#ifndef PON_H
#define PON_H
#include <SFML/Graphics.hpp>

class Pon
{
public:
    int pon_id;
    int pon_class;
    int pon_squad_position;

    int pon_exp;
    int pon_level;

    int slot_1_invItem_id = -1;
    int slot_2_invItem_id = -1;
    int slot_3_invItem_id = -1;
    int slot_4_invItem_id = -1;
    int slot_5_invItem_id = -1;
    std::vector<int> slots = {slot_1_invItem_id, slot_2_invItem_id, slot_3_invItem_id, slot_4_invItem_id, slot_5_invItem_id};

    int pon_base_hp = 100;
    int pon_hp = 100;
    int pon_base_min_dmg = 0;
    int pon_min_dmg = 0;
    int pon_base_max_dmg = 0;
    int pon_max_dmg = 0;
    int pon_base_crit;
    int pon_crit = 0;
    float pon_base_attack_speed = 2.00;
    float pon_attack_speed = 2.00;


    void recalculateStats();
    void giveItem(int inv_item_id, int where = 0);
    void removeItem(int where = 0);
    int getSlotCount();                             // add an int pon_class argument to make static
    bool canEquip(std::vector<int>, int where = 0); // add an int pon_class argument to make static
    Pon();
};

#endif // PON_H
