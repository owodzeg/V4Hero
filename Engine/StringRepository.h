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

    float GetKerning(std::pair<char32_t, char32_t>& pair);
    float GetAdvance(std::pair<char32_t, char32_t>& pair);

    std::unordered_map<std::string, std::optional<std::string>> langToFontMapping; // Maps language to font name
    std::unordered_map<std::string, std::shared_ptr<sf::Font>> fontStore;  // Use shared_ptr for automatic memory management
    std::unordered_map<std::pair<char32_t, char32_t>, std::optional<float>, pair_hash<char32_t, char32_t>> kerningStore; // Store font kerning
    std::unordered_map<std::pair<char32_t, char32_t>, std::optional<float>, pair_hash<char32_t, char32_t>> advanceStore; // Store letter advance, [glyph][size]

    struct GlyphCache
    {
        double advance = 0;
        double height = 0;
        double advance_bold = 0;
        double height_bold = 0;
    };

    // new for PataText
    // kerningCache[font][charSize][{char1, char2}]
    std::unordered_map<std::string, std::unordered_map<double, std::unordered_map<std::pair<char32_t, char32_t>, std::optional<double>, pair_hash<char32_t, char32_t>>>> kerningCache;

    // advanceCache[font][charSize][char]
    std::unordered_map<std::string, std::unordered_map<double, std::unordered_map<char32_t, GlyphCache>>> advanceCache;

    float GetKerningForFont(const std::string& fontName, std::pair<char32_t, char32_t>& pair, const double charSize);
    float GetAdvanceForFont(const std::string& fontName, char32_t& character, const double charSize, bool bold);
    float GetHeightForFont(const std::string& fontName, char32_t& character, const double charSize, bool bold);

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> languages;
    std::unordered_map<std::string, std::string> languageNativeNames;
    std::string currentLanguageCode;

    void ParseLanguageData(std::istream& inputStream, std::unordered_map<std::string, std::string>& langMap);
};

#endif // STRINGREPOSITORY_H