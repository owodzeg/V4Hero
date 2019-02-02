#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>

class MainMenu
{
    public:
        sf::RectangleShape mm_bigBox;
        sf::RectangleShape mm_smallerBox;
        sf::RectangleShape mm_titleBox;


        sf::Font f_font;
        sf::Text t_title;
        sf::Text t_pressToContinue;


        Update(sf::RenderWindow &window, float fps);
        KeyPressedEvent(sf::Event event);
        MainMenu();
        ~MainMenu();

};


#endif // CAMERA_H
