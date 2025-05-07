#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Barracks.h"
#include "Altar.h"
#include <sstream>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"
#include "../StateManager.h"
#include "../Constants.h"

template<typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

Barracks::Barracks()
{
    SPDLOG_INFO("Initializing Barracks");

    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    int quality = CoreManager::getInstance().getConfig()->GetInt("textureQuality");

    switch (quality)
    {
        case 0: ///low
        {
            ratio_x = CoreManager::getInstance().getConfig()->GetInt("resX") / CANVAS_LOW_X;
            ratio_y = CoreManager::getInstance().getConfig()->GetInt("resY") / CANVAS_LOW_Y;
            break;
        }

        case 1: ///med
        {
            ratio_x = CoreManager::getInstance().getConfig()->GetInt("resX") / CANVAS_MED_X;
            ratio_y = CoreManager::getInstance().getConfig()->GetInt("resY") / CANVAS_MED_Y;
            break;
        }

        case 2: ///high
        {
            ratio_x = CoreManager::getInstance().getConfig()->GetInt("resX") / CANVAS_HIGH_X;
            ratio_y = CoreManager::getInstance().getConfig()->GetInt("resY") / CANVAS_HIGH_Y;
            break;
        }

        case 3: ///ultra
        {
            ratio_x = CoreManager::getInstance().getConfig()->GetInt("resX") / CANVAS_ULTRA_X;
            ratio_y = CoreManager::getInstance().getConfig()->GetInt("resY") / CANVAS_ULTRA_Y;
            break;
        }
    }

    patapon_y = 607*3;
    floor_y = 980*3;

    mm_selected_item_line.setSize(sf::Vector2f(135*3 * CoreManager::getInstance().getCore()->resRatio, 3*3 * CoreManager::getInstance().getCore()->resRatio));
    mm_selected_item_line.setFillColor(sf::Color::Red);

    t_title.defaultStyleSetFont(font);
    t_title.defaultStyleSetCharSize(171);
    t_title.defaultStyleSetColor(sf::Color::White);

    t_item_title.defaultStyleSetFont(font);
    t_item_title.defaultStyleSetCharSize(126);
    t_item_title.defaultStyleSetColor(sf::Color::Black);

    ///             ####   BARRACKS MENU BACKGROUND
    s_background.loadFromFile("resources/graphics/bg/barracks/barracks.png");

    ///         highlighted unit
    s_pon_highlight.loadFromFile("resources/graphics/ui/highlighted_pon.png");

    ///             ####   UNIT CLASS ICON
    class_icon.loadFromFile("resources/graphics/ui/yari_icon.png");
    class_icon.setOrigin(class_icon.getLocalBounds().size.x / 2, class_icon.getLocalBounds().size.y / 2);
    class_icon.setPosition(102*3, 98*3);

    ///             ####   UNIT ITEM ICON
    s_unit_icon.loadFromFile("resources/graphics/ui/unit_icon.png");
    s_unit_icon.setPosition(946*3, 82*3);

    unit_status.defaultStyleSetFont(font);
    unit_status.defaultStyleSetCharSize(66);
    unit_status.defaultStyleSetColor(sf::Color(239, 88, 98));
    unit_status.append(Func::GetStrFromKey("barracks_unit_status"));

    class_name.defaultStyleSetFont(font);
    class_name.defaultStyleSetCharSize(102);
    class_name.defaultStyleSetColor(sf::Color::Black);
    class_name.append(Func::GetStrFromKey("barracks_yaripon"));

    /// Stat text
    unit_stat_level_t.defaultStyleSetFont(font);
    unit_stat_level_t.defaultStyleSetCharSize(81);
    unit_stat_level_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_level_t.append(Func::GetStrFromKey("barracks_stat_level"));
    unit_stat_level_v.defaultStyleSetFont(font);
    unit_stat_level_v.defaultStyleSetCharSize(81);
    unit_stat_level_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_exp_t.defaultStyleSetFont(font);
    unit_stat_exp_t.defaultStyleSetCharSize(81);
    unit_stat_exp_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_exp_t.append(Func::GetStrFromKey("barracks_stat_exp"));
    unit_stat_exp_v.defaultStyleSetFont(font);
    unit_stat_exp_v.defaultStyleSetCharSize(81);
    unit_stat_exp_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_hp_t.defaultStyleSetFont(font);
    unit_stat_hp_t.defaultStyleSetCharSize(81);
    unit_stat_hp_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_hp_t.append(Func::GetStrFromKey("barracks_stat_hp"));
    unit_stat_hp_v.defaultStyleSetFont(font);
    unit_stat_hp_v.defaultStyleSetCharSize(81);
    unit_stat_hp_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_dmg_t.defaultStyleSetFont(font);
    unit_stat_dmg_t.defaultStyleSetCharSize(81);
    unit_stat_dmg_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_dmg_t.append(Func::GetStrFromKey("barracks_stat_damage"));
    unit_stat_dmg_v.defaultStyleSetFont(font);
    unit_stat_dmg_v.defaultStyleSetCharSize(81);
    unit_stat_dmg_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_atkspd_t.defaultStyleSetFont(font);
    unit_stat_atkspd_t.defaultStyleSetCharSize(81);
    unit_stat_atkspd_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_atkspd_t.append(Func::GetStrFromKey("barracks_stat_attackspeed"));
    unit_stat_atkspd_v.defaultStyleSetFont(font);
    unit_stat_atkspd_v.defaultStyleSetCharSize(81);
    unit_stat_atkspd_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_critc_t.defaultStyleSetFont(font);
    unit_stat_critc_t.defaultStyleSetCharSize(81);
    unit_stat_critc_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_critc_t.append(Func::GetStrFromKey("barracks_stat_critchance"));
    unit_stat_critc_v.defaultStyleSetFont(font);
    unit_stat_critc_v.defaultStyleSetCharSize(81);
    unit_stat_critc_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_kbc_t.defaultStyleSetFont(font);
    unit_stat_kbc_t.defaultStyleSetCharSize(81);
    unit_stat_kbc_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_kbc_t.append(Func::GetStrFromKey("barracks_stat_knockbackchance"));
    unit_stat_kbc_v.defaultStyleSetFont(font);
    unit_stat_kbc_v.defaultStyleSetCharSize(81);
    unit_stat_kbc_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_stgc_t.defaultStyleSetFont(font);
    unit_stat_stgc_t.defaultStyleSetCharSize(81);
    unit_stat_stgc_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_stgc_t.append(Func::GetStrFromKey("barracks_stat_staggerchance"));
    unit_stat_stgc_v.defaultStyleSetFont(font);
    unit_stat_stgc_v.defaultStyleSetCharSize(81);
    unit_stat_stgc_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_firec_t.defaultStyleSetFont(font);
    unit_stat_firec_t.defaultStyleSetCharSize(81);
    unit_stat_firec_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_firec_t.append(Func::GetStrFromKey("barracks_stat_burnchance"));
    unit_stat_firec_v.defaultStyleSetFont(font);
    unit_stat_firec_v.defaultStyleSetCharSize(81);
    unit_stat_firec_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_icec_t.defaultStyleSetFont(font);
    unit_stat_icec_t.defaultStyleSetCharSize(81);
    unit_stat_icec_t.defaultStyleSetColor(sf::Color::Black);
    unit_stat_icec_t.append(Func::GetStrFromKey("barracks_stat_freezechance"));
    unit_stat_icec_v.defaultStyleSetFont(font);
    unit_stat_icec_v.defaultStyleSetCharSize(81);
    unit_stat_icec_v.defaultStyleSetColor(sf::Color::Black);

    int equip_height = 50*3;

    ///             ####   WEAPON ITEM ICON
    s_weapon_icon.loadFromFile("resources/graphics/ui/sword_weapon_icon.png");
    s_weapon_icon.setPosition(946*3, s_unit_icon.getPosition().y + equip_height);

    ///             ####   WEAPON 2 (OTHER HAND) ITEM ICON
    s_weapon2_icon.loadFromFile("resources/graphics/ui/sword_weapon_icon.png");
    s_weapon2_icon.setPosition(946*3, s_weapon_icon.getPosition().y + equip_height);

    ///             ####   ARMOUR ITEM ICON
    s_armour_icon.loadFromFile("resources/graphics/ui/helm_icon.png");
    s_armour_icon.setPosition(946*3, s_weapon2_icon.getPosition().y + equip_height);

    ///             ####   MASK ITEM ICON
    s_mask_icon.loadFromFile("resources/graphics/ui/mask_icon.png");
    s_mask_icon.setPosition(946*3, s_armour_icon.getPosition().y + equip_height);

    /// unit + item name text

    t_unit_rarepon_name.append("{size 72}{color 0 0 0}");
    t_unit_rarepon_name.append(Func::GetStrFromKey("item_wooden_spear"));
    t_unit_rarepon_name.setGlobalOrigin(0, t_unit_rarepon_name.getGlobalBounds().size.y / 2);

    for (int i = 0; i < t_eq_names.size(); i++)
    {
        t_eq_names[i].append("{size 72}{color 0 0 0}");
        t_eq_names[i].append(Func::GetStrFromKey("item_wooden_spear"));
        t_eq_names[i].setGlobalOrigin(0, t_eq_names[i].getGlobalBounds().size.y / 2);
    }

    inv_box.loadFromFile("resources/graphics/ui/mini_inventory.png");

    item_title.append("{size 72}{color 0 0 0}");
    item_desc.append("{size 72}{color 0 0 0}");

    current_menu_position = -1;
    enabled_positons.push_back(true);
    enabled_positons.push_back(true);
    enabled_positons.push_back(false);
    enabled_positons.push_back(true);
    enabled_positons.push_back(false);

    quality_setting = quality;
    highlighted_pon.loadFromFile("resources/graphics/ui/highlighted_pon.png");

    //TO-DO: replace old pointers with new CoreManager pointers
    //applyEquipment();

    rr_main_sh.Create(1102*3, 222*3, 20*3, sf::Color(0, 0, 0, 96));
    rr_main_sh.x = 669*3;
    rr_main_sh.y = 189*3;
    rr_main_sh.setOrigin(sf::Vector2f((1100 + 40)*3 / 2, (220 + 40)*3 / 2));

    rr_uniticon_sh.Create(178*3, 10*3, 34*3, sf::Color(0, 0, 0, 96));
    rr_uniticon_sh.x = 187*3;
    rr_uniticon_sh.y = 97*3;
    rr_uniticon_sh.setOrigin(sf::Vector2f((176 + 68)*3 / 2, (8 + 68)*3 / 2));

    rr_unitstatus_sh.Create(177*3, 22*3, 25*3, sf::Color(0, 0, 0, 96));
    rr_unitstatus_sh.x = 1126*3;
    rr_unitstatus_sh.y = 63*3;
    rr_unitstatus_sh.setOrigin(sf::Vector2f((175 + 50)*3 / 2, (12 + 50)*3 / 2));

    rr_main.Create(1100*3, 220*3, 20*3);
    rr_main.x = 670*3;
    rr_main.y = 190*3;
    rr_main.setOrigin(sf::Vector2f((1100 + 40)*3 / 2, (220 + 40)*3 / 2));

    rr_uniticon.Create(176*3, 8*3, 34*3);
    rr_uniticon.x = 188*3;
    rr_uniticon.y = 98*3;
    rr_uniticon.setOrigin(sf::Vector2f((176 + 68)*3 / 2, (8 + 68)*3 / 2));

    rr_uniticon.Create(175*3, 20*3, 25*3);
    rr_uniticon.x = 1127*3;
    rr_uniticon.y = 64*3;
    rr_uniticon.setOrigin(sf::Vector2f((175 + 50)*3 / 2, (12 + 50)*3 / 2));

    //mm_inventory_background.setSize(sf::Vector2f(mm_inventory_background.getSize().x+(40*CoreManager::getInstance().getCore()->resRatio),mm_inventory_background.getSize().y+(40*CoreManager::getInstance().getCore()->resRatio)));

    // TODO: hardcoded yaripons. change when tatepons are implemented.
    auto yaripon_count = static_cast<int>(CoreManager::getInstance().getSaveReader()->ponReg.pons.size());

    for(int i=1; i<=yaripon_count; i++)
    {
        barracks_units.push_back(std::make_unique<AnimatedObject>());
        barracks_units.back().get()->LoadConfig("resources/units/unit/yaripon.zip");

        Pon* currentPon = CoreManager::getInstance().getSaveReader()->ponReg.GetPonByID(i-1);
        InventoryData::InventoryItem eq = CoreManager::getInstance().getSaveReader()->invData.items[currentPon->slots[0]];

        std::string wpn = eq.item->spritesheet+"/"+Func::num_padding(eq.item->spritesheet_id, 4);

        eq = CoreManager::getInstance().getSaveReader()->invData.items[currentPon->slots[1]];

        std::string hlm = eq.item->spritesheet+"/"+Func::num_padding(eq.item->spritesheet_id, 4);

        barracks_units.back().get()->loadExtra(wpn, "weapon");
        barracks_units.back().get()->loadExtra(hlm, "helm");

        float pon_width = 225.f / yaripon_count * 6.f;
        barracks_units.back().get()->setGlobalPosition(sf::Vector2f((1260 + (pon_width * (i-1))), 1815));
    }

    initialized = true;

    //TO-DO: is this needed?
    is_active = false;
}

