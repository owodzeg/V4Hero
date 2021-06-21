#include "Spear.h"
#include "../../../Graphics/PSprite.h"
#include "../../../Item/Equipment.h"
#include "../../../Item/Item.h"
#include "../Projectile.h"


using namespace std;

Spear* Spear::FromItem(Item* item, PSprite sprite)
{
    Spear* spear = new Spear(sprite);
    spear->max_dmg = item->equip->max_dmg;
    spear->min_dmg = item->equip->min_dmg;
    spear->crit = item->equip->crit;
    spear->attack_speed = item->equip->attack_speed;

    return spear;
}
Spear::Spear(PSprite& tsprite)
    : Projectile(tsprite)
{
}
