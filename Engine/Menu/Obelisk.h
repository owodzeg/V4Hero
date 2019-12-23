#ifndef OBELISK_H
#define OBELISK_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/Menu.h"
class V4Core;
class PatapolisMenu;
class ObeliskMenu : public Menu
{
    public:

        sf::Font f_font;
        sf::Text t_titlemenu;
        sf::Text t_itemtitle;

        PatapolisMenu *parentMenu;
        Camera camera;

        void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent,PatapolisMenu *curParentMenu);
        void Update(sf::RenderWindow &window, float fps);
        void EventFired(sf::Event event);
        void OnExit();
        void UpdateButtons();
        ObeliskMenu();
        ~ObeliskMenu();



};


#endif // OBELISK_H