int Barracks::countOccupied(std::vector<int> order_id)
{
    SPDLOG_DEBUG("Barracks::countOccupied({} {} {})", order_id[0], order_id[1], order_id[2]);
    int occ = 0;

    for (int i = 0; i < CoreManager::getInstance().getSaveReader()->ponReg.pons.size(); i++) // Go through every pon
    {
        for (int o = 0; o < CoreManager::getInstance().getSaveReader()->ponReg.pons[i].slots.size(); o++) // *through every equip slot of every pon
        {
            if (CoreManager::getInstance().getSaveReader()->ponReg.pons[i].slots[o] == CoreManager::getInstance().getSaveReader()->invData.getInvIDByItemID(order_id)) // if it matches the inv id (equivalent to is same item cause of the inv system)
            {
                occ++;
            }
        }
    }

    SPDLOG_DEBUG("Item is occupied {} times", occ);

    return occ;
}

void Barracks::loadInventory()
{
    inventory_boxes.clear();

    std::vector<InventoryData::InventoryItem> items_filtered; ///take only the items we want
    std::vector<int> items_invIDs;                            ///i dont remember if invid is accessible through inventoryitem

    for (int i = 0; i < CoreManager::getInstance().getSaveReader()->invData.items.size(); i++)
    {
        InventoryData::InventoryItem cur_inv_item = CoreManager::getInstance().getSaveReader()->invData.items[i];
        Item* cur_item = cur_inv_item.item;

        if (cur_item->order_id[0] == active_category)
        {
            if (active_category == 3) ///weapons, force spears
            {
                if (cur_item->order_id[1] == 0)
                {
                    items_filtered.push_back(CoreManager::getInstance().getSaveReader()->invData.items[i]);
                    items_invIDs.push_back(i);
                }
            }

            if (active_category == 4) ///armor, force helms
            {
                if (cur_item->order_id[1] == 1)
                {
                    items_filtered.push_back(CoreManager::getInstance().getSaveReader()->invData.items[i]);
                    items_invIDs.push_back(i);
                }
            }
        }
    }

    for (int i = 0; i < items_filtered.size(); i++)
    {
        InventoryData::InventoryItem cur_inv_item = items_filtered[i];
        Item* cur_item = cur_inv_item.item;
        int cur_count = cur_inv_item.item_count;
        int cur_occ_count = countOccupied(cur_item->order_id);

        InvBox cur_box;
        cur_box.data = cur_item;
        ///lol inv id shouldnt be based off filtered equipment
        //cur_box.inv_id = i;
        cur_box.inv_id = items_invIDs[i];
        cur_box.occ_amount = cur_occ_count;
        cur_box.amount = cur_count;
        if (cur_count <= cur_occ_count)
        {
            cur_box.highlight = true;
        } else
        {
            cur_box.highlight = false;
        }

        cur_box.r_outer.setSize(sf::Vector2f(210 * CoreManager::getInstance().getCore()->resRatio, 153 * CoreManager::getInstance().getCore()->resRatio));
        cur_box.r_outer.setFillColor(sf::Color(102, 102, 102, 255));

        cur_box.r_inner.setSize(sf::Vector2f(138 * CoreManager::getInstance().getCore()->resRatio, 138 * CoreManager::getInstance().getCore()->resRatio));
        cur_box.r_inner.setFillColor(sf::Color(183, 183, 183, 255));

        switch (cur_item->order_id[0]) // Is there a better way than nested switches here? (look: weapons -> spears or swords will be necessary)
        {
            case 0: // Key Items
            {
                cur_box.r_inner.setFillColor(sf::Color(183, 183, 183, 255));

                ///look up item's icon
                cur_box.icon.loadFromFile("resources/graphics/ui/altar/materials/" + Func::num_padding(cur_item->spritesheet_id, 4) + ".png");
                cur_box.icon.setOrigin(cur_box.icon.getLocalBounds().size.x / 2, cur_box.icon.getLocalBounds().size.y / 2);

                break;
            }

            case 1: // Materials
            {
                cur_box.r_inner.setFillColor(sf::Color(146, 173, 217, 255));

                ///look up item's icon
                cur_box.icon.loadFromFile("resources/graphics/ui/altar/materials/" + Func::num_padding(cur_item->spritesheet_id, 4) + ".png");
                cur_box.icon.setOrigin(cur_box.icon.getLocalBounds().size.x / 2, cur_box.icon.getLocalBounds().size.y / 2);

                break;
            }

            case 3: // Weapons
            {
                cur_box.r_inner.setFillColor(sf::Color(199, 221, 167, 255));

                ///look up item's icon
                cur_box.icon.loadFromFile("resources/graphics/ui/altar/equip/spear_1.png");
                cur_box.icon.setOrigin(cur_box.icon.getLocalBounds().size.x / 2, cur_box.icon.getLocalBounds().size.y / 2);

                break;
            }

            case 4: // Armour
            {
                cur_box.r_inner.setFillColor(sf::Color(199, 221, 167, 255));

                ///look up item's icon
                cur_box.icon.loadFromFile("resources/graphics/ui/altar/equip/helm_1.png");
                cur_box.icon.setOrigin(cur_box.icon.getLocalBounds().size.x / 2, cur_box.icon.getLocalBounds().size.y / 2);

                break;
            }
        }

        auto strRepo = CoreManager::getInstance().getStrRepo();
        std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

        cur_box.num.default_style.char_size = 60;
        cur_box.num.default_style.c_red = 255;
        cur_box.num.default_style.c_green = 255;
        cur_box.num.default_style.c_blue = 255;
        cur_box.num.reset();
        cur_box.num.append("001");
        cur_box.num_shadow.default_style.char_size = 60;
        cur_box.num_shadow.default_style.c_red = 136;
        cur_box.num_shadow.default_style.c_green = 136;
        cur_box.num_shadow.default_style.c_blue = 36;
        cur_box.num_shadow.reset();
        cur_box.num_shadow.append("001");

        inventory_boxes.push_back(cur_box);
        inventory_boxes[i].num.reset();
        inventory_boxes[i].num.append(Func::num_padding(inventory_boxes[i].amount, 3));
        inventory_boxes[i].num_shadow.reset();
        inventory_boxes[i].num_shadow.append(Func::num_padding(inventory_boxes[i].amount, 3));
    }

    if (inventory_boxes.size() > 0) // I'm absolutely sure this is broken.
    {
        std::sort(inventory_boxes.begin(), inventory_boxes.end(),
                  [](const InvBox& a, const InvBox& b) {
                      if (a.data->order_id[0] != b.data->order_id[0])
                      {
                          return a.data->order_id[0] < b.data->order_id[0];
                      }
                      if (a.data->order_id[1] != b.data->order_id[1])
                      {
                          return a.data->order_id[1] < b.data->order_id[1];
                      }
                      if (a.data->order_id.size() > 2 && b.data->order_id.size() > 2)
                      {
                          return a.data->order_id[2] < b.data->order_id[2];
                      } else
                      {
                          SPDLOG_ERROR("Inventory boxes sorting in barrack.cpp has reached a point it shouldn't have. The items may or may not appear in the correct order because of this.");
                          throw std::exception();
                      }
                  });
    }

    refreshStats();
    applyEquipment();
}

