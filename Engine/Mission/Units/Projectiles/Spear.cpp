#include "Spear.h"
#include "../Projectile.h"
#include "../../../Item/Item.h"
#include "../../../Item/Weapon.h"
#include "../../../Graphics/PSprite.h"


using namespace std;

Spear* Spear::FromItem(Item* item,PSprite sprite)
{
    Spear* spear = new Spear(sprite);
    Weapon* owep = static_cast<Weapon*>(item);
    spear->maxdmg=owep->maxdmg;
    spear->mindmg=owep->mindmg;
    spear->crit=owep->crit;

    return spear;
}
Spear::Spear(PSprite& tsprite) : Projectile(tsprite)
{

}
