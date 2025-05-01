#ifndef ALTAR_H
#define ALTAR_H

#include "../Dialog/ControlTips.h"
#include "../Dialog/RoundedRect.h"
#include "../Graphics/PSprite.h"
#include "../Item/Item.h"
#include <SFML/Graphics.hpp>

class AltarMenu
{
public:
    float ratio_x, ratio_y;
    int q = 1;

    ControlTips ctrlTips;

    PSprite altar_main;

    float highlight_x = 0;

    struct InvBox {
        ///data
        Item* data;
        int amount = 0;
        bool highlight = false;

        ///display
        sf::RectangleShape r_outer;
        sf::RectangleShape r_inner;
        sf::RectangleShape r_highlight;
        PataText num, num_shadow;
        PSprite icon;
    };

    std::vector<InvBox> inventory_boxes;
    RoundedRect rr_title, rr_title_sh;
    RoundedRect rr_desc, rr_desc_sh;

    PataText altar_title;
    PataText altar_kaching;
    PataText altar_item_title, altar_item_category, altar_item_desc;

    int grid_sel_x = 0, grid_sel_y = 0, grid_offset_y = 0;
    sf::RectangleShape r_sel;

    bool save_loaded = false;
    bool is_active = false;
    bool initialized = false;

    void Update();
    void reloadInventory();
    void updateAltarDescriptions();
    AltarMenu();
    ~AltarMenu();
};


#endif // ALTARMENU_H
