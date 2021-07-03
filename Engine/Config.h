#ifndef CONFIG_H
#define CONFIG_H
#include "StringRepository.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
class V4Core;
class Config
{
public:
    std::vector<std::string> configKeys = {"resX",
                                           "resY",
                                           "textureQuality",
                                           "framerateLimit",
                                           "verticalSync",
                                           "showFPS",
                                           "highRange",
                                           "lowRange",
                                           "enableFullscreen",
                                           "enableBorderlessWindow",
                                           "enableDrumChants",
                                           "enableDrums",
                                           "enablePataponChants",
                                           "keybindBack",
                                           "mission1Theme",
                                           "mission1Background",
                                           "mission2Theme",
                                           "mission2Background",
                                           "showDebugMsg",
                                           "showRhythmDebugMsg",
                                           "showImportantDebugMsg",
                                           "lang",
                                           "masterVolume",
                                           "sfxVolume",
                                           "bgmVolume",
                                           "antiAliasing",
                                           "seasonalEvents",
                                           "keybindSquare1",
                                           "keybindCircle1",
                                           "keybindCross1",
                                           "keybindTriangle1",
                                           "keybindLTrigger1",
                                           "keybindRTrigger1",
                                           "keybindLeft1",
                                           "keybindRight1",
                                           "keybindDown1",
                                           "keybindUp1",
                                           "keybindStart1",
                                           "keybindSelect1",
                                           "keybindSquare2",
                                           "keybindCircle2",
                                           "keybindCross2",
                                           "keybindTriangle2",
                                           "keybindLTrigger2",
                                           "keybindRTrigger2",
                                           "keybindLeft2",
                                           "keybindRight2",
                                           "keybindDown2",
                                           "keybindUp2",
                                           "keybindStart2",
                                           "keybindSelect2"};
    std::vector<std::string> configDefaults = {"1280",
                                               "720",
                                               "1",
                                               "240",
                                               "1",
                                               "0",
                                               "210",
                                               "135",
                                               "0",
                                               "0",
                                               "0",
                                               "1",
                                               "1",
                                               std::to_string(sf::Keyboard::Escape),
                                               "Pokkurimakka Rock Theme",
                                               "shidavalley",
                                               "Pokkurimakka Rock Theme",
                                               "evilmass",
                                               "0",
                                               "0",
                                               "1",
                                               "1",
                                               "50",
                                               "100",
                                               "100",
                                               "0",
                                               "1",
                                               "0",
                                               "23",
                                               "25",
                                               "18",
                                               "16",
                                               "4",
                                               "71",
                                               "72",
                                               "74",
                                               "73",
                                               "58",
                                               "57",
                                               "1000",
                                               "1002",
                                               "1001",
                                               "1003",
                                               "1004",
                                               "1005",
                                               "1100",
                                               "1101",
                                               "1102",
                                               "1103",
                                               "1008",
                                               "1009"};
    std::map<std::string, std::string> configMap;
    int configDebugID = 0;
    bool changedLang = false;
    bool se_christmas = false;
    std::string fontPath = "";
    Config();
    void LoadConfig(V4Core* core);
    void ReloadLanguages();
    V4Core* thisCore;

    StringRepository strRepo;
    ///I wanted to overload these two in a single function, but somehow it just doesnt want to work...
    ///Will change it into one someday
    int GetInt(std::string key);
    std::string GetString(std::string key);
    void SetString(std::string key, std::string val);
    std::wstring GetLanguageName();
    void SaveConfig();
    bool keyExists(std::string key);
};

#endif // CONFIG_H
