#include "SaveFile.h"

using json = nlohmann::json;

void to_json(json& json_output, const SquadSlot& squad_slot)
{
    json_output["class"] = squad_slot._class;
    json_output["exp"] = squad_slot.exp;
    json_output["level"] = squad_slot.level;
    json_output["rarepon"] = squad_slot.rarepon;
    json_output["slots"] = squad_slot.slots;
}

void from_json(const json& json_input, SquadSlot& squad_slot)
{
    squad_slot._class = json_input.value("class", 0.f);
    squad_slot.exp = json_input.value("exp", 0.f);
    squad_slot.level = json_input.value("level", 0.f);
    squad_slot.rarepon = json_input.at("rarepon");
    squad_slot.slots = json_input.value("slots", std::vector<std::vector<int>>{});
}

void to_json(json& json_output, const Army& army)
{
    json_output = json::array();
    json_output.push_back(army.hero);

    for (const auto& squad : army.squads)
    {
        json_output.push_back(squad);
    }
}

void from_json(const json& json_input, Army& army)
{
    const json& hero_input = json_input.at(0);

    json input_without_hero = json_input;
    input_without_hero.erase(0);

    army.hero = hero_input.get<SquadSlot>();
    army.squads = input_without_hero.get<std::vector<std::vector<SquadSlot>>>();
}

void to_json(json& json_output, const SavedMission& saved_mission)
{
    json_output = json::array();

    json_output.push_back(saved_mission.id);
    json_output.push_back(saved_mission.level);
    json_output.push_back(saved_mission.unlocked);
}

void from_json(const json& json_input, SavedMission& saved_mission)
{
    int mission_number = json_input.at(0);
    int level = json_input.at(1);
    bool unlocked = json_input.at(2);

    saved_mission.id = mission_number;
    saved_mission.level = level;
    saved_mission.unlocked = unlocked;
}
