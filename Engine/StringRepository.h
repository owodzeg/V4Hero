#ifndef STRINGREPOSITORY_H
#define STRINGREPOSITORY_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>  // For smart pointers

class StringRepository
{
public:
    StringRepository();
    bool refreshStrings = false;

    void LoadLanguageFile(const std::string& countryCode, const std::string& countryNativeName, const std::string& filename);
    void LoadLanguageFromString(const std::string& countryCode, const std::string& countryNativeName, const std::string& langContent);

    void SetCurrentLanguage(const std::string& countryCode);
    std::string GetCurrentLanguage() const;
    std::string GetString(const std::string& key) const;

    std::vector<std::pair<std::string, std::string>> GetAvailableLanguages() const;

    // Font loading and retrieval
    void LoadFontFromString(const std::string& fontName, const std::vector<char>& fontData);
    void LoadFontFromFile(const std::string& fontPath, const std::string& fontName);
    std::string GetFontNameForLanguage(const std::string& countryCode);
    sf::Font& GetFontFromName(const std::string& fontName);

    std::unordered_map<std::string, std::string> langToFontMapping; // Maps language to font name
    std::unordered_map<std::string, sf::Font> fontStore;  // Use shared_ptr for automatic memory management

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> languages;
    std::unordered_map<std::string, std::string> languageNativeNames;
    std::string currentLanguageCode;

    void ParseLanguageData(std::istream& inputStream, std::unordered_map<std::string, std::string>& langMap);
};

#endif // STRINGREPOSITORY_H