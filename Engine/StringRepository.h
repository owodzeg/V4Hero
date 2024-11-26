#ifndef STRINGREPOSITORY_H
#define STRINGREPOSITORY_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>  // For smart pointers
#include <optional>

class StringRepository
{
public:
    StringRepository();
    void Reload();

    bool refreshStrings = false;
    std::string defaultFont = "fallback";

    void LoadLanguageFile(const std::string& countryCode, const std::string& countryNativeName, const std::string& filename);
    void LoadLanguageFromString(const std::string& countryCode, const std::string& countryNativeName, const std::string& langContent);

    void SetCurrentLanguage(const std::string& countryCode);
    std::string& GetCurrentLanguage();
    std::string GetString(const std::string& key) const;

    std::vector<std::pair<std::string, std::string>> GetAvailableLanguages() const;

    // Font loading and retrieval
    void LoadFontFromString(const std::string& fontName, const std::vector<char>& fontData);
    void LoadFontFromFile(const std::string& fontPath, const std::string& fontName);
    std::string& GetFontNameForLanguage(const std::string& countryCode);
    sf::Font& GetFontFromName(const std::string& fontName);

    float GetKerning(std::string& font, sf::Uint32& char1, sf::Uint32& char2);
    float GetAdvance(std::string& font, sf::Uint32& char1, unsigned int& fsize);

    std::unordered_map<std::string, std::optional<std::string>> langToFontMapping; // Maps language to font name
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fontStore;  // Use shared_ptr for automatic memory management
    std::unordered_map<std::string, std::unordered_map<sf::Uint32, std::unordered_map<sf::Uint32, std::optional<float>>>> kerningStore; // Store font kerning
    std::unordered_map<std::string, std::unordered_map<sf::Uint32, std::unordered_map<unsigned int, std::optional<float>>>> advanceStore; // Store letter advance, [font][glyph][size]

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> languages;
    std::unordered_map<std::string, std::string> languageNativeNames;
    std::string currentLanguageCode;

    void ParseLanguageData(std::istream& inputStream, std::unordered_map<std::string, std::string>& langMap);
};

#endif // STRINGREPOSITORY_H