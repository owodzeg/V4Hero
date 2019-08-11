#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <string>
#include <SFML/Graphics.hpp>
class ButtonList;
class MenuButton
{
    public:
        std::wstring buttonText;
        std::wstring endString;
        sf::Text t_buttonText;
        bool selected=false;
        float originX;
        float originY;
        sf::RectangleShape debug_text_bounding_box;
        ButtonList *parentList;
        int buttonListIndex;
        void Update(sf::RenderWindow &window, float fps, sf::Vector2f *mousePos);
        void SetSelected(bool isSelected);
        void MouseUp(int xPos,int yPos);
        void UpdateText();
        void AddEndString(std::wstring text);
        std::wstring GetEndString(std::wstring text);

        MenuButton(std::wstring text,sf::Font *font,int fontSize, float y, ButtonList *p_list,int index);
        ~MenuButton();

};


#endif // CAMERA_H
