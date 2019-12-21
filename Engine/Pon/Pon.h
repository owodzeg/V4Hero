#ifndef PON_H
#define PON_H
#include <SFML/Graphics.hpp>
class SaveReader;
class Pon
{
    public:

    int pon_id;
    int pon_class;
    int pon_squad_position;

    int pon_exp;
    int pon_level;

    int weapon_invItem_id=-1;
    int weapon2_invItem_id=-1;
    int mask_invItem_id=-1;
    int armour_invItem_id=-1;
    SaveReader* savReader;


    int pon_hp=100;
    int pon_min_dmg=0;
    int pon_max_dmg=0;
    int pon_crit=0;
    float pon_attack_speed=2.00;




    void GiveItem(int InvItemId,int where=0);
    void RemoveItem(int InvItemId,int where=0);
    Pon(SaveReader* core);

};

#endif // PON_H
