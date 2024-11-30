////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include <cassert>
#include <cmath>
#include <iostream>

#include "RichText.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <SFML/System/String.hpp>

#include <CoreManager.h>

namespace sfe
{

////////////////////////////////////////////////////////////////////////////////
void RichText::Line::setCharacterColor(std::size_t pos, sf::Color color)
{
    assert(pos < getLength());
    isolateCharacter(pos);
    std::size_t stringToFormat = convertLinePosToLocal(pos);
    m_texts[stringToFormat].setFillColor(color);
    updateGeometry();
}

////////////////////////////////////////////////////////////////////////////////
void RichText::Line::setCharacterStyle(std::size_t pos, sf::Text::Style style)
{
    assert(pos < getLength());
    isolateCharacter(pos);
    std::size_t stringToFormat = convertLinePosToLocal(pos);
    m_texts[stringToFormat].setStyle(style);
    updateGeometry();
}
////////////////////////////////////////////////////////////////////////////////
void RichText::Line::setCharacter(std::size_t pos, sf::Uint32 character)
{
    assert(pos < getLength());
    sf::Text& text = m_texts[convertLinePosToLocal(pos)];
    sf::String string = text.getString();
    string[pos] = character;
    text.setString(string);
    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::setCharacterSize(unsigned int size)
{
    for (sf::Text &text : m_texts)
        text.setCharacterSize(size);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::setFont(const sf::Font &font)
{
    for (sf::Text &text : m_texts)
        text.setFont(font);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
std::size_t RichText::Line::getLength() const
{
    std::size_t count = 0;
    for (sf::Text &text : m_texts)
    {
        count += text.getString().getSize();
    }
    return count;
}


////////////////////////////////////////////////////////////////////////////////
sf::Color RichText::Line::getCharacterColor(std::size_t pos) const
{
    assert(pos < getLength());
    return m_texts[convertLinePosToLocal(pos)].getFillColor();
}


////////////////////////////////////////////////////////////////////////////////
sf::Uint32 RichText::Line::getCharacterStyle(std::size_t pos) const
{
    assert(pos < getLength());
    return m_texts[convertLinePosToLocal(pos)].getStyle();
}


////////////////////////////////////////////////////////////////////////////////
sf::Uint32 RichText::Line::getCharacter(std::size_t pos) const
{
    assert(pos < getLength());
    sf::Text& text = m_texts[convertLinePosToLocal(pos)];
    return text.getString()[pos];
}


////////////////////////////////////////////////////////////////////////////////
const std::vector<sf::Text> &RichText::Line::getTexts() const
{
    return m_texts;
}

////////////////////////////////////////////////////////////////////////////////
void RichText::Line::appendText(sf::Text text)
{
    updateTextAndGeometry(text,text);
    m_reusableText = text;
    m_texts.push_back(std::move(text));
}


////////////////////////////////////////////////////////////////////////////////
sf::FloatRect RichText::Line::getLocalBounds() const
{
    return m_bounds;
}


////////////////////////////////////////////////////////////////////////////////
sf::FloatRect RichText::Line::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (const sf::Text &text : m_texts)
        target.draw(text, states);
}


////////////////////////////////////////////////////////////////////////////////
std::size_t RichText::Line::convertLinePosToLocal(std::size_t& pos) const
{
    assert(pos < getLength());
    std::size_t arrayIndex = 0;
    for (; pos >= m_texts[arrayIndex].getString().getSize(); ++arrayIndex)
    {
        pos -= m_texts[arrayIndex].getString().getSize();
    }
    return arrayIndex;
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::isolateCharacter(std::size_t pos)
{
    std::size_t localPos = pos;
    std::size_t index = convertLinePosToLocal(localPos);

    if (m_texts[index].getString().getSize() == 1)
        return;

    m_reusableString = m_texts[index].getString();

    if (localPos != 0)
    {
        m_reusableText.setString(m_reusableString.substring(0, localPos));
        m_texts.insert(m_texts.begin() + index, m_reusableText);
    }

    m_texts[index].setString(m_reusableString.substring(localPos, 1));

    if (localPos != m_reusableString.getSize() - 1)
    {
        m_reusableText.setString(m_reusableString.substring(localPos + 1));
        m_texts.insert(m_texts.begin() + index + 1, m_reusableText);
    }
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::updateGeometry() const
{
    m_bounds = sf::FloatRect();

    auto itCurr = m_texts.begin();
    auto itPrev = itCurr;

    for (++itCurr; itCurr != m_texts.end(); ++itCurr)
    {
        if (itPrev == m_texts.begin()) {
            updateTextAndGeometry(m_emptyText, *itPrev);
        }

        updateTextAndGeometry(*itPrev, *itCurr);
        ++itPrev;
    }
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::updateTextAndGeometry(sf::Text& prevText, sf::Text& text) const
{
    // Set text offset
    auto font = text.getFont();
    text.setPosition(m_bounds.width, 0.f);

    auto& prevStr = prevText.getString();
    auto& nextStr = text.getString();

    auto charSize = text.getCharacterSize();
    auto prevGlyph = prevStr[0];
    auto nextGlyph = nextStr[0];

    auto strRepo = CoreManager::getInstance().getStrRepo();

    if(m_bounds.width != 0)
    {
        std::pair<sf::Uint32, sf::Uint32> p = {prevGlyph, nextGlyph};
        float kerning = strRepo->GetKerning(p);

        if (kerning == -999) 
        {
            kerning = font->getKerning(prevGlyph, nextGlyph, charSize);
            strRepo->kerningStore[{prevGlyph, nextGlyph}] = kerning;
        }
        
        text.setPosition(m_bounds.width + kerning, 0.f);
    }

    // Update bounds
    float lineSpacing = std::floor(font->getLineSpacing(charSize));
    m_bounds.height = std::max(m_bounds.height, lineSpacing);

    if (!nextStr.isEmpty())
    {
        std::pair<sf::Uint32, sf::Uint32> p = {nextGlyph, charSize};
        float advance = strRepo->GetAdvance(p);

        if (advance == -999)
        {
            advance = font->getGlyph(nextGlyph, charSize, false).advance;
            strRepo->advanceStore[{nextGlyph, charSize}] = advance;
        }

        m_bounds.width += advance;
    }
}


////////////////////////////////////////////////////////////////////////////////
RichText::RichText()
    : RichText(nullptr)
{

}


////////////////////////////////////////////////////////////////////////////////
RichText::RichText(const sf::Font& font)
    : RichText(&font)
{

}

////////////////////////////////////////////////////////////////////////////////
RichText& RichText::operator << (const TextStroke& stroke)
{
    m_currentStroke = stroke;
    return *this;
}

RichText& RichText::operator << (const Outline& outline)
{
    m_currentStroke.outline = outline.outline;
    m_currentStroke.thickness = outline.thickness;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (const sf::Color& color)
{
    m_currentStroke.fill = color;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (sf::Text::Style style)
{
    m_currentStyle = style;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
std::vector<sf::String> explode(const sf::String& string, sf::Uint32 delimiter)
{
    if (string.isEmpty())
        return std::vector<sf::String>();

    // For each character in the string
    std::vector<sf::String> result;
    sf::String buffer;
    for (sf::Uint32 character : string) {
        // If we've hit the delimiter character
        if (character == delimiter) {
            // Add them to the result vector
            result.push_back(buffer);
            buffer.clear();
        } else {
            // Accumulate the next character into the sequence
            buffer += character;
        }
    }

    // Add to the result if buffer still has contents or if the last character is a delimiter
    if (!buffer.isEmpty() || string[string.getSize() - 1] == delimiter)
        result.push_back(buffer);

    return result;
}


////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (const sf::String& string)
{
    // Maybe skip
    if (string.isEmpty())
        return *this;

    // Explode into substrings
    std::vector<sf::String> subStrings = explode(string, '\n');

    // Append first substring using the last line
    auto it = subStrings.begin();
    if (it != subStrings.end()) {
        // If there isn't any line, just create it
        if (m_lines.empty())
            m_lines.resize(1);

        // Remove last line's height
        Line &line = m_lines.back();
        m_bounds.height -= line.getGlobalBounds().height;

        // Append text
        line.appendText(createText(*it));

        // Update bounds
        m_bounds.height += line.getGlobalBounds().height;
        m_bounds.width = std::max(m_bounds.width, line.getGlobalBounds().width);
    }

    // Append the rest of substrings as new lines
    while (++it != subStrings.end()) {
        Line line;
        line.setPosition(0.f, m_bounds.height);
        line.appendText(createText(*it));
        m_lines.push_back(std::move(line));

        // Update bounds
        m_bounds.height += line.getGlobalBounds().height;
        m_bounds.width = std::max(m_bounds.width, line.getGlobalBounds().width);
    }

    // Return
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
void RichText::setCharacterColor(std::size_t line, std::size_t pos, sf::Color color)
{
    assert(line < m_lines.size());
    m_lines[line].setCharacterColor(pos, color);
    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::setCharacterStyle(std::size_t line, std::size_t pos, sf::Text::Style style)
{
    assert(line < m_lines.size());
    m_lines[line].setCharacterStyle(pos, style);
    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::setCharacter(std::size_t line, std::size_t pos, sf::Uint32 character)
{
    assert(line < m_lines.size());
    m_lines[line].setCharacter(pos, character);
    updateGeometry();
}

////////////////////////////////////////////////////////////////////////////////
void RichText::setCharacterSize(unsigned int size)
{
    // Maybe skip
    if (m_characterSize == size)
        return;

    // Update character size
    m_characterSize = size;

    // Set texts character size
    for (Line &line : m_lines)
        line.setCharacterSize(size);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::setFont(const sf::Font& font)
{
    // Maybe skip
    if (m_font == &font)
        return;

    // Update font
    m_font = &font;

    // Set texts font
    for (Line &line : m_lines)
        line.setFont(font);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::clear()
{
    // Clear texts
    m_lines.clear();

    // Reset bounds
    m_bounds = sf::FloatRect();
}


////////////////////////////////////////////////////////////////////////////////
sf::Color RichText::getCharacterColor(std::size_t line, std::size_t pos) const
{
    assert(line < m_lines.size());
    return m_lines[line].getCharacterColor(pos);
}


////////////////////////////////////////////////////////////////////////////////
sf::Uint32 RichText::getCharacterStyle(std::size_t line, std::size_t pos) const
{
    assert(line < m_lines.size());
    return m_lines[line].getCharacterStyle(pos);
}


////////////////////////////////////////////////////////////////////////////////
sf::Uint32 RichText::getCharacter(std::size_t line, std::size_t pos) const
{
    assert(line < m_lines.size());
    return m_lines[line].getCharacter(pos);
}


////////////////////////////////////////////////////////////////////////////////
const std::vector<RichText::Line> &RichText::getLines() const
{
    return m_lines;
}


////////////////////////////////////////////////////////////////////////////////
unsigned int RichText::getCharacterSize() const
{
    return m_characterSize;
}


////////////////////////////////////////////////////////////////////////////////
const sf::Font *RichText::getFont() const
{
    return m_font;
}


////////////////////////////////////////////////////////////
sf::FloatRect RichText::getLocalBounds() const
{
    return m_bounds;
}


////////////////////////////////////////////////////////////
sf::FloatRect RichText::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////////////////////////
void RichText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (const Line &line : m_lines)
        target.draw(line, states);
}


////////////////////////////////////////////////////////////////////////////////
RichText::RichText(const sf::Font* font)
    : m_font(font),
      m_characterSize(30),
      m_currentStroke{ sf::Color::White, sf::Color::Transparent },
      m_currentStyle(sf::Text::Regular)
{

}


////////////////////////////////////////////////////////////////////////////////
sf::Text RichText::createText(const sf::String& string) const
{
    sf::Text text;
    text.setString(string);
    text.setFillColor(m_currentStroke.fill);
    text.setOutlineColor(m_currentStroke.outline);
    text.setOutlineThickness(m_currentStroke.thickness);
    text.setStyle(m_currentStyle);
    text.setCharacterSize(m_characterSize);
    if (m_font)
    {
        text.setFont(*m_font);
    }
    return text;
}


////////////////////////////////////////////////////////////////////////////////
void RichText::updateGeometry() const
{
    m_bounds = sf::FloatRect();

    for (Line &line : m_lines) {
        line.setPosition(0.f, m_bounds.height);

        m_bounds.height += line.getGlobalBounds().height;
        m_bounds.width = std::max(m_bounds.width, line.getGlobalBounds().width);
    }
}

}
