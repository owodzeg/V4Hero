#ifndef INTRODUCTIONMENU_H
#define INTRODUCTIONMENU_H
#include "../Config.h"
#include "../Dialog/MessageCloud.h"
#include "../Graphics/Menu.h"
#include "../Graphics/PSprite.h"
#include "../Graphics/PText.h"
#include "../Input/InputController.h"
#include <SFML/Graphics.hpp>

class IntroductionMenu : public Menu
{
public:
    MessageCloud msgcloud;
    sf::Clock timeout, timeout2;
    bool initialized = false;

    void Initialize();
    void Update();
    void EventFired(sf::Event event);
    IntroductionMenu();
    ~IntroductionMenu();
};

#endif // INTRODUCTIONMENU_H
