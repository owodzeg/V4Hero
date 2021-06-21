#ifndef NEWGAMEMENU_H
#define NEWGAMEMENU_H
#include "../Config.h"
#include "../Graphics/Menu.h"
#include "../Input/InputController.h"
#include "../Mission/MissionController.h"
#include "ButtonLists/NewGameMenuButtonList.h"
#include "NewGameNameEntryMenu.h"
#include "OptionsMenu.h"
#include "Patapolis.h"
#include <SFML/Graphics.hpp>

class V4Core;
class NewGameMenu : public Menu
{
public:
    sf::RectangleShape mm_bigBox;
    sf::RectangleShape mm_titleBox;

    sf::Font f_font;
    sf::Text t_title;
    sf::Text t_pressToContinue;

    // this is outside the loop
    int startAlpha;
    int endAlpha;
    sf::Time targetTime;
    sf::Clock timer;
    sf::RectangleShape fade;

    NewGameMenuButtonList buttonList;

    OptionsMenu optionsMenu;
    NewGameNameEntryMenu newGameNameEntryMenu;
    void Initialise(Config* thisConfig, V4Core* parent);

    void Update(sf::RenderWindow& window, float fps, InputController& inputCtrl, InputController& inputCtrlHeld);
    void EventFired(sf::Event event);
    void OnExit();
    void UpdateButtons();
    NewGameMenu();
    ~NewGameMenu();
};


#endif // NEWGAMEMENU_H
