
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
    int q = 1;

    ControlTips ctrlTips;

    PSprite mater_main;
    PSprite mater_selector;

    PSprite up_arrow_prompt;
    PSprite down_arrow_prompt;

    float mouseX = 0, mouseY = 0;

    struct SquadBox {
        ///data
        int amount = 0;
        int maxSize = 0;
        float y = 0.f;

        sf::String title;
        std::string squad_icon;
    };
    std::vector<SquadBox> squads;

    float highlight_x = 0;

    PataText altar_kaching;
    PataText mater_title;
    PataText squad_title;

    bool save_loaded = false;

    void Update();
    void DrawAsleepSquad(MaterOuterMenu::SquadBox& squad, int squad_alpha);
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
