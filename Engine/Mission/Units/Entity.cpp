#include "Entity.h"

Entity::Entity()
{
    handleSpawn();
}

void Entity::setEntityID(int new_entityID)
{
    entityID = new_entityID;
}

int Entity::getEntityID()
{
    return entityID;
}

// handlers - environment
void Entity::handleHit(float damage) // when inflicted damage from player
{
    switch(bh_hit)
    {
        case Behavior::Hit::BH_HIT_DEFAULT: {
            curHP -= damage;
            break;
        }

        case Behavior::Hit::BH_HIT_STAGGER: {
            curHP -= damage;
            setAnimation("stagger");
            restartAnimation();
            break;
        }
    }
}

void Entity::handleDeath() // when hp goes under 0
{
    switch(bh_death)
    {
        case Behavior::Death::BH_DEATH_REMOVE_INSTANT: {
            forRemoval = true;
            break;
        }
    }
}

void Entity::handleNoise() // when fever is activated
{

}

void Entity::handleApproach() // when player gets close
{

}

void Entity::handleCommand() // on player command inputted
{

}

// handlers - self
void Entity::handleDecisions() // what should the entity do?
{

}

void Entity::handleSpawn() // on entity creation
{

}

void Entity::handleAttack() // entity's attack
{

}

void Entity::handleIdle() // when entity is idling
{

}

void Entity::handleFlee() // entity's flee
{

}

void Entity::Draw()
{
    if(curHP <= 0)
    {
        handleDeath();
    }

    AnimatedObject::Draw();
}