void Barracks::setInventoryPosition()
{
    int inv_id = -1;

    Pon* currentPon = CoreManager::getInstance().getSaveReader()->ponReg.GetPonByID(current_selected_pon);

    if (current_item_position == 1)
        inv_id = currentPon->slots[0];
    if (current_item_position == 3)
        inv_id = currentPon->slots[1];

    if (inv_id != -1)
    {
        Item* cur_item = CoreManager::getInstance().getSaveReader()->invData.items[inv_id].item;

        int invbox_id = -1;

        for (int i = 0; i < inventory_boxes.size(); i++)
        {
            if (inventory_boxes[i].data == cur_item)
                invbox_id = i;
        }

        if (invbox_id != -1)
        {
            grid_offset_y = static_cast<int>(ceil((invbox_id + 1) / 4) - 4);

            if (grid_offset_y <= 0)
                grid_offset_y = 0;

            grid_sel_x = invbox_id % 4;

            grid_sel_y = static_cast<int>(ceil(((invbox_id + 1) - (grid_offset_y * 4)) / 4.0) - 1);

            SPDLOG_TRACE("invbox_id: {} gridSelX: {} gridSelY: {} gridOffsetY: {}", invbox_id, grid_sel_x, grid_sel_y, grid_offset_y);
        } else
        {
            grid_sel_x = 0;
            grid_sel_y = 0;
            grid_offset_y = 0;
        }
    } else
    {
        grid_sel_x = 0;
        grid_sel_y = 0;
        grid_offset_y = 0;
    }

    refreshStats();
    applyEquipment();
    updatePreviewText();
}

void Barracks::applyEquipment()
{
    int yaripon_count = static_cast<int>(CoreManager::getInstance().getSaveReader()->ponReg.pons.size());

    for (int i = 1; i <= yaripon_count; i++)
    {
        Pon* currentPon = CoreManager::getInstance().getSaveReader()->ponReg.GetPonByID(i-1);
        InventoryData::InventoryItem eq = CoreManager::getInstance().getSaveReader()->invData.items[currentPon->slots[0]];

        std::string wpn = eq.item->spritesheet+"/"+Func::num_padding(eq.item->spritesheet_id, 4);

        eq = CoreManager::getInstance().getSaveReader()->invData.items[currentPon->slots[1]];

        std::string hlm = eq.item->spritesheet+"/"+Func::num_padding(eq.item->spritesheet_id, 4);

        barracks_units[i-1].get()->loadExtra(wpn, "weapon");
        barracks_units[i-1].get()->loadExtra(hlm, "helm");
    }
}

