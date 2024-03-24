#pragma once

#include <string>
#include <vector>
#include "Squad.h"

class SquadRegistry {
public:
    SquadRegistry();

    nlohmann::json squad_data;

    // Returns the number of squads in the registry
    int GetNumSquads() const;

    // Returns the squad at the given index
    Squad& GetSquad(int index);

    // Adds a new squad to the registry
    void AddSquad(const Squad& squad);

    // Removes the squad at the given index from the registry
    void RemoveSquad(int index);

    // Saves the registry to the JSON file
    void Save() const;
    // Loads the registry from the JSON file
    void Load();
    std::vector<Squad> m_squads;

private:
    std::string m_jsonFilePath;

};
