#include "MenuButton.h"
#include "string"

MenuButton::MenuButton(std::string text,sf::Font *font,int fontSize, float y )
{
    buttonText = text;
    t_buttonText.setFont(*font);
    t_buttonText.setCharacterSize(fontSize);
    t_buttonText.setFillColor(sf::Color(100,100,100));
    t_buttonText.setString(text);
    t_buttonText.setOrigin(t_buttonText.getGlobalBounds().width/2,t_buttonText.getGlobalBounds().height/2);

    //originX = x;
    originY = y;
}
void MenuButton::Update(sf::RenderWindow &window, float fps)
{
    t_buttonText.setPosition(window.getSize().x/2,originY);
    window.draw(t_buttonText);
}

void MenuButton::SetSelected(bool isSelected)
{
    selected = isSelected;

    if(selected)
    {
         t_buttonText.setFillColor(sf::Color::White);
    }
    else
    {
        t_buttonText.setFillColor(sf::Color(100,100,100));
    }
}

MenuButton::~MenuButton()
{

}
