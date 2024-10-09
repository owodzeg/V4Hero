#include "Entity.h"

#include <spdlog/spdlog.h>
#include <libzippp.h>
#include <filesystem>
#include <fstream>

using namespace libzippp;

using json = nlohmann::json;
namespace fs = std::filesystem;

Entity::Entity()
{

}

void Entity::LoadEntity(const std::string& path)
{
    SPDLOG_INFO("Loading PNGAnimation model from {}", path);

    auto f = fs::path(path);
    std::string model_name = f.stem().string();

    bool zip = false;

    if (f.extension() == ".zip")
    {
        SPDLOG_INFO(".zip was provided, we need to extract the file");
        zip = true;
    }

    json entity;

    if(zip)
    {
        ZipArchive zf(f.string());
        zf.open(ZipArchive::ReadOnly);
        ZipEntry entry = zf.getEntry("entity.json");
        if(!entry.isNull())
        {
            entity = json::parse(entry.readAsText());

            SPDLOG_INFO("Reading entity.json.");
        }
    }
    else
    {
        std::ifstream ent(model_name+"/entity.json");
        if(ent.good())
        {
            entity << ent;
        }
        else
        {
            SPDLOG_ERROR("No entity.json file found. Entity will use default behaviors and statistics.");
        }
    }

    if(entity.contains("behavior"))
    {
        if(entity["behavior"].contains("hit"))
            bh_hit = behavior.convStringToHitEnum(entity["behavior"]["hit"]);
        if(entity["behavior"].contains("death"))
            bh_death = behavior.convStringToDeathEnum(entity["behavior"]["death"]);
    }

    AnimatedObject::LoadConfig(path);
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