#ifndef MOUSECONTROLLER_H
#define MOUSECONTROLLER_H

#include <SFML/Graphics.hpp>

class MouseController {
    public:
    int x=0,y=0;
    bool lmb = false, rmb = false;
    MouseController();
    void parseEvents(sf::Event& event);
    sf::Vector2i getMousePos();
    bool getClick(int which);
};

#endif //MOUSECONTROLLER_H
