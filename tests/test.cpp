#include "SaveFile.h"

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using json = nlohmann::json;

TEST_CASE("deserialization", "is ok")
{
    std::ifstream conf("test-resources/new_save.p4sv", std::ios::in);
    json save_data;
    conf >> save_data;
    SaveFile save_file = save_data.get<SaveFile>();

    SaveFile expected_save_file = SaveFile{
        .save = SaveData{
            .army = Army{
                .hero = SquadSlot{
                    .rarepon = -1,
                    ._class = 0,
                    .level = 0,
                    .exp = 0,
                    .slots = {},
                },
                .squads = {
                    {
                        SquadSlot{
                            .rarepon = 0,
                            ._class = 0,
                            .level = 1,
                            .exp = 0,
                            .slots = {
                                {3, 0, 0},
                                {4, 1, 0},
                                {-1},
                                {-1},
                                {-1}
                            }
                        },
                        SquadSlot{
                            .rarepon = 1,
                            ._class = 0,
                            .level = 1,
                            .exp = 0,
                            .slots = {
                                {3, 0, 0},
                                {4, 1, 0},
                                {-1},
                                {-1},
                                {-1}
                            }
                        },
                        SquadSlot{
                            .rarepon = 2,
                            ._class = 0,
                            .level = 1,
                            .exp = 0,
                            .slots = {
                                {3, 0, 0},
                                {4, 1, 0},
                                {-1},
                                {-1},
                                {-1}
                            }
                        },
                        SquadSlot{
                            .rarepon = 3,
                            ._class = 0,
                            .level = 1,
                            .exp = 0,
                            .slots = {
                                {3, 0, 0},
                                {4, 1, 0},
                                {-1},
                                {-1},
                                {-1}
                            }
                        },
                        SquadSlot{
                            .rarepon = 4,
                            ._class = 0,
                            .level = 1,
                            .exp = 0,
                            .slots = {
                                {3, 0, 0},
                                {4, 1, 0},
                                {-1},
                                {-1},
                                {-1}
                            }
                        },
                        SquadSlot{
                            .rarepon = 5,
                            ._class = 0,
                            .level = 1,
                            .exp = 0,
                            .slots = {
                                {3, 0, 0},
                                {4, 1, 0},
                                {-1},
                                {-1},
                                {-1}
                            }
                        }
                    }
                }
            },
            .details = SaveFileDetails{
                .version = "2.0",
                .name = "Kamipon",
                .locations_unlocked = {1},
                .story_point = 1
            },
            .items = {
                SavedItem{
                    .name = "item_wooden_spear",
                    .count = 6
                },
                SavedItem{
                    .name = "item_wooden_helmet",
                    .count = 6
                }
            },
            .missions = {
                SavedMission{
                    .id = 1,
                    .level = 0,
                    .unlocked = true
                }
            }
        }
    };

    REQUIRE(save_file == expected_save_file);
}
