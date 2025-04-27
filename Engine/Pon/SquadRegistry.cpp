
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Squad.h"
#include <spdlog/spdlog.h>
#include "SquadRegistry.h"

using json = nlohmann::json;


SquadRegistry::SquadRegistry()
{
}

int SquadRegistry::GetNumSquads() const
{
    return m_squads.size();
}

Squad& SquadRegistry::GetSquad(int index)
{
    return m_squads[index];
}

void SquadRegistry::AddSquad(const Squad& squad)
{
    m_squads.push_back(squad);
}

void SquadRegistry::RemoveSquad(int index)
{
    m_squads.erase(m_squads.begin() + index);
}

void SquadRegistry::Load()
{
    m_jsonFilePath = "resources/data/classes.json";
    std::ifstream file(m_jsonFilePath);

    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open file for reading: {}", m_jsonFilePath);
        return;
    }

    json j;
    file >> j;
    
    m_squads.clear();
    for (const auto& squadJson : j["classes"]) {
        m_squads.push_back(Squad::fromJson(squadJson));
        SPDLOG_INFO("Loaded squad: {}", m_squads.back().getName());
    }
    SPDLOG_INFO("Loaded {} squads", m_squads.size());

    // quickly check that we have no duplicate squad IDs and that they are in order
    for (int i = 0; i < m_squads.size(); i++) {
        if (m_squads[i].getId() != i) {
            SPDLOG_ERROR("Squad IDs are not in order or are not unique!");
            break;
        }
    }
}
