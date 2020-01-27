#ifndef CONFIG_H
#define CONFIG_H
#include <SFML/Graphics.hpp>
#include "StringRepository.h"
#include <vector>
#include <map>
#include <string>
#include "DebugOut.h"
class V4Core;
class Config
{
    public:
    std::vector<std::string> configKeys = {"resX",
    "resY",
    "textureQuality",
    "framerateLimit",
    "enableFullscreen",
    "enableBorderlessWindow",
    "enableDrumChants",
    "enableDrums",
    "enablePataponChants",
    "keybindPata",
    "keybindPon",
    "keybindDon",
    "keybindChaka",
    "keybindSpace",
    "secondaryKeybindPata",
    "secondaryKeybindPon",
    "secondaryKeybindDon",
    "secondaryKeybindChaka",
    "keybindMenuEnter",
    "keybindBack",
    "mission1Theme",
    "mission1Background",
    "mission2Theme",
    "mission2Background",
    "showDebugMsg",
    "showRhythmDebugMsg",
    "showImportantDebugMsg",
    "showKeyboardPressCode",
    "lang"};
    std::vector<std::string> configDefaults = {"1280",
    "720",
    "1",
    "240",
    "0",
    "0",
    "0",
    "1",
    "1",
    "71",
    "72",
    "74",
    "73",
    "5",
    std::to_string(sf::Keyboard::A),
    std::to_string(sf::Keyboard::D),
    std::to_string(sf::Keyboard::S),
    std::to_string(sf::Keyboard::W),
    std::to_string(sf::Keyboard::Enter),
    std::to_string(sf::Keyboard::Escape),
    "Hey Whazzat Rock Theme",
    "nanjarohills",
    "Pokkurimakka Rock Theme",
    "evilmass",
    "0",
    "0",
    "1",
    "0",
    "1"};
    std::map<std::string,std::string> configMap;
    int configDebugID=0;
    bool changedLang = false;
    Config();
    void LoadConfig(V4Core* core);
    void ReloadLanguages();
    DebugOut* debugOut;
    V4Core* thisCore;

    StringRepository strRepo;
    ///I wanted to overload these two in a single function, but somehow it just doesnt want to work...
    ///Will change it into one someday
    int GetInt(std::string key);
    std::string GetString(std::string key);
    void SetString(std::string key,std::string val);
    std::wstring GetLanguageName();
    void SaveConfig();
};

#endif // CONFIG_H