void Barracks::refreshStats()
{
    Pon* currentPon = CoreManager::getInstance().getSaveReader()->ponReg.GetPonByID(current_selected_pon);
    class_name.reset();

    switch (currentPon->pon_class)
    {
        case 1: {
            class_name.append(Func::GetStrFromKey("barracks_yaripon"));
            break;
        }
        case 2: {
            class_name.append(Func::GetStrFromKey("barracks_tatepon"));
            break;
        }
        case 3: {
            class_name.append(Func::GetStrFromKey("barracks_yumipon"));
            break;
        }
        case 4: {
            class_name.append(CoreManager::getInstance().getSaveReader()->kami_name);
            break;
        }
    }

    t_unit_rarepon_name.reset();
    t_unit_rarepon_name.append(Func::GetStrFromKey("rarepon_normal"));
    t_unit_rarepon_name.append(std::to_string(static_cast<int>(currentPon->pon_level)));

    for (int i = 0; i < currentPon->slots.size(); i++)
    {
        if (currentPon->slots[i] >= 0)
        {
            InventoryData::InventoryItem eq = CoreManager::getInstance().getSaveReader()->invData.items[currentPon->slots[i]];
            t_eq_names[i].reset();
            t_eq_names[i].append(Func::GetStrFromKey(eq.item->item_name));
            t_eq_names[i].setGlobalOrigin(0, 0);

            SPDLOG_TRACE("currentPon->slots[{}]: {} {}", i, currentPon->slots[i], eq.item->item_name);
        }
    }

    //s_unit_level.append(std::to_string(currentPon->pon_level));
    //s_unit_experience.append(std::to_string(currentPon->pon_exp));
    //s_unit_experience.append(std::to_string(currentPon->pon_exp));
    //s_unit_damage.append(std::to_string(currentPon->pon_min_dmg)+"/"+std::to_string(currentPon->pon_max_dmg));
    //s_unit_hp.append(std::to_string(currentPon->pon_hp));
    //s_unit_crit.append(std::to_string(currentPon->pon_crit));
    //s_unit_attack_speed.append(std::to_string(currentPon->pon_attack_speed));
    
    // added to reset text color after the preview -L
    unit_stat_hp_v.defaultStyleSetColor(sf::Color::Black);
    unit_stat_atkspd_v.defaultStyleSetColor(sf::Color::Black);
    unit_stat_dmg_v.defaultStyleSetColor(sf::Color::Black);

    unit_stat_level_v.reset();
    unit_stat_exp_v.reset();
    unit_stat_hp_v.reset();
    unit_stat_dmg_v.reset();
    unit_stat_atkspd_v.reset();
    unit_stat_critc_v.reset();
    unit_stat_kbc_v.reset();
    unit_stat_stgc_v.reset();
    unit_stat_firec_v.reset();
    unit_stat_icec_v.reset();

    unit_stat_level_v.append(std::to_string(static_cast<int>(currentPon->pon_level))); /// CHANGE THESE WHEN IMPLEMENTING STATUS EFFECTS ETC.
    unit_stat_exp_v.append(std::to_string(static_cast<int>(currentPon->pon_exp)) + "/1200");
    unit_stat_hp_v.append(std::to_string(static_cast<int>(currentPon->pon_hp)));
    unit_stat_dmg_v.append(std::to_string(static_cast<int>(currentPon->pon_min_dmg)) + "-" + std::to_string(static_cast<int>(currentPon->pon_max_dmg)));
    unit_stat_atkspd_v.append(to_string_with_precision(currentPon->pon_attack_speed, 2));
    unit_stat_critc_v.append("0%");
    unit_stat_kbc_v.append("0%");
    unit_stat_stgc_v.append("0%");
    unit_stat_firec_v.append("0%");
    unit_stat_icec_v.append("0%");

    /*if (inventoryGridXPos+inventoryGridYPos*numItemColumns<CoreManager::getInstance().getSaveReader()->invdata.ItemsByType(activeCategory).size())
    {
        Item* starting_item = CoreManager::getInstance().getSaveReader()->invdata.ItemsByType(activeCategory)[inventoryGridXPos+inventoryGridYPos*numItemColumns].item;

        t_itemtitle.append(starting_item->item_name)));
        t_itemtitle.setOrigin(t_itemtitle.getLocalBounds().size.x/2,t_itemtitle.getLocalBounds().size.y/2);
    }
    else
    {
        t_itemtitle.append("item_none")));
        t_itemtitle.setOrigin(t_itemtitle.getLocalBounds().size.x/2,t_itemtitle.getLocalBounds().size.y/2);
    }*/
}

std::string Barracks::getPreviewText(float stat, float pon_stat, float pon_base_stat, int digits)
{
    std::string equip_attr = Func::fnum_padding(stat, digits);
    float actual_attr = pon_base_stat + stat;
    std::string preview_pon_attr = Func::fnum_padding(actual_attr, digits);
    std::string preview_attr;
    if (stat != 0) 
    {
        if (pon_stat == actual_attr) 
        {
            preview_attr = preview_pon_attr;
            return preview_attr;
        } else if (pon_stat < actual_attr) 
        {
            preview_attr = preview_pon_attr + "(+" + equip_attr + ")";
            return preview_attr;
        } else if (pon_stat > actual_attr) 
        {
            preview_attr = preview_pon_attr + "(-" + equip_attr + ")";
            return preview_attr;
        } 
    } else
    {
        return Func::fnum_padding(pon_stat, digits);
    }

    return "error";
}

std::string Barracks::getPreviewText(float stat, float pon_stat, float pon_base_stat, float stat2, float pon_stat2, float pon_base_stat2, int digits)
{
    float actual_attr = pon_base_stat + stat;
    float actual_attr2 = pon_base_stat2 + stat2;
    std::string preview_pon_attr = Func::fnum_padding(actual_attr, digits);
    std::string preview_pon_attr2 = Func::fnum_padding(actual_attr2, digits);
    std::string preview_attr;
    if (stat != 0 && stat2 != 0)
    {
        if (pon_stat == actual_attr && pon_stat2 == actual_attr2)
        {
            preview_attr = preview_pon_attr + "-" + preview_pon_attr2;
            return preview_attr;
        } else if ((stat2 - stat) < (pon_stat2 - pon_stat))
        {
            preview_attr = preview_pon_attr + "-" + preview_pon_attr2 + ("(-)");
            return preview_attr;
        } else
        {
            preview_attr = preview_pon_attr + "-" + preview_pon_attr2 + ("(+)");
            return preview_attr;
        }
    } else
    {
        return Func::fnum_padding(pon_stat, digits) + "-" + Func::fnum_padding(pon_stat2, digits);
    }
}

sf::Color Barracks::getPreviewColorText(float stat, float pon_stat, float pon_base_stat, bool invert_color, int digits)
{
    if (stat != 0)
    {
        if (pon_stat == (pon_base_stat + stat))
            return sf::Color::Black;

        if (pon_stat < (pon_base_stat + stat))
        {
            if (invert_color)
                return sf::Color::Red;
            return sf::Color::Blue;
        }

        if (pon_stat > (pon_base_stat + stat))
        {
            if (invert_color)
                return sf::Color::Blue;
            return sf::Color::Red;
        }
    }

    return sf::Color::Black;
}

sf::Color Barracks::getPreviewColorText(float stat, float pon_stat, float pon_base_stat, float stat2, float pon_stat2, float pon_base_stat2, int digits)
{
    if (stat != 0 && stat2 != 0)
    {
        if (pon_stat == (pon_base_stat + stat) && pon_stat2 == (pon_base_stat2 + stat2))
            return sf::Color::Black;
        if ((stat2 - stat) < (pon_stat2 - pon_stat))
            return sf::Color::Red;

        return sf::Color::Blue;
    }

    return sf::Color::Black;
}

