#ifndef CONFIG_H
#define CONFIG_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>

class Config
{
    public:
    std::vector<std::string> configKeys = {"resX", "resY", "framerateLimit", "enableFullscreen", "enableBorderlessWindow", "enableDrumChants", "enableDrums", "enablePataponChants", "keybindPata", "keybindPon", "keybindDon", "keybindChaka", "secondaryKeybindPata", "secondaryKeybindPon", "secondaryKeybindDon", "secondaryKeybindChaka","debugTheme","debugBackground"};
    std::vector<std::string> configDefaults = {"1280", "720", "240", "0", "0", "0", "1", "1", "71", "72", "74", "73",std::to_string(sf::Keyboard::A), std::to_string(sf::Keyboard::D), std::to_string(sf::Keyboard::S), std::to_string(sf::Keyboard::W),"Freakout Rock Theme","nanjarohills"};
    std::map<std::string,std::string> configMap;

    Config();
    void LoadConfig();
    ///I wanted to overload these two in a single function, but somehow it just doesnt want to work...
    ///Will change it into one someday
    int GetInt(std::string key);
    std::string GetString(std::string key);
    void SaveConfig();
};

#endif // CONFIG_H
