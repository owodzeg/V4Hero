#ifndef PON_H
#define PON_H
#include <SFML/Graphics.hpp>

class Pon
{
public:
    int pon_id = -1;
    int pon_class = 0;
    int pon_squad_position = 0;

    float pon_exp = 0;
    float pon_level = 1;

    int slot_1_invItem_id = -1;
    int slot_2_invItem_id = -1;
    int slot_3_invItem_id = -1;
    int slot_4_invItem_id = -1;
    int slot_5_invItem_id = -1;
    std::vector<int> slots = {slot_1_invItem_id, slot_2_invItem_id, slot_3_invItem_id, slot_4_invItem_id, slot_5_invItem_id};

    float pon_base_hp = 100;
    float pon_hp = 100;
    float pon_base_min_dmg = 0;
    float pon_min_dmg = 0;
    float pon_base_max_dmg = 0;
    float pon_max_dmg = 0;
    float pon_base_crit;
    float pon_crit = 0;
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
