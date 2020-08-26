#ifndef INTRODUCTIONMENU_H
#define INTRODUCTIONMENU_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Graphics/PSprite.h"
#include "../Graphics/PText.h"
#include "../Dialog/MessageCloud.h"
#include "../Input/InputController.h"

class IntroductionMenu : public Menu
{
    public:
        MessageCloud msgcloud;
        sf::Clock timeout,timeout2;

        Menu *parentMenu;
        void Initialise(Config *thisConfig, V4Core *parent, Menu *curParentMenu);
        void Update(sf::RenderWindow& window, float fps, InputController& inputCtrl);
        void EventFired(sf::Event event);
        IntroductionMenu();
        ~IntroductionMenu();
};

#endif // INTRODUCTIONMENU_H
