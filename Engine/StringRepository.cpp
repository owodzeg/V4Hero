#include "StringRepository.h"
#include "Func.h"
#include <spdlog/spdlog.h>
#include <sstream>
#include <fstream>

StringRepository::StringRepository() : currentLanguageCode("us")
{
    // get fonts
    std::ifstream fontFile("resources/lang/fonts.txt");
    std::string line;

    while(std::getline(fontFile, line))
    {
        std::vector<std::string> param = Func::Split(line, ',');
        std::string name = param[0];
        std::string fontf = param[1];

        LoadFontFromFile("resources/font/"+fontf, name);

        SPDLOG_INFO("Loaded font file {} for language {}", fontf, name);
    }

    // get langs
    std::ifstream langFile("resources/lang/languages.txt");

    while(std::getline(langFile, line))
    {
        std::vector<std::string> param = Func::Split(line, ',');
        std::string code = param[0];
        std::string name = param[1];
        std::string font = param[2];

        LoadLanguageFile(code, name, "resources/lang/"+code+"/"+code+".txt");
        langToFontMapping[code] = font;

        SPDLOG_INFO("Loaded language {} {}, font file: {}", code, name, font);
    }
}

void StringRepository::Reload()
{
    langToFontMapping.clear();
    languages.clear();
    languageNativeNames.clear();
    kerningStore.clear();
    advanceStore.clear();

    // get fonts
    std::ifstream fontFile("resources/lang/fonts.txt");
    std::string line;

    while(std::getline(fontFile, line))
    {
        std::vector<std::string> param = Func::Split(line, ',');
        std::string name = param[0];
        std::string fontf = param[1];

        LoadFontFromFile("resources/font/"+fontf, name);

        SPDLOG_INFO("Loaded font file {} for language {}", fontf, name);
    }

    // get langs
    std::ifstream langFile("resources/lang/languages.txt");

    while(std::getline(langFile, line))
    {
        std::vector<std::string> param = Func::Split(line, ',');
        std::string code = param[0];
        std::string name = param[1];
        std::string font = param[2];

        LoadLanguageFile(code, name, "resources/lang/"+code+"/"+code+".txt");
        langToFontMapping[code] = font;

        SPDLOG_INFO("Loaded language {} {}, font file: {}", code, name, font);
    }
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
        kerningStore.clear();
        advanceStore.clear();
        SPDLOG_INFO("Current language set to: {}", countryCode);
    }
    else
    {
        SPDLOG_WARN("Language with code {} not loaded.", countryCode);
    }
}

std::string& StringRepository::GetCurrentLanguage()
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

// Load font from memory (std::string data)
void StringRepository::LoadFontFromString(const std::string& fontName, const std::vector<char>& fontData)
{
    if (fontStore.find(fontName) != fontStore.end()) {
        fontStore.erase(fontStore.find(fontName));
    }

    if (fontStore.find(fontName) == fontStore.end()) {
        std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();
        if (!font->openFromMemory(fontData.data(), fontData.size())) {
            throw std::runtime_error("Failed to load font from memory.");
        }
        fontStore[fontName] = std::move(font);
    }

    SPDLOG_INFO("Font '{}' loaded successfully", fontName);
}
// Load font from memory (std::string data)
void StringRepository::LoadFontFromFile(const std::string& fontPath, const std::string& fontName)
{
    if (fontStore.find(fontName) != fontStore.end()) {
        fontStore.erase(fontStore.find(fontName));
    }

    if (fontStore.find(fontName) == fontStore.end()) {
        std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();
        if (!font->openFromFile(fontPath)) {
            throw std::runtime_error("Failed to load font: " + fontPath);
        }
        fontStore[fontName] = std::move(font);
    }

    SPDLOG_INFO("Font '{}' loaded successfully", fontName);
}

std::string& StringRepository::GetFontNameForLanguage(const std::string& countryCode)
{
    auto& fontName = langToFontMapping[countryCode];

    if (fontName == std::nullopt)
    {
        SPDLOG_WARN("No font found for language '{}'", countryCode);
        return defaultFont;
    }
    return fontName.value();
}

sf::Font& StringRepository::GetFontFromName(const std::string& fontName)
{
    auto it = fontStore.find(fontName);
    if (it != fontStore.end()) {
        return *it->second;
    } else {
        throw std::runtime_error("Font not found: " + fontName);
    }
}

float StringRepository::GetKerning(std::pair<char32_t, char32_t>& pair)
{
    return kerningStore[pair].value_or(-999);
}

float StringRepository::GetAdvance(std::pair<char32_t, char32_t>& pair)
{
    return advanceStore[pair].value_or(-999);
}

float StringRepository::GetKerningForFont(const std::string& fontName, std::pair<char32_t, char32_t>& pair, const double charSize)
{
    if (kerningCache.contains(fontName)) {
        auto& fontCache = kerningCache[fontName];
        if (fontCache.contains(charSize))
        {
            auto& sizeCache = fontCache[charSize];
            if (sizeCache.contains(pair))
            {
                return sizeCache[pair].value();
            }
        }
    }

    // cache not found
    auto& font = GetFontFromName(fontName);
    float kerning = font.getKerning(pair.first, pair.second, charSize);
    kerningCache[fontName][charSize][pair] = kerning;
    return kerning;
}

float StringRepository::GetAdvanceForFont(const std::string& fontName, char32_t& character, const double charSize, bool bold)
{
    if (advanceCache.contains(fontName))
    {
        auto& fontCache = advanceCache[fontName];
        if (fontCache.contains(charSize))
        {
            auto& sizeCache = fontCache[charSize];
            if (sizeCache.contains(character))
            {
                return bold ? sizeCache[character].advance_bold : sizeCache[character].advance;
            }
        }
    }
    // cache not found
    auto& font = GetFontFromName(fontName);
    auto glyph = font.getGlyph(character, charSize, false);
    auto glyphBold = font.getGlyph(character, charSize, true);
    advanceCache[fontName][charSize][character] = {glyph.advance, glyph.bounds.size.y, glyphBold.advance, glyphBold.bounds.size.y};
    return bold ? glyphBold.advance : glyph.advance;
}

float StringRepository::GetHeightForFont(const std::string& fontName, char32_t& character, const double charSize, bool bold)
{
    // Try to get the cached value directly, if available
    auto& fontCache = advanceCache[fontName];
    auto& sizeCache = fontCache[charSize];

    if (sizeCache.contains(character))
    {
        const auto& cacheEntry = sizeCache[character];
        return bold ? cacheEntry.height_bold : cacheEntry.height;
    }

    // If cache miss, calculate and store values
    auto& font = GetFontFromName(fontName);

    // Retrieve both regular and bold glyphs once to minimize redundant calls
    auto glyph = font.getGlyph(character, charSize, false);
    auto glyphBold = font.getGlyph(character, charSize, true);

    // Store the values in cache, including both regular and bold heights
    sizeCache[character] = {glyph.advance, glyph.bounds.size.y, glyphBold.advance, glyphBold.bounds.size.y};

    return bold ? glyphBold.bounds.size.y : glyph.bounds.size.y;
}