#include "Spear.h"
#include "Projectile.h"
#include "../../Item/Item.h"
#include "../../Item/Weapon.h"


using namespace std;

Spear* Spear::FromItem(Item* item){
    Spear* spear = new Spear();
    Weapon* owep = static_cast<Weapon*>(item);
    spear->maxdmg=owep->maxdmg;
    spear->mindmg=owep->mindmg;
    spear->crit=owep->crit;
    return spear;
}