void Barracks::updatePreviewText()
{
    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    int invbox_id = ((grid_offset_y + grid_sel_y) * 4) + grid_sel_x;
    Pon* currentPon = CoreManager::getInstance().getSaveReader()->ponReg.GetPonByID(current_selected_pon);
    if (inventory_boxes.size() > 0)
    {
        if (invbox_id < inventory_boxes.size())
        {
            item_title.reset();
            item_desc.reset();

            item_title.append(Func::GetStrFromKey(inventory_boxes[invbox_id].data->item_name));
            item_desc.append(Func::ConvertToUtf8String(Func::wrap_text(Func::GetStrFromKey(inventory_boxes[invbox_id].data->item_description), 330*3, font, 66)));
            //preview stats -L
            unit_stat_hp_v.reset();
            unit_stat_hp_v.append(getPreviewText(inventory_boxes[invbox_id].data->equip->hp, currentPon->pon_hp, currentPon->pon_base_hp, 0));
            unit_stat_hp_v.defaultStyleSetColor(getPreviewColorText(inventory_boxes[invbox_id].data->equip->hp, currentPon->pon_hp, currentPon->pon_base_hp, false));

            unit_stat_atkspd_v.reset();
            unit_stat_atkspd_v.append(getPreviewText(inventory_boxes[invbox_id].data->equip->attack_speed, currentPon->pon_attack_speed, currentPon->pon_base_attack_speed, 2)); //the only one that gives problems with the preview. -L
            unit_stat_atkspd_v.defaultStyleSetColor(getPreviewColorText(inventory_boxes[invbox_id].data->equip->attack_speed, currentPon->pon_attack_speed, currentPon->pon_base_attack_speed, true));

            unit_stat_dmg_v.reset();
            unit_stat_dmg_v.append(getPreviewText(inventory_boxes[invbox_id].data->equip->min_dmg, currentPon->pon_min_dmg, currentPon->pon_base_min_dmg, inventory_boxes[invbox_id].data->equip->max_dmg, currentPon->pon_max_dmg, currentPon->pon_base_max_dmg, 0));
            unit_stat_dmg_v.defaultStyleSetColor(getPreviewColorText(inventory_boxes[invbox_id].data->equip->min_dmg, currentPon->pon_min_dmg, currentPon->pon_base_min_dmg, inventory_boxes[invbox_id].data->equip->max_dmg, currentPon->pon_max_dmg, currentPon->pon_base_max_dmg));
        } else
        {
            unit_stat_hp_v.defaultStyleSetColor(sf::Color::Black);
            unit_stat_atkspd_v.defaultStyleSetColor(sf::Color::Black);
            unit_stat_dmg_v.defaultStyleSetColor(sf::Color::Black);

            unit_stat_hp_v.reset();
            unit_stat_atkspd_v.reset();
            unit_stat_dmg_v.reset();

            item_title.reset();
            item_desc.reset();
            item_title.append("");
            item_desc.append("");
        }
    } else
    {
        unit_stat_hp_v.defaultStyleSetColor(sf::Color::Black);
        unit_stat_atkspd_v.defaultStyleSetColor(sf::Color::Black);
        unit_stat_dmg_v.defaultStyleSetColor(sf::Color::Black);

        unit_stat_hp_v.reset();
        unit_stat_atkspd_v.reset();
        unit_stat_dmg_v.reset();

        item_title.reset();
        item_desc.reset();

        item_title.append("");
        item_desc.append("");
    }
}

