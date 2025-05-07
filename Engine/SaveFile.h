#ifndef SAVEFILE_H
#define SAVEFILE_H
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct SquadSlot
{
    int rarepon = 0;
    int _class = 0;
    float level = 0;
    float exp = 0;
    std::vector<std::vector<int>> slots;

    bool operator==(const SquadSlot&) const = default;

    friend void to_json(nlohmann::json& json_output, const SquadSlot& squad_slot);

    friend void from_json(const nlohmann::json& json_input, SquadSlot& squad_slot);
};

struct Army
{
    SquadSlot hero;
    std::vector<std::vector<SquadSlot>> squads;

    bool operator==(const Army&) const = default;

    friend void to_json(nlohmann::json& json_output, const Army& army);

    friend void from_json(const nlohmann::json& json, Army& army);
};

struct SaveFileDetails
{
    std::string version = "2.0";
    std::string name = "";
    std::vector<int> locations_unlocked;
    int story_point = 0;

    bool operator==(const SaveFileDetails&) const = default;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
        SaveFileDetails,
        version,
        name,
        locations_unlocked,
        story_point
    );
};

struct SavedItem
{
    std::string name = "";
    int count = 0;

    bool operator==(const SavedItem&) const = default;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        SavedItem,
        name,
        count
    );
};

struct SavedMission
{
    int id = 0;
    int level = 0;
    bool unlocked = false;

    bool operator==(const SavedMission&) const = default;

    friend void to_json(nlohmann::json& json_output, const SavedMission& saved_mission);

    friend void from_json(const nlohmann::json& json_input, SavedMission& saved_mission);
};

struct SaveData
{
    Army army;
    SaveFileDetails details;
    std::vector<SavedItem> items;
    std::vector<SavedMission> missions;

    bool operator==(const SaveData&) const = default;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        SaveData,
        army,
        details,
        items,
        missions
    );
};

struct SaveFile {
    SaveData save;

    bool operator==(const SaveFile&) const = default;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        SaveFile,
        save
    );
};

#endif //SAVEFILE_H
