#include "StringRepository.h"
#include "Func.h"
#include <spdlog/spdlog.h>
#include <sstream>
#include <fstream>

StringRepository::StringRepository() : currentLanguageCode("us")
{
    // Set English (us) as the default language
}

// Parse language data from input stream (file or stringstream)
void StringRepository::ParseLanguageData(std::istream& inputStream, std::unordered_map<std::string, std::string>& langMap)
{
    std::string line;
    while (std::getline(inputStream, line))
    {
        // Ignore empty lines and comments
        if (line.empty() || line.find("//") != std::string::npos) continue;

        // Split by the delimiter `|` (key|value)
        auto keyValue = Func::Split(line, '|');
        if (keyValue.size() == 2)
        {
            langMap[keyValue[0]] = keyValue[1];
        }
    }
}

// Load language file from disk
void StringRepository::LoadLanguageFile(const std::string& countryCode, const std::string& countryNativeName, const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        SPDLOG_ERROR("Failed to open language file: {}", filename);
        return;
    }

    std::unordered_map<std::string, std::string>& langMap = languages[countryCode];
    languageNativeNames[countryCode] = countryNativeName;
    ParseLanguageData(file, langMap);
}

// Load language data from a string
void StringRepository::LoadLanguageFromString(const std::string& countryCode, const std::string& countryNativeName, const std::string& langContent)
{
    std::istringstream stream(langContent);
    std::unordered_map<std::string, std::string>& langMap = languages[countryCode];
    languageNativeNames[countryCode] = countryNativeName;
    ParseLanguageData(stream, langMap);
}

// Set the current language to be used for translations
void StringRepository::SetCurrentLanguage(const std::string& countryCode)
{
    if (languages.find(countryCode) != languages.end())
    {
        currentLanguageCode = countryCode;
        refreshStrings = true;
        SPDLOG_INFO("Current language set to: {}", countryCode);
    }
    else
    {
        SPDLOG_WARN("Language with code {} not loaded.", countryCode);
    }
}

std::string StringRepository::GetCurrentLanguage() const
{
    return currentLanguageCode;
}

// Get a string for the current language using a key
std::string StringRepository::GetString(const std::string& key) const
{
    auto langIt = languages.find(currentLanguageCode);
    if (langIt != languages.end())
    {
        const auto& langMap = langIt->second;
        auto it = langMap.find(key);
        if (it != langMap.end())
        {
            return it->second;
        }
    }
    SPDLOG_WARN("Key '{}' not found for language '{}'", key, currentLanguageCode);
    return "language "+currentLanguageCode+"{n}key "+key+"{n}missing translation";
}

// Get a list of available languages and their native names
std::vector<std::pair<std::string, std::string>> StringRepository::GetAvailableLanguages() const
{
    std::vector<std::pair<std::string, std::string>> langs;
    for (const auto& lang : languages)
    {
        langs.emplace_back(lang.first, languageNativeNames.at(lang.first));
    }
    std::sort(langs.begin(), langs.end());
    return langs;
}

// Load font from memory (string data)
void StringRepository::LoadFontFromString(const std::string& fontName, const std::vector<char>& fontData)
{
    sf::Font font;
    if (!font.loadFromMemory(fontData.data(), fontData.size())) {
        SPDLOG_ERROR("Could not load font '{}'", fontName);
        return;
    }

    // Insert font into the map using emplace or insert
    fontStore.emplace(fontName, std::move(font));  // Move the font to avoid copying
    SPDLOG_INFO("Font '{}' loaded successfully", fontName);
}
// Load font from memory (string data)
void StringRepository::LoadFontFromFile(const std::string& fontPath, const std::string& fontName)
{
    sf::Font font;
    if (!font.loadFromFile(fontPath)) {
        SPDLOG_ERROR("Could not load font '{}'", fontPath);
        return;
    }

    // Insert font into the map using emplace or insert
    fontStore.emplace(fontName, std::move(font));  // Move the font to avoid copying
    SPDLOG_INFO("Font '{}' loaded successfully", fontName);
}

std::string StringRepository::GetFontNameForLanguage(const std::string& countryCode)
{
    auto it = langToFontMapping.find(countryCode);
    if (it != langToFontMapping.end())
    {
        return it->second;
    }
    SPDLOG_WARN("No font found for language '{}'", countryCode);
    assert(0);
}

sf::Font& StringRepository::GetFontFromName(const std::string& fontName)
{
    auto fontIt = fontStore.find(fontName);
    if (fontIt != fontStore.end())
    {
        return fontIt->second;
    }
    SPDLOG_WARN("Font '{}' not found", fontName);
    assert(0);
}