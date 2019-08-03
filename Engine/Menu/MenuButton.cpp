#include "MenuButton.h"
#include "string"
#include "ButtonList.h"
MenuButton::MenuButton(std::wstring text,sf::Font *font,int fontSize, float y , ButtonList *p_Menu,int menuListIndex)
{
    buttonListIndex = menuListIndex;
    buttonText = text;
    parentList = p_Menu;
    t_buttonText.setFont(*font);
    t_buttonText.setCharacterSize(fontSize);
    t_buttonText.setFillColor(sf::Color(100,100,100));
    t_buttonText.setString(text);
    t_buttonText.setOrigin(t_buttonText.getGlobalBounds().width/2,t_buttonText.getGlobalBounds().height/2);
    debug_text_bounding_box.setFillColor(sf::Color::Red);
    //originX = x;
    originY = y;
}
void MenuButton::Update(sf::RenderWindow &window, float fps, sf::Vector2f *mousePos)
{
    sf::FloatRect textBounds = t_buttonText.getGlobalBounds();
    debug_text_bounding_box.setPosition(textBounds.left, textBounds.top);
    sf::Vector2f recSize(textBounds.width, textBounds.height);
    debug_text_bounding_box.setSize(recSize);

    if (textBounds.contains(*mousePos)){
        parentList->HighlightButton(this,buttonListIndex);
    }


    t_buttonText.setPosition(window.getSize().x/2,originY);
    //window.draw(debug_text_bounding_box);   Don't draw the bounding boxes for now. Later we can make a debug keybind for it - something like ctrl + f10
    window.draw(t_buttonText);
}
void MenuButton::MouseUp(int xPos,int yPos){
    sf::FloatRect textBounds = t_buttonText.getGlobalBounds();
    if (textBounds.contains(sf::Vector2f(xPos,yPos))){
        parentList->SelectButton(buttonListIndex);
    }

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
