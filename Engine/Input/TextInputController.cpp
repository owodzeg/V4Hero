#include "TextInputController.h"

TextInputController::TextInputController()
{

}

void TextInputController::latchOn(sf::String& targetString) {
    m_targetString = &targetString;
}

void TextInputController::release() {
    m_targetString = nullptr;
}

void TextInputController::parseEvents(const sf::Event& event) {
    if (m_targetString == nullptr) return;  // Ensure we're latched to something

    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) {  // Backspace
            if (m_targetString->getSize() > 0) {
                m_targetString->erase(std::distance(m_targetString->begin(),m_targetString->end()-1));
            }
        } else if(event.text.unicode == 9 || event.text.unicode == 13)
        {
            special = 1;
        } else {
            m_targetString->insert(m_targetString->getSize(), static_cast<wchar_t>(event.text.unicode));
        }
    }
}

int TextInputController::sendSpecial()
{
    int old = int(special);
    special = -1;
    return old;
}