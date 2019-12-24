#include "MenuButton.h"
#include "string"
#include "ButtonList.h"
#include "../Func.h"



/// PLEASE NOTE: The functionality of actually pressing a button is held in the ButtonList. This class just deals with display and mouse events.


MenuButton::MenuButton(std::wstring text,sf::Font *font,int fontSize, float y , ButtonList *p_Menu,int menuListIndex,float x)
{
    buttonListIndex = menuListIndex;
    buttonText = text;
    parentList = p_Menu;
    t_buttonText.setFont(*font);
    t_buttonText.setCharacterSize(fontSize);
    t_buttonText.setFillColor(sf::Color(100,100,100));
    t_buttonText.setString(Func::ConvertToUtf8String(parentList->config->strRepo.GetUnicodeString(text)));

    debug_text_bounding_box.setFillColor(sf::Color::Red);
    //originX = x;
    originY = y;
    originX = x;
    if (originX!=-1){
        t_buttonText.setOrigin(0,t_buttonText.getGlobalBounds().height/2);
    } else {
        t_buttonText.setOrigin(t_buttonText.getGlobalBounds().width/2,t_buttonText.getGlobalBounds().height/2);
    }
}
void MenuButton::AddEndString(std::wstring text){
    endString = text;
    ///t_buttonText.setString(Func::ConvertToUtf8String(text)+"EGG");
    t_buttonText.setString(Func::ConvertToUtf8String(parentList->config->strRepo.GetUnicodeString(buttonText))+" "+Func::ConvertToUtf8String(endString));
    if (originX!=-1){
        t_buttonText.setOrigin(0,t_buttonText.getGlobalBounds().height/2);
    } else {
        t_buttonText.setOrigin(t_buttonText.getGlobalBounds().width/2,t_buttonText.getGlobalBounds().height/2);
    }

}
std::wstring MenuButton::GetEndString(std::wstring text){
    return endString;
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
    else if(!parentList->SelectButtons)
    {
        parentList->DeselectButton(buttonListIndex);
    }

    if (originX!=-1){
        t_buttonText.setPosition(originX,originY);
    } else {
        t_buttonText.setPosition(window.getSize().x/2,originY);
    }
    ///Don't draw the bounding boxes for now. Later we can make a debug keybind for it - something like ctrl + f10
    //window.draw(debug_text_bounding_box);

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
void MenuButton::UpdateText(){
    if (endString.size()){
        t_buttonText.setString(Func::ConvertToUtf8String(parentList->config->strRepo.GetUnicodeString(buttonText))+" "+Func::ConvertToUtf8String(endString));
    } else {
        t_buttonText.setString(Func::ConvertToUtf8String(parentList->config->strRepo.GetUnicodeString(buttonText)));
    }
    t_buttonText.setOrigin(t_buttonText.getGlobalBounds().width/2,t_buttonText.getGlobalBounds().height/2);
}
void MenuButton(){

}
MenuButton::~MenuButton()
{

}