void Barracks::Update()
{
    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    auto lastView = window->getView();
    window->setView(window->getDefaultView());

    rs_cover.setSize(sf::Vector2f(1280*3 * CoreManager::getInstance().getCore()->resRatio, 720*3 * CoreManager::getInstance().getCore()->resRatio));
    rs_cover.setFillColor(sf::Color::Black);
    rs_cover.setPosition({0, 0});
    window->draw(rs_cover);

    s_background.setPosition(0, 0);
    s_background.draw();

    int yaripon_count = static_cast<int>(CoreManager::getInstance().getSaveReader()->ponReg.pons.size());

    int highlight_width = 225*3;
    int pon_width = 75 * 3 / yaripon_count * 6;

    for(auto& pon : barracks_units)
    {
        pon->setAnimation("idle_armed");
        pon->Draw();
    }

    highlighted_pon.setPosition((388*3 + (static_cast<float>(pon_width) * static_cast<float>(current_selected_pon))), 530*3);
    highlighted_pon.draw();

    s_pon_highlight.setPosition(static_cast<float>(highlight_width) * 2*3, 675*3);
    s_pon_highlight.draw();


    rr_main_sh.Draw();
    rr_uniticon_sh.Draw();
    rr_unitstatus_sh.Draw();
    rr_main.Draw();
    rr_uniticon.Draw();
    rr_uniticon.Draw();

    class_icon.draw();
    
    unit_status.reset();
    unit_status.append(Func::GetStrFromKey("barracks_unit_status"));
    unit_status.append(" " + std::to_string(current_selected_pon + 1) + "/6");
    unit_status.setGlobalPosition(1048*3, 38*3);

    Pon* currentPon = new Pon;
    currentPon = CoreManager::getInstance().getSaveReader()->ponReg.GetPonByID(current_selected_pon);
    class_name.reset();

    switch (currentPon->pon_class)
    {
        case 1: {
            class_name.append(Func::GetStrFromKey("barracks_yaripon"));
            break;
        }
        case 2: {
            class_name.append(Func::GetStrFromKey("barracks_tatepon"));
            break;
        }
    }
    class_name.setGlobalPosition(136*3, 74*3);

    unit_status.draw();
    class_name.draw();

    ///stat text
    unit_stat_level_t.setGlobalOrigin(0, unit_stat_level_t.getGlobalBounds().size.y / 2);
    unit_stat_exp_t.setGlobalOrigin(0, unit_stat_exp_t.getGlobalBounds().size.y / 2);
    unit_stat_hp_t.setGlobalOrigin(0, unit_stat_hp_t.getGlobalBounds().size.y / 2);
    unit_stat_dmg_t.setGlobalOrigin(0, unit_stat_dmg_t.getGlobalBounds().size.y / 2);
    unit_stat_atkspd_t.setGlobalOrigin(0, unit_stat_atkspd_t.getGlobalBounds().size.y / 2);
    unit_stat_critc_t.setGlobalOrigin(0, unit_stat_critc_t.getGlobalBounds().size.y / 2);
    unit_stat_kbc_t.setGlobalOrigin(0, unit_stat_kbc_t.getGlobalBounds().size.y / 2);
    unit_stat_stgc_t.setGlobalOrigin(0, unit_stat_stgc_t.getGlobalBounds().size.y / 2);
    unit_stat_firec_t.setGlobalOrigin(0, unit_stat_firec_t.getGlobalBounds().size.y / 2);
    unit_stat_icec_t.setGlobalOrigin(0, unit_stat_icec_t.getGlobalBounds().size.y / 2);

    unit_stat_level_v.setGlobalOrigin(unit_stat_level_v.getGlobalBounds().size.x, unit_stat_level_v.getGlobalBounds().size.y / 2);
    unit_stat_exp_v.setGlobalOrigin(unit_stat_exp_v.getGlobalBounds().size.x, unit_stat_exp_v.getGlobalBounds().size.y / 2);
    unit_stat_hp_v.setGlobalOrigin(unit_stat_hp_v.getGlobalBounds().size.x, unit_stat_hp_v.getGlobalBounds().size.y / 2);
    unit_stat_dmg_v.setGlobalOrigin(unit_stat_dmg_v.getGlobalBounds().size.x, unit_stat_dmg_v.getGlobalBounds().size.y / 2);
    unit_stat_atkspd_v.setGlobalOrigin(unit_stat_atkspd_v.getGlobalBounds().size.x, unit_stat_atkspd_v.getGlobalBounds().size.y / 2);
    unit_stat_critc_v.setGlobalOrigin(unit_stat_critc_v.getGlobalBounds().size.x, unit_stat_critc_v.getGlobalBounds().size.y / 2);
    unit_stat_kbc_v.setGlobalOrigin(unit_stat_kbc_v.getGlobalBounds().size.x, unit_stat_kbc_v.getGlobalBounds().size.y / 2);
    unit_stat_stgc_v.setGlobalOrigin(unit_stat_stgc_v.getGlobalBounds().size.x, unit_stat_stgc_v.getGlobalBounds().size.y / 2);
    unit_stat_firec_v.setGlobalOrigin(unit_stat_firec_v.getGlobalBounds().size.x, unit_stat_firec_v.getGlobalBounds().size.y / 2);
    unit_stat_icec_v.setGlobalOrigin(unit_stat_icec_v.getGlobalBounds().size.x, unit_stat_icec_v.getGlobalBounds().size.y / 2);

    unit_stat_level_t.setGlobalPosition(136*3, 146*3);
    unit_stat_level_v.setGlobalPosition(136*3 + 370*3, 146*3);
    unit_stat_exp_t.setGlobalPosition(136*3, 180*3);
    unit_stat_exp_v.setGlobalPosition(136*3 + 370*3, 180*3);
    unit_stat_hp_t.setGlobalPosition(136*3, 214*3);
    unit_stat_hp_v.setGlobalPosition(136*3 + 370*3, 214*3);
    unit_stat_dmg_t.setGlobalPosition(136*3, 248*3);
    unit_stat_dmg_v.setGlobalPosition(136*3 + 370*3, 248*3);
    unit_stat_atkspd_t.setGlobalPosition(136*3, 282*3);
    unit_stat_atkspd_v.setGlobalPosition(136*3 + 370*3, 282*3);

    unit_stat_critc_t.setGlobalPosition(540*3, 146*3);
    unit_stat_critc_v.setGlobalPosition(540*3 + 370*3, 146*3);
    unit_stat_kbc_t.setGlobalPosition(540*3, 180*3);
    unit_stat_kbc_v.setGlobalPosition(540*3 + 370*3, 180*3);
    unit_stat_stgc_t.setGlobalPosition(540*3, 214*3);
    unit_stat_stgc_v.setGlobalPosition(540*3 + 370*3, 214*3);
    unit_stat_firec_t.setGlobalPosition(540*3, 248*3);
    unit_stat_firec_v.setGlobalPosition(540*3 + 370*3, 248*3);
    unit_stat_icec_t.setGlobalPosition(540*3, 282*3);
    unit_stat_icec_v.setGlobalPosition(540*3 + 370*3, 282*3);

    unit_stat_level_t.draw();
    unit_stat_level_v.draw();
    unit_stat_exp_t.draw();
    unit_stat_exp_v.draw();
    unit_stat_hp_t.draw();
    unit_stat_hp_v.draw();
    unit_stat_dmg_t.draw();
    unit_stat_dmg_v.draw();
    unit_stat_atkspd_t.draw();
    unit_stat_atkspd_v.draw();

    unit_stat_critc_t.draw();
    unit_stat_critc_v.draw();
    unit_stat_kbc_t.draw();
    unit_stat_kbc_v.draw();
    unit_stat_stgc_t.draw();
    unit_stat_stgc_v.draw();
    unit_stat_firec_t.draw();
    unit_stat_firec_v.draw();
    unit_stat_icec_t.draw();
    unit_stat_icec_v.draw();


    /// equipped item + unit name text
    int equip_height = 53*3;

    if (enabled_positons[0]) /// Rarepon
    {
        t_unit_rarepon_name.setGlobalOrigin(0, 0);
        t_unit_rarepon_name.setGlobalPosition(s_unit_icon.getPosition().x + equip_height, s_unit_icon.getPosition().y + 8*3);
        s_unit_icon.setPosition(s_unit_icon.getPosition().x, s_unit_icon.getPosition().y);
        s_unit_icon.draw();
        t_unit_rarepon_name.draw();
    }
    if (enabled_positons[1]) /// wep1
    {
        t_slot_1_name.setGlobalPosition(s_unit_icon.getPosition().x + equip_height, s_weapon_icon.getPosition().y + 8*3);
        s_weapon_icon.setPosition(s_unit_icon.getPosition().x, s_weapon_icon.getPosition().y);
        s_weapon_icon.draw();
        t_slot_1_name.draw();

        t_eq_names[0].setGlobalPosition(s_unit_icon.getPosition().x + equip_height, s_weapon_icon.getPosition().y + 8*3);
        t_eq_names[0].draw();
    }

    if (enabled_positons[2]) /// wep2
    {
        t_slot_2_name.setGlobalPosition(s_unit_icon.getPosition().x + equip_height, s_weapon2_icon.getPosition().y + 8*3);
        s_weapon2_icon.setPosition(s_unit_icon.getPosition().x, s_weapon2_icon.getPosition().y);
        s_weapon2_icon.draw();
        t_slot_2_name.draw();

        //t_eq_names[1].setPosition(s_unit_icon.getPosition().x+equip_height,s_weapon2_icon.getPosition().y+8);
        //t_eq_names[1].draw();
    }

    if (enabled_positons[3]) /// armour
    {
        t_slot_3_name.setGlobalPosition(s_unit_icon.getPosition().x + equip_height, s_armour_icon.getPosition().y + 8*3);
        s_armour_icon.setPosition(s_unit_icon.getPosition().x, s_armour_icon.getPosition().y);
        s_armour_icon.draw();
        t_slot_3_name.draw();

        /// //////////////////// ///
        /// hardcoded solution
        /// i didnt want to bother with right slot positions compared to t_eq_names
        /// slot[1] in ponregistry is armor
        /// armor is slot 2 (third) in equipment, not second
        /// //////////////////// ///
        t_eq_names[1].setGlobalPosition(s_unit_icon.getPosition().x + equip_height, s_armour_icon.getPosition().y + 8*3);
        t_eq_names[1].draw();
    }

    if (enabled_positons[4]) /// mask
    {
        t_slot_4_name.setGlobalPosition(s_unit_icon.getPosition().x + equip_height, s_armour_icon.getPosition().y + 8*3);
        s_armour_icon.setPosition(s_unit_icon.getPosition().x, s_armour_icon.getPosition().y);
        s_armour_icon.draw();
        t_slot_4_name.draw();

        t_eq_names[3].setGlobalPosition(s_unit_icon.getPosition().x + equip_height, s_armour_icon.getPosition().y + 8*3);
        t_eq_names[3].draw();
    }

    item_line_flash += 8.0f / fps;

    int bar_size = 266*3;

    mm_selected_item_line.setSize(sf::Vector2f(bar_size*3 * CoreManager::getInstance().getCore()->resRatio, 9 * CoreManager::getInstance().getCore()->resRatio));
    mm_selected_item_line.setFillColor(sf::Color(239, 88, 98, static_cast<uint8_t>(128 + (sin(item_line_flash) * 128))));
    mm_selected_item_line.setPosition(sf::Vector2f((s_unit_icon.getPosition().x) * CoreManager::getInstance().getCore()->resRatio, ((s_unit_icon.getPosition().y + current_item_position * 50 * 3) + 47 * 3) * CoreManager::getInstance().getCore()->resRatio));

    ctrlTips.x = 0;
    ctrlTips.y = (2160 - ctrlTips.ySize);
    ctrlTips.draw();

    if (menu_mode)
    {
        inv_box.setPosition(40*3, 366*3);
        inv_box.draw();

        for (int i = 0; i < 16; i++)
        {
            if (grid_offset_y * 4 + i < inventory_boxes.size())
            {
                if (inventory_boxes.size() > 0)
                {
                    int cur_item = grid_offset_y * 4 + i;

                    int grid_x = i % 4;
                    int grid_y = static_cast<int>(floor(i / 4));

                    float xpos = 46*3 + (static_cast<float>(grid_x) * 77*3);
                    float ypos = 37*3 + (static_cast<float>(grid_y) * 54*3);

                    inventory_boxes[cur_item].r_outer.setPosition(sf::Vector2f((120 + xpos) * CoreManager::getInstance().getCore()->resRatio, (1098 + ypos) * CoreManager::getInstance().getCore()->resRatio));
                    inventory_boxes[cur_item].r_inner.setPosition(sf::Vector2f((120 + xpos + 7.5f) * CoreManager::getInstance().getCore()->resRatio, (1098 + ypos + 7.5f) * CoreManager::getInstance().getCore()->resRatio));
                    window->draw(inventory_boxes[cur_item].r_outer);
                    window->draw(inventory_boxes[cur_item].r_inner);

                    //inventory_boxes[i].num.setOrigin(inventory_boxes[i].num.getLocalBounds().size.x,inventory_boxes[i].num.getLocalBounds().size.y);
                    //inventory_boxes[i].num_shadow.setOrigin(inventory_boxes[i].num_shadow.getLocalBounds().size.x,inventory_boxes[i].num_shadow.getLocalBounds().size.y);

                    inventory_boxes[cur_item].icon.setScale(0.64f, 0.64f);

                    if ((inventory_boxes[cur_item].data->item_category == "key_items") || (inventory_boxes[cur_item].data->item_category == "consumables")) ///Bound to break
                        inventory_boxes[cur_item].icon.setScale(0.41f, 0.41f);

                    inventory_boxes[cur_item].icon.setPosition(193.5f + xpos, 1170.5f + ypos);
                    inventory_boxes[cur_item].icon.draw();

                    inventory_boxes[cur_item].num.setGlobalPosition(225 + xpos, 1179 + ypos);
                    inventory_boxes[cur_item].num_shadow.setGlobalPosition(228 + xpos, 1185 + ypos);

                    inventory_boxes[cur_item].num_shadow.draw();
                    inventory_boxes[cur_item].num.draw();

                    if (inventory_boxes[cur_item].highlight)
                    {
                        inventory_boxes[cur_item].r_highlight.setSize(sf::Vector2f(210 * CoreManager::getInstance().getCore()->resRatio, 153 * CoreManager::getInstance().getCore()->resRatio));
                        inventory_boxes[cur_item].r_highlight.setPosition(sf::Vector2f((120 + xpos) * CoreManager::getInstance().getCore()->resRatio, (1098 + ypos) * CoreManager::getInstance().getCore()->resRatio
                    ));
                        inventory_boxes[cur_item].r_highlight.setFillColor(sf::Color(0, 0, 0, 192));
                        window->draw(inventory_boxes[cur_item].r_highlight);
                    }
                }
            } else
            {
                InvBox tmp_inv;

                int grid_x = i % 4;
                int grid_y = static_cast<int>(floor(i / 4));

                float xpos = 46 * 3 + (static_cast<float>(grid_x) * 77 * 3);
                float ypos = 37 * 3 + (static_cast<float>(grid_y) * 54 * 3);

                tmp_inv.r_outer.setSize(sf::Vector2f(210 * CoreManager::getInstance().getCore()->resRatio, 153 * CoreManager::getInstance().getCore()->resRatio));
                tmp_inv.r_outer.setFillColor(sf::Color(102, 102, 102, 255));

                tmp_inv.r_inner.setSize(sf::Vector2f(138 * CoreManager::getInstance().getCore()->resRatio, 138 * CoreManager::getInstance().getCore()->resRatio));
                tmp_inv.r_inner.setFillColor(sf::Color(183, 183, 183, 255));

                tmp_inv.r_outer.setPosition(sf::Vector2f((120 + xpos) * CoreManager::getInstance().getCore()->resRatio, (1098 + ypos) * CoreManager::getInstance().getCore()->resRatio
            ));
                tmp_inv.r_inner.setPosition(sf::Vector2f((120 + xpos + 7.5f) * CoreManager::getInstance().getCore()->resRatio, (1098 + ypos + 7.5f) * CoreManager::getInstance().getCore()->resRatio));
                window->draw(tmp_inv.r_outer);
                window->draw(tmp_inv.r_inner);

                tmp_inv.r_highlight.setSize(sf::Vector2f(210 * CoreManager::getInstance().getCore()->resRatio, 153 * CoreManager::getInstance().getCore()->resRatio));
                tmp_inv.r_highlight.setPosition(sf::Vector2f((120 + xpos) * CoreManager::getInstance().getCore()->resRatio, (1098 + ypos) * CoreManager::getInstance().getCore()->resRatio
            ));
                tmp_inv.r_highlight.setFillColor(sf::Color(0, 0, 0, 192));
                window->draw(tmp_inv.r_highlight);
            }
        }

        r_sel.setSize(sf::Vector2f(210 * CoreManager::getInstance().getCore()->resRatio, 153 * CoreManager::getInstance().getCore()->resRatio));
        r_sel.setFillColor(sf::Color::Transparent);
        r_sel.setOutlineThickness(2);
        r_sel.setOutlineColor(sf::Color(255, 0, 32, 255));
        r_sel.setPosition(sf::Vector2f((258 + (grid_sel_x * 231)) * CoreManager::getInstance().getCore()->resRatio, (1199 + (grid_sel_y * 162)) * CoreManager::getInstance().getCore()->resRatio));

        window->draw(r_sel);

        rr_itempreview_sh.Create(362*3, 262*3, 20*3, sf::Color(0, 0, 0, 96));
        rr_itempreview_sh.x = 649*3;
        rr_itempreview_sh.y = 489*3;
        rr_itempreview_sh.setOrigin(sf::Vector2f((360 + 40)*3 / 2, (220 + 40)*3 / 2));
        rr_itempreview_sh.Draw();

        rr_itempreview.Create(360*3, 260*3, 20*3);
        rr_itempreview.x = 650*3;
        rr_itempreview.y = 490*3;
        rr_itempreview.setOrigin(sf::Vector2f((360 + 40)*3 / 2, (220 + 40)*3 / 2));
        rr_itempreview.Draw();

        item_title.setGlobalOrigin(item_title.getGlobalBounds().size.x / 2, item_title.getGlobalBounds().size.y / 2);
        item_title.setGlobalPosition(650*3, 390*3);
        item_title.draw();

        item_desc.setGlobalPosition(480*3, 440*3);
        item_desc.draw();
    }

    window->draw(mm_selected_item_line);

    screenFade.draw();

    window->setView(lastView);

    if ((CoreManager::getInstance().getDialogHandler()->dialogboxes.size() <= 0) && (screenFade.checkFinished()))
    {
        if (!menu_mode)
        {
            /*
            * Only the ones that are *true* will appear on the selection list and should be selectable.
            *
            enabled_positons.push_back(true);  [0]
            enabled_positons.push_back(true);  [1]
            enabled_positons.push_back(false); [2]
            enabled_positons.push_back(true);  [3]
            enabled_positons.push_back(false); [4]
            */

            if (inputCtrl->isKeyPressed(Input::Keys::UP))
            {
                /*current_item_position -=1;
                if (current_item_position<0){
                    current_item_position = enabled_positons.size()-1;
                }
                while(!enabled_positons[current_item_position]){
                    current_item_position -=1;
                }*/

                SPDLOG_DEBUG("Current item position: {}", current_item_position);

                if (current_item_position >= 0)
                {
                    bool found = false;

                    while (!found)
                    {
                        current_item_position--;

                        if (current_item_position >= 0)
                        {
                            if (enabled_positons[current_item_position])
                            {
                                found = true;
                                SPDLOG_DEBUG("Going to current_item_position={}", current_item_position);
                                break;
                            }
                        } else
                        {
                            ///Go back to bottom
                            current_item_position = static_cast<int>(enabled_positons.size()) - 1;
                        }
                    }
                }
            }

            if (inputCtrl->isKeyPressed(Input::Keys::DOWN))
            {
                /*if(current_item_position < 4)
                {
                    current_item_position += 1;

                    if (current_item_position > enabled_positons.size() - 1)
                    {
                        current_item_position = 0;
                    }

                    while (!enabled_positons[current_item_position])
                    {
                        current_item_position += 1;
                    }

                    if (current_item_position > enabled_positons.size() - 1)
                    {
                        current_item_position = 0;
                    }
                }
                else
                {
                    current_item_position = 0;
                }*/

                SPDLOG_DEBUG("Current item position: {}", current_item_position);

                // if (4 < 4) ok
                if (current_item_position < enabled_positons.size() - 1)
                {
                    bool found = false;

                    while (!found)
                    {
                        current_item_position++;

                        if (current_item_position <= enabled_positons.size() - 1)
                        {
                            if (enabled_positons[current_item_position])
                            {
                                found = true;
                                SPDLOG_DEBUG("Going to current_item_position={}", current_item_position);
                                break;
                            }
                        } else
                        {
                            ///Go back to top
                            current_item_position = -1;
                        }
                    }
                }
            }

            if (inputCtrl->isKeyPressed(Input::Keys::LEFT))
            {
                if (current_selected_pon > 0)
                    current_selected_pon--;

                refreshStats();
            }

            if (inputCtrl->isKeyPressed(Input::Keys::RIGHT))
            {
                if (current_selected_pon < CoreManager::getInstance().getSaveReader()->ponReg.pons.size() - 1)
                    current_selected_pon++;

                refreshStats();
            }
        } else
        {
            if (inputCtrl->isKeyPressed(Input::Keys::LEFT))
            {
                grid_sel_x--;

                if (grid_sel_x < 0)
                    grid_sel_x = 3;

                updatePreviewText();
            }
            if (inputCtrl->isKeyPressed(Input::Keys::RIGHT))
            {
                grid_sel_x++;

                if (grid_sel_x > 3)
                    grid_sel_x = 0;

                updatePreviewText();
            }
            if (inputCtrl->isKeyPressed(Input::Keys::UP))
            {
                grid_sel_y--;

                if (grid_sel_y < 0)
                {
                    if (grid_offset_y > 0)
                    {
                        grid_offset_y--;
                        grid_sel_y = 0;
                    } else
                    {
                        grid_offset_y = static_cast<int>(ceil(inventory_boxes.size() / 4.0) - 4);

                        if (grid_offset_y < 0)
                            grid_offset_y = 0;

                        grid_sel_y = 3;
                    }
                }

                updatePreviewText();
            }
            if (inputCtrl->isKeyPressed(Input::Keys::DOWN))
            {
                grid_sel_y++;

                if (grid_sel_y > 3)
                {
                    if (inventory_boxes.size() > (4 + grid_offset_y) * 4)
                    {
                        grid_offset_y++;
                        grid_sel_y = 3;
                    } else
                    {
                        grid_sel_y = 0;
                        grid_offset_y = 0;
                    }
                }

                updatePreviewText();
            }
        }

        if (inputCtrl->isKeyPressed(Input::Keys::CROSS))
        {
            if (!menu_mode)
            {
                menu_mode = true;
                updateInputControls();

                if (current_item_position == 0)
                {
                    active_category = 5;
                } else if (current_item_position == 1)
                {
                    active_category = 3;
                } else if (current_item_position == 4)
                {
                    active_category = 2;
                } else if (current_item_position == 3)
                {
                    active_category = 4;
                }

                loadInventory();
                setInventoryPosition();
            } else
            {
                ///Item selection was here
                int invbox_id = ((grid_offset_y + grid_sel_y) * 4) + grid_sel_x;

                if (inventory_boxes.size() > 0)
                {
                    if (invbox_id < inventory_boxes.size())
                    {
                        ///Check if item isn't highlighted, that means it's accessible
                        if (!inventory_boxes[invbox_id].highlight)
                        {
                            InventoryData::InventoryItem currentItem = CoreManager::getInstance().getSaveReader()->invData.items[inventory_boxes[invbox_id].inv_id];

                            SPDLOG_TRACE("InvID: {}", inventory_boxes[invbox_id].inv_id);

                            std::string str_order_id;

                            for (int i = 1; i < currentItem.item->order_id.size(); i++)
                            {
                                str_order_id += " " + std::to_string(currentItem.item->order_id[i]);
                            }

                            SPDLOG_TRACE("Order_id is {}", str_order_id);

                            if ((inventory_boxes[invbox_id].amount > inventory_boxes[invbox_id].occ_amount) && (CoreManager::getInstance().getSaveReader()->ponReg.pons[current_selected_pon].canEquip(currentItem.item->order_id, active_category - 3))) ///I have put active_category-2 here because where=0 when you wanna equip spear and where=1 when you wanna equip helm. theres some confusion between gui slots and equipment slots in ponregistry. gotta fix it someday.
                            {
                                CoreManager::getInstance().getSaveReader()->ponReg.pons[current_selected_pon].giveItem(CoreManager::getInstance().getSaveReader()->invData.getInvIDByItemID(currentItem.item->order_id), active_category - 3);

                                refreshStats();
                                applyEquipment();
                                updateInputControls();

                                menu_mode = false;
                            }
                        } else
                        {
                            SPDLOG_WARN("Cannot apply. All items are occupied.");
                        }
                    }
                }
            }
        }

        if (inputCtrl->isKeyPressed(Input::Keys::CIRCLE))
        {
            if (menu_mode)
            {
                menu_mode = false;

                updateInputControls();
                refreshStats();
            } else
            {
                inputCtrl->lockInput = true;
                screenFade.Create(ScreenFade::FADEOUT, 1024);

                if (obelisk)
                {
                    goto_id = 0;
                }
                else
                {
                    goto_id = 1;
                }
            }
        }

        if (inputCtrl->isKeyPressed(Input::Keys::START))
        {
            if (obelisk)
            {
                CoreManager::getInstance().getDialogHandler()->dialogboxes.emplace_back(std::make_unique<PataDialogBox>(Func::GetStrFromKey("barracks_depart"), std::vector<PataDialogBox::Option>{
                    {Func::GetStrFromKey("nav_yes"), [this, inputCtrl](){
                        SPDLOG_DEBUG("Go on mission!");
                        goto_id = 2;
                        inputCtrl->lockInput = true;
                        screenFade.Create(ScreenFade::FADEOUT, 1024);
                    }},
                    {Func::GetStrFromKey("nav_no"), [](){}}
                }));
            }
        }
    }

    if (screenFade.checkFinished())
    {
        inputCtrl->lockInput = false;

        switch (goto_id)
        {
            case 0: {
                StateManager::getInstance().setState(StateManager::OBELISK);
                break;
            }

            case 1: {
                StateManager::getInstance().setState(StateManager::PATAPOLIS);
                break;
            }

            case 2: {
                CoreManager::getInstance().getCore()->mission_file = mission_file;
                CoreManager::getInstance().getCore()->mission_id = mission_id;
                CoreManager::getInstance().getCore()->mission_multiplier = 1;

                StateManager::getInstance().setState(StateManager::MISSIONCONTROLLER);

                return; //we return here because Barracks get destroyed, so no illegal memory accesses are made.

                break;
            }
        }

        goto_id = -1;
    }
}

void Barracks::updateInputControls()
{
    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    if (!menu_mode)
    {
        if (!obelisk)
            ctrlTips.create(82, font, 20, sf::String("Left/Right: Select unit      Up/Down: Select equipment      X: Change equipment      O: Return to Patapolis"));
        else
            ctrlTips.create(82, font, 20, sf::String("Left/Right: Select unit      Up/Down: Select equipment      X: Change equipment      O: Return to World map      Start: Start mission"));
    } else
    {
        ctrlTips.create(82, font, 20, sf::String("Left/Right/Up/Down: Navigate item      X: Equip item      O: Cancel"));
    }
}

void Barracks::updateButtons()
{
    /// this should update the text on all the buttons
}

void Barracks::onExit()
{
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}

Barracks::~Barracks()
{
    //dtor
}
