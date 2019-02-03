#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <string>
#include <SFML/Graphics.hpp>

class MenuButton
{
    public:
        std::string buttonText;
        sf::Text t_buttonText;
        bool selected=false;
        float originX;
        float originY;

        void Update(sf::RenderWindow &window, float fps);
        void SetSelected(bool isSelected);

        MenuButton(std::string text,sf::Font *font,int fontSize, float y);
        ~MenuButton();

};


#endif // CAMERA_H
