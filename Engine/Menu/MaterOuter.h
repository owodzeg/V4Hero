
#ifndef MATEROUTER_H
#define MATEROUTER_H
#include "../Config.h"
#include "../Dialog/ControlTips.h"
#include "../Dialog/RoundedRect.h"
#include "../Graphics/Menu.h"
#include "../Input/InputController.h"
#include "../Pon/Squad.h"
#include <SFML/Graphics.hpp>

class V4Core;
class PatapolisMenu;
class MaterOuterMenu : public Menu
{
private:
    int squadpos = 0;
    int cursquad = 0;

public:
    float ratio_x, ratio_y;
    float res_ratio_x, res_ratio_y;
    int q = 1;

    sf::Font f_font;
    ControlTips ctrlTips;

    PSprite mater_main;
    PSprite mater_selector;

    PSprite up_arrow_prompt;
    PSprite down_arrow_prompt;

    float mouseX = 0, mouseY = 0;

    struct SquadBox {
        ///data
        int amount = 0;
        int maxSize;
        int y;

        sf::String title;
        std::string squad_icon;
    };
    std::vector<SquadBox> squads;

    float highlight_x = 0;

    PText altar_kaching;
    PText mater_title;
    PText squad_title;

    bool save_loaded = false;

    PatapolisMenu* parentMenu;

    void initialise(Config* thisConfig, V4Core* parent, PatapolisMenu* curParentMenu);
    void Update();
    void eventFired(sf::Event event);
    void DrawAsleepSquad(MaterOuterMenu::SquadBox& squad, int squad_alpha, sf::RenderWindow* window);
    std::vector<MaterOuterMenu::SquadBox*> GetSquadsCentered();
    void showCategory();
    void onExit();
    void showMater();
    void updateButtons();
    void MoveSquadPos(int spaces);
    MaterOuterMenu();
    ~MaterOuterMenu();
};


#endif // MATEROUTER_H
