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
    template<typename T1, typename T2>
    struct pair_hash {
        std::size_t operator()(const std::pair<T1, T2>& p) const
        {
            // Combine the hash of the first element and the second element
            std::size_t hash1 = std::hash<T1>{}(p.first);
            std::size_t hash2 = std::hash<T2>{}(p.second);

            // Use bitwise XOR and bit shifting to combine the hashes
            return hash1 ^ (hash2 << 1); // hash1 XOR shifted hash2
        }
    };

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

    float GetKerning(std::pair<sf::Uint32, sf::Uint32>& pair);
    float GetAdvance(std::pair<sf::Uint32, sf::Uint32>& pair);

    std::unordered_map<std::string, std::optional<std::string>> langToFontMapping; // Maps language to font name
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fontStore;  // Use shared_ptr for automatic memory management
    std::unordered_map<std::pair<sf::Uint32, sf::Uint32>, std::optional<float>, pair_hash<sf::Uint32, sf::Uint32>> kerningStore; // Store font kerning
    std::unordered_map<std::pair<sf::Uint32, sf::Uint32>, std::optional<float>, pair_hash<sf::Uint32, sf::Uint32>> advanceStore; // Store letter advance, [glyph][size]

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> languages;
    std::unordered_map<std::string, std::string> languageNativeNames;
    std::string currentLanguageCode;

    void ParseLanguageData(std::istream& inputStream, std::unordered_map<std::string, std::string>& langMap);
};

#endif // STRINGREPOSITORY_H