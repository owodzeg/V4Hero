#include "Barracks.h"
#include "ButtonList.h"
#include <iostream>
#include "../V4Core.h"
#include <sstream>
#include "Altar.h"

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

Barracks::Barracks()
{
    //ctor
    //f_font.loadFromFile("resources/fonts/arial.ttf");

    is_active=false;
}
void Barracks::initialise(Config *_thisConfig, V4Core *parent, Menu *curParentMenu)
{
    parent->saveToDebugLog("Initializing Barracks...");
    Scene::Initialise(_thisConfig, parent);
    parentMenu = curParentMenu;
    currentController = &(v4Core->currentController);
    thisConfig = _thisConfig;

    f_font.loadFromFile(thisConfig->fontPath);

    int army_size = v4Core->saveReader.ponReg.pons.size();

    cout << "Barracks army size: " << army_size << endl;

    for(int i = 0; i < army_size; i++)
    {
        cout << "Checking Pon " << i << endl;

        Pon* current_pon = new Pon;
        current_pon = v4Core->saveReader.ponReg.GetPonByID(i);

        cout << "Pon class: " << current_pon->pon_class << endl;

        switch(current_pon->pon_class)
        {
            case 0:
            {
                unique_ptr<Yaripon> wip_pon = make_unique<Yaripon>();
                wip_pon.get()->setUnitID(current_pon->pon_class);

                cout << "Loading Pon..." << endl;

                wip_pon.get()->LoadConfig(thisConfig);
                wip_pon.get()->setAnimationSegment("idle_armed");

                cout << "Assigning equipment to Pon (" << parent->saveReader.ponReg.GetPonByID(i)->slots.size() << " slots)" << endl;

                for(int o = 0; o < parent->saveReader.ponReg.GetPonByID(i)->slots.size(); o++)
                {
                    cout << "Slot " << o << ": " << parent->saveReader.ponReg.GetPonByID(i)->slots[o] << endl;

                    if(parent->saveReader.ponReg.GetPonByID(i)->slots[o] != -1)
                    {
                        cout << "Applying equipment" << endl;
                        wip_pon.get()->applyEquipment(parent->saveReader.invData.items[parent->saveReader.ponReg.GetPonByID(i)->slots[o]].item->order_id, o);
                    }
                }

                units.push_back(std::move(wip_pon));
                break;
            }
            /*case 1:
            {
                unique_ptr<Tatepon> wip_pon = make_unique<Tatepon>();
                wip_pon.get()->setUnitID(current_pon->pon_class);

                wip_pon.get()->LoadConfig(thisConfig);
                wip_pon.get()->setAnimationSegment("idle_armed");

                wip_pon.get()->applyEquipment(parent->saveReader.invData.items[parent->saveReader.ponReg.GetPonByID(i)->slot_1_invItem_id].item->order_id, 0);
                if(parent->saveReader.invData.items[parent->saveReader.ponReg.GetPonByID(i)->slot_2_invItem_id].item->item_category == "weapon")
                {
                    wip_pon.get()->applyEquipment(parent->saveReader.invData.items[parent->saveReader.ponReg.GetPonByID(i)->slot_2_invItem_id].item->order_id, 1, true);
                }
                else
                {
                    wip_pon.get()->applyEquipment(parent->saveReader.invData.items[parent->saveReader.ponReg.GetPonByID(i)->slot_2_invItem_id].item->order_id, 1);
                }
                wip_pon.get()->applyEquipment(parent->saveReader.invData.items[parent->saveReader.ponReg.GetPonByID(i)->slot_3_invItem_id].item->order_id, 2);

                units.push_back(std::move(wip_pon));
                break;
            }*/
        }
    }

    int quality = thisConfig->GetInt("textureQuality");
    q = quality;

    switch(quality)
    {
        case 0: ///low
        {
            ratio_x = thisConfig->GetInt("resX") / float(640);
            ratio_y = thisConfig->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratio_x = thisConfig->GetInt("resX") / float(1280);
            ratio_y = thisConfig->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratio_x = thisConfig->GetInt("resX") / float(1920);
            ratio_y = thisConfig->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratio_x = thisConfig->GetInt("resX") / float(3840);
            ratio_y = thisConfig->GetInt("resY") / float(2160);
            break;
        }
    }

    res_ratio_x = thisConfig->GetInt("resX") / float(1280);
    res_ratio_y = thisConfig->GetInt("resY") / float(720);

    patapon_y = 607;
    floor_y = 980;

    mm_selected_item_line.setSize(sf::Vector2f(135 * res_ratio_x, 3 * res_ratio_y));
    mm_selected_item_line.setFillColor(sf::Color::Red);

    t_title.createText(f_font, 57, sf::Color::White, "", quality, 2);
    t_item_title.createText(f_font, 42, sf::Color::Black, "", quality, 2);

    ///             ####   BARRACKS MENU BACKGROUND
    s_background.loadFromFile("resources/graphics/bg/barracks/barracks.png", quality, 2);

    ///         highlighted unit
    s_pon_highlight.loadFromFile("resources/graphics/ui/highlight.png", quality, 2);

    ///             ####   UNIT CLASS ICON
    class_icon.loadFromFile("resources/graphics/ui/yari_icon.png", quality, 1);
    class_icon.setOrigin(class_icon.getLocalBounds().width/2, class_icon.getLocalBounds().height/2);
    class_icon.setPosition(102, 98);

    ///             ####   UNIT ITEM ICON
    s_unit_icon.loadFromFile("resources/graphics/ui/unit_icon.png", quality, 1);
    s_unit_icon.setPosition(946, 82);

    Pon* cur_pon = parentMenu->v4Core->saveReader.ponReg.GetPonByID(current_selected_pon);
    cout << "[DEBUG]" << parentMenu->v4Core->saveReader.ponReg.GetPonByID(current_selected_pon);
    unit_status.createText(f_font, 22, sf::Color(239, 88, 98, 255), Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_unit_status")), quality, 1);
    class_name.createText(f_font, 34, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_yaripon")), quality, 1);

    /// Stat text
    unit_stat_level_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_level")), quality, 1);
    unit_stat_level_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);
    unit_stat_exp_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_exp")), quality, 1);
    unit_stat_exp_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);
    unit_stat_hp_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_hp")), quality, 1);
    unit_stat_hp_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);
    unit_stat_dmg_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_damage")), quality, 1);
    unit_stat_dmg_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);
    unit_stat_atkspd_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_attackspeed")), quality, 1);
    unit_stat_atkspd_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);

    unit_stat_critc_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_critchance")), quality, 1);
    unit_stat_critc_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);
    unit_stat_kbc_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_knockbackchance")), quality, 1);
    unit_stat_kbc_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);
    unit_stat_stgc_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_staggerchance")), quality, 1);
    unit_stat_stgc_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);
    unit_stat_firec_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_burnchance")), quality, 1);
    unit_stat_firec_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);
    unit_stat_icec_t.createText(f_font, 27, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_stat_freezechance")), quality, 1);
    unit_stat_icec_v.createText(f_font, 27, sf::Color::Black, "", quality, 1);

    int equip_height = 50;

    ///             ####   WEAPON ITEM ICON
    s_weapon_icon.loadFromFile("resources/graphics/ui/sword_weapon_icon.png", quality, 1);
    s_weapon_icon.setPosition(946, s_unit_icon.getPosition().y+equip_height);

    ///             ####   WEAPON 2 (OTHER HAND) ITEM ICON
    s_weapon2_icon.loadFromFile("resources/graphics/ui/sword_weapon_icon.png", quality, 1);
    s_weapon2_icon.setPosition(946, s_weapon_icon.getPosition().y+equip_height);

    ///             ####   ARMOUR ITEM ICON
    s_armour_icon.loadFromFile("resources/graphics/ui/helm_icon.png", quality, 1);
    s_armour_icon.setPosition(946, s_weapon2_icon.getPosition().y+equip_height);

    ///             ####   MASK ITEM ICON
    s_mask_icon.loadFromFile("resources/graphics/ui/mask_icon.png", quality, 1);
    s_mask_icon.setPosition(946, s_armour_icon.getPosition().y+equip_height);

    /// unit + item name text

    t_unit_rarepon_name.createText(f_font, 24, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 1);
    t_unit_rarepon_name.setOrigin(0,t_unit_rarepon_name.getLocalBounds().height/2);

    for(int i = 0; i < t_eq_names.size(); i++)
    {
        t_eq_names[i].createText(f_font, 24, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 1); // Why always wooden spear? I assumed unimplemented and is gonna change?
        t_eq_names[i].setOrigin(0, t_eq_names[i].getLocalBounds().height / 2);
    }

    inv_box.loadFromFile("resources/graphics/ui/mini_inventory.png", quality, 1);

    item_title.createText(f_font, 34, sf::Color::Black, "", quality, 1);
    item_desc.createText(f_font, 22, sf::Color::Black, "", quality, 1);

    current_menu_position = -1;
    enabled_positons.push_back(true);
    enabled_positons.push_back(true);
    enabled_positons.push_back(false);
    enabled_positons.push_back(true);
    enabled_positons.push_back(false);

    quality_setting = quality;
    highlighted_pon.loadFromFile("resources/graphics/ui/highlighted_pon.png", quality_setting, 1);

    applyEquipment();

    rr_main_sh.Create(1100 + 2, 220 + 2, 20, thisConfig->GetInt("resX") / float(1280), sf::Color(0, 0, 0, 96));
    rr_main_sh.x = 670 - 1;
    rr_main_sh.y = 190 - 1;
    rr_main_sh.setOrigin(sf::Vector2f((1100 + 40) / 2, (220 + 40) / 2)); 
    
    rr_uniticon_sh.Create(176 + 2, 8 + 2, 34, thisConfig->GetInt("resX") / float(1280), sf::Color(0, 0, 0, 96));
    rr_uniticon_sh.x = 188 - 1;
    rr_uniticon_sh.y = 98 - 1;
    rr_uniticon_sh.setOrigin(sf::Vector2f((176 + 68) / 2, (8 + 68) / 2));

    rr_unitstatus_sh.Create(175 + 2, 20 + 2, 25, thisConfig->GetInt("resX") / float(1280), sf::Color(0, 0, 0, 96));
    rr_unitstatus_sh.x = 1127 - 1;
    rr_unitstatus_sh.y = 64 - 1;
    rr_unitstatus_sh.setOrigin(sf::Vector2f((175 + 50) / 2, (12 + 50) / 2));

    rr_main.Create(1100, 220, 20, thisConfig->GetInt("resX") / float(1280));
    rr_main.x = 670;
    rr_main.y = 190;
    rr_main.setOrigin(sf::Vector2f((1100 + 40) / 2, (220 + 40) / 2));

    rr_uniticon.Create(176, 8, 34, thisConfig->GetInt("resX") / float(1280));
    rr_uniticon.x = 188;
    rr_uniticon.y = 98;
    rr_uniticon.setOrigin(sf::Vector2f((176 + 68) / 2, (8 + 68) / 2));

    rr_uniticon.Create(175, 20, 25, thisConfig->GetInt("resX") / float(1280));
    rr_uniticon.x = 1127;
    rr_uniticon.y = 64;
    rr_uniticon.setOrigin(sf::Vector2f((175 + 50) / 2, (12 + 50) / 2));

    //mm_inventory_background.setSize(sf::Vector2f(mm_inventory_background.getSize().x+(40*resRatioX),mm_inventory_background.getSize().y+(40*resRatioX)));
    parent->saveToDebugLog("Initializing Barracks finished.");

}
void Barracks::eventFired(sf::Event event)
{
    if(is_active)
    {
        if(event.type == sf::Event::KeyPressed)
        {


        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        }
    }
}

int Barracks::countOccupied(vector<int> order_id)
{
    cout << "Barracks::countOccupied(" << order_id[0] << " " << order_id[1] << " " << order_id[2] << ")" << endl;
    int occ = 0;

    for(int i = 0; i < v4Core->saveReader.ponReg.pons.size(); i++) // Go through every pon
    {
        for(int o = 0; o < v4Core->saveReader.ponReg.pons[i].slots.size(); o++) // *through every equip slot of every pon
        {
            if(v4Core->saveReader.ponReg.pons[i].slots[o] == v4Core->saveReader.invData.getInvIDByItemID(order_id)) // if it matches the inv id (equivalent to is same item cause of the inv system)
            {
                occ++;
            }
        }
    }

    cout << "Item is occupied " << occ << " times" << endl;

    return occ;
}

void Barracks::loadInventory()
{
    inventory_boxes.clear();

    std::vector<InventoryData::InventoryItem> items_filtered; ///take only the items we want
    std::vector<int> items_invIDs; ///i dont remember if invid is accessible through inventoryitem

    for(int i = 0; i < v4Core->saveReader.invData.items.size(); i++)
    {
        InventoryData::InventoryItem cur_inv_item = v4Core->saveReader.invData.items[i];
        Item* cur_item = cur_inv_item.item;

        if(cur_item->order_id[0] == active_category)
        {
            if(active_category == 3) ///weapons, force spears
            {
                if(cur_item->order_id[1] == 0)
                {
                    items_filtered.push_back(v4Core->saveReader.invData.items[i]);
                    items_invIDs.push_back(i);
                }
            }

            if(active_category == 4) ///armor, force helms
            {
                if(cur_item->order_id[1] == 1)
                {
                    items_filtered.push_back(v4Core->saveReader.invData.items[i]);
                    items_invIDs.push_back(i);
                }
            }
        }
    }

    for(int i = 0; i < items_filtered.size(); i++)
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
        if(cur_count <= cur_occ_count)
        {
            cur_box.highlight = true;
        }
        else
        {
            cur_box.highlight = false;
        }

        cur_box.r_outer.setSize(sf::Vector2f(70.0 * res_ratio_x, 51.0 * res_ratio_y));
        cur_box.r_outer.setFillColor(sf::Color(102, 102, 102, 255));

        cur_box.r_inner.setSize(sf::Vector2f(46.0 * res_ratio_x, 46.0 * res_ratio_y));
        cur_box.r_inner.setFillColor(sf::Color(183, 183, 183, 255));

        switch(cur_item->order_id[0]) // Is there a better way than nested switches here? (look: weapons -> spears or swords will be necessary)
        {
            case 0: // Key Items
            {
                cur_box.r_inner.setFillColor(sf::Color(183, 183, 183, 255));

                ///look up item's icon
                cur_box.icon.loadFromFile("resources/graphics/ui/altar/materials/" + Func::num_padding(cur_item->spritesheet_id, 4) + ".png", q, 1);
                cur_box.icon.setOrigin(cur_box.icon.getLocalBounds().width / 2, cur_box.icon.getLocalBounds().height / 2);

                break;
            }

            case 1: // Materials
            {
                cur_box.r_inner.setFillColor(sf::Color(146, 173, 217, 255));

                ///look up item's icon
                cur_box.icon.loadFromFile("resources/graphics/ui/altar/materials/" + Func::num_padding(cur_item->spritesheet_id, 4) + ".png", q, 1);
                cur_box.icon.setOrigin(cur_box.icon.getLocalBounds().width / 2, cur_box.icon.getLocalBounds().height / 2);

                break;
            }

            case 3: // Weapons
            {
                cur_box.r_inner.setFillColor(sf::Color(199, 221, 167, 255));

                ///look up item's icon
                cur_box.icon.loadFromFile("resources/graphics/ui/altar/equip/spear_1.png", q, 1);
                cur_box.icon.setOrigin(cur_box.icon.getLocalBounds().width / 2, cur_box.icon.getLocalBounds().height / 2);

                break;
            }

            case 4: // Armour
            {
                cur_box.r_inner.setFillColor(sf::Color(199, 221, 167, 255));

                ///look up item's icon
                cur_box.icon.loadFromFile("resources/graphics/ui/altar/equip/helm_1.png", q, 1);
                cur_box.icon.setOrigin(cur_box.icon.getLocalBounds().width / 2, cur_box.icon.getLocalBounds().height / 2);

                break;
            }
        }

        cur_box.num.createText(f_font, 20, sf::Color::White, "001", q, 1); // I know what these do but am confused why there's a default value to begin with, should be "Error" or smth, so the player can't get fooled
        cur_box.num_shadow.createText(f_font, 20, sf::Color(136, 136, 36, 255), "001", q, 1);
        inventory_boxes.push_back(cur_box);
        inventory_boxes[i].num.setString(Func::num_padding(inventory_boxes[i].amount, 3));
        inventory_boxes[i].num_shadow.setString(Func::num_padding(inventory_boxes[i].amount, 3));
    }

    if(inventory_boxes.size() > 0) // I'm absolutely sure this is broken.
    {
        std::sort(inventory_boxes.begin(), inventory_boxes.end(),
                  [](const InvBox& a, const InvBox& b)
                    {
                        if(a.data->order_id[0] != b.data->order_id[0])
                        {
                            return a.data->order_id[0] < b.data->order_id[0];
                        }
                        if(a.data->order_id[1] != b.data->order_id[1])
                        {
                            return a.data->order_id[1] < b.data->order_id[1];
                        }
                        if(a.data->order_id.size() > 2 && b.data->order_id.size() > 2)
                        {
                            return a.data->order_id[2] < b.data->order_id[2];
                        }
                        else
                        {
                            cout << "[ERROR]: Inventory boxes sorting in barrack.cpp has reached a point it shouldn't have." << '\n' << "[ The items may or may not appear in the correct order because of this.";
                        }
                    });
    }

    refreshStats();
    applyEquipment();
}

void Barracks::setInventoryPosition()
{
    int inv_id = -1;

    Pon* currentPon = parentMenu->v4Core->saveReader.ponReg.GetPonByID(current_selected_pon);

    if(current_item_position == 1)
    inv_id = currentPon->slots[0];
    if(current_item_position == 3)
    inv_id = currentPon->slots[1];

    if(inv_id != -1)
    {
        Item* cur_item = v4Core->saveReader.invData.items[inv_id].item;

        int invbox_id = -1;

        for(int i = 0; i<inventory_boxes.size(); i++)
        {
            if(inventory_boxes[i].data == cur_item)
            invbox_id = i;
        }

        if(invbox_id != -1)
        {
            grid_offset_y = ceil((invbox_id + 1) / 4) - 4;

            if(grid_offset_y <= 0)
            grid_offset_y = 0;

            grid_sel_x = invbox_id % 4;

            grid_sel_y = ceil(((invbox_id + 1) - (grid_offset_y * 4)) / 4.0) - 1;

            cout << "invbox_id: " << invbox_id << " gridSelX: " << grid_sel_x << " gridSelY: " << grid_sel_y << " gridOffsetY: " << grid_offset_y << endl;
        }
        else
        {
            grid_sel_x = 0;
            grid_sel_y = 0;
            grid_offset_y = 0;
        }
    }
    else
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
    for(int i = 0; i < units.size(); i++)
    {
        for(int o = 0; o < v4Core->saveReader.ponReg.GetPonByID(i)->slots.size(); o++)
        {
            if(v4Core->saveReader.ponReg.GetPonByID(i)->slots[o] != -1)
            {
                units[i].get()->applyEquipment(v4Core->saveReader.invData.items[v4Core->saveReader.ponReg.GetPonByID(i)->slots[o]].item->order_id, o);
            }
        }
    }
}

void Barracks::refreshStats()
{
    Pon* currentPon = parentMenu->v4Core->saveReader.ponReg.GetPonByID(current_selected_pon);
    switch(currentPon->pon_class)
    {
        case 1:
        {
            class_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_yaripon")));
            break;
        }
        case 2:
        {
            class_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_tatepon")));
            break;
        }
        case 3:
        {
            class_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_yumipon")));
            break;
        }
        case 4:
        {
            class_name.setString(thisConfig->thisCore->saveReader.kami_name);
            break;
        }
    }
    t_unit_rarepon_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"rarepon_normal"))+" "+Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_lvl"))+" "+std::to_string(currentPon->pon_level));

    for(int i = 0; i < currentPon->slots.size(); i++)
    {
        if(currentPon->slots[i] >= 0)
        {
            InventoryData::InventoryItem eq = parentMenu->v4Core->saveReader.invData.items[currentPon->slots[i]];
            t_eq_names[i].setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(eq.item->item_name)));
            t_eq_names[i].setOrigin(0, 0);

            cout << "currentPon->slots[" << i << "]: " << currentPon->slots[i] << " " << eq.item->item_name << endl;
        }
    }

    //s_unit_level.setString(std::to_string(currentPon->pon_level));
    //s_unit_experience.setString(std::to_string(currentPon->pon_exp));
    //s_unit_experience.setString(std::to_string(currentPon->pon_exp));
    //s_unit_damage.setString(std::to_string(currentPon->pon_min_dmg)+"/"+std::to_string(currentPon->pon_max_dmg));
    //s_unit_hp.setString(std::to_string(currentPon->pon_hp));
    //s_unit_crit.setString(std::to_string(currentPon->pon_crit));
    //s_unit_attack_speed.setString(std::to_string(currentPon->pon_attack_speed));

    unit_stat_level_v.setString(std::to_string(currentPon->pon_level)); /// CHANGE THESE WHEN IMPLEMENTING STATUS EFFECTS ETC.
    unit_stat_exp_v.setString(std::to_string(currentPon->pon_exp)+"/1200");
    unit_stat_hp_v.setString(std::to_string(currentPon->pon_hp));
    unit_stat_dmg_v.setString(std::to_string(currentPon->pon_min_dmg)+"-"+std::to_string(currentPon->pon_max_dmg));
    unit_stat_atkspd_v.setString(to_string_with_precision(currentPon->pon_attack_speed, 2));
    unit_stat_critc_v.setString("0%");
    unit_stat_kbc_v.setString("0%");
    unit_stat_stgc_v.setString("0%");
    unit_stat_firec_v.setString("0%");
    unit_stat_icec_v.setString("0%");


    /*if (inventoryGridXPos+inventoryGridYPos*numItemColumns<v4core->saveReader.invdata.ItemsByType(activeCategory).size())
    {
        Item* starting_item = v4core->saveReader.invdata.ItemsByType(activeCategory)[inventoryGridXPos+inventoryGridYPos*numItemColumns].item;

        t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(starting_item->item_name)));
        t_itemtitle.setOrigin(t_itemtitle.getLocalBounds().width/2,t_itemtitle.getLocalBounds().height/2);
    }
    else
    {
        t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_none")));
        t_itemtitle.setOrigin(t_itemtitle.getLocalBounds().width/2,t_itemtitle.getLocalBounds().height/2);
    }*/
}

void Barracks::updatePreviewText()
{
    int invbox_id = ((grid_offset_y + grid_sel_y) * 4) + grid_sel_x;

    if(inventory_boxes.size() > 0)
    {
        if(invbox_id < inventory_boxes.size())
        {
            item_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(inventory_boxes[invbox_id].data->item_name)));
            item_desc.setString(Func::ConvertToUtf8String(Func::wrap_text(thisConfig->strRepo.GetUnicodeString(inventory_boxes[invbox_id].data->item_description), 340, f_font, 22)));
        }
        else
        {
            item_title.setString("");
            item_desc.setString("");
        }
    }
    else
    {
        item_title.setString("");
        item_desc.setString("");
    }
}

void Barracks::update(sf::RenderWindow &window, float fps, InputController& inputCtrl)
{
    if(is_active)
    {
        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        rs_cover.setSize(sf::Vector2f(1280 * res_ratio_x, 720 * res_ratio_y));
        rs_cover.setFillColor(sf::Color::Black);
        rs_cover.setPosition(0,0);
        window.draw(rs_cover);

        s_background.setPosition(0,0);
        s_background.draw(window);

        int highlight_width = 225;
        int pon_width = 75;

        highlighted_pon.setPosition((468 + (75*(current_selected_pon))), 530);
        highlighted_pon.draw(window);

        for(int i = 0; i < units.size(); i++)
        {
            units[i].get()->setGlobalPosition(sf::Vector2f(500 + (75*(i)), patapon_y));
            units[i].get()->fps = fps;
            units[i].get()->Draw(window);
        }

        s_pon_highlight.setPosition(highlight_width*2,675);
        s_pon_highlight.draw(window);

        
        rr_main_sh.Draw(window);
        rr_uniticon_sh.Draw(window);
        rr_unitstatus_sh.Draw(window);
        rr_main.Draw(window);
        rr_uniticon.Draw(window);
        rr_uniticon.Draw(window);

        class_icon.draw(window);

        unit_status.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_unit_status"))+" "+to_string(current_selected_pon+1)+"/3");
        unit_status.setPosition(1048, 38);

        Pon* currentPon = new Pon;
        currentPon = parentMenu->v4Core->saveReader.ponReg.GetPonByID(current_selected_pon);
        switch(currentPon->pon_class)
        {
            case 1:
            {
                class_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_yaripon")));
                break;
            }
            case 2:
            {
                class_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_tatepon")));
                break;
            }
        }
        class_name.setPosition(136, 74);

        unit_status.draw(window);
        class_name.draw(window);

        ///stat text
        unit_stat_level_t.setOrigin(0, unit_stat_level_t.getLocalBounds().height/2);
        unit_stat_exp_t.setOrigin(0, unit_stat_exp_t.getLocalBounds().height/2);
        unit_stat_hp_t.setOrigin(0, unit_stat_hp_t.getLocalBounds().height/2);
        unit_stat_dmg_t.setOrigin(0, unit_stat_dmg_t.getLocalBounds().height/2);
        unit_stat_atkspd_t.setOrigin(0, unit_stat_atkspd_t.getLocalBounds().height/2);
        unit_stat_critc_t.setOrigin(0, unit_stat_critc_t.getLocalBounds().height/2);
        unit_stat_kbc_t.setOrigin(0, unit_stat_kbc_t.getLocalBounds().height/2);
        unit_stat_stgc_t.setOrigin(0, unit_stat_stgc_t.getLocalBounds().height/2);
        unit_stat_firec_t.setOrigin(0, unit_stat_firec_t.getLocalBounds().height/2);
        unit_stat_icec_t.setOrigin(0, unit_stat_icec_t.getLocalBounds().height/2);

        unit_stat_level_v.setOrigin(unit_stat_level_v.getLocalBounds().width, unit_stat_level_v.getLocalBounds().height/2);
        unit_stat_exp_v.setOrigin(unit_stat_exp_v.getLocalBounds().width, unit_stat_exp_v.getLocalBounds().height/2);
        unit_stat_hp_v.setOrigin(unit_stat_hp_v.getLocalBounds().width, unit_stat_hp_v.getLocalBounds().height/2);
        unit_stat_dmg_v.setOrigin(unit_stat_dmg_v.getLocalBounds().width, unit_stat_dmg_v.getLocalBounds().height/2);
        unit_stat_atkspd_v.setOrigin(unit_stat_atkspd_v.getLocalBounds().width, unit_stat_atkspd_v.getLocalBounds().height/2);
        unit_stat_critc_v.setOrigin(unit_stat_critc_v.getLocalBounds().width, unit_stat_critc_v.getLocalBounds().height/2);
        unit_stat_kbc_v.setOrigin(unit_stat_kbc_v.getLocalBounds().width, unit_stat_kbc_v.getLocalBounds().height/2);
        unit_stat_stgc_v.setOrigin(unit_stat_stgc_v.getLocalBounds().width, unit_stat_stgc_v.getLocalBounds().height/2);
        unit_stat_firec_v.setOrigin(unit_stat_firec_v.getLocalBounds().width, unit_stat_firec_v.getLocalBounds().height/2);
        unit_stat_icec_v.setOrigin(unit_stat_icec_v.getLocalBounds().width, unit_stat_icec_v.getLocalBounds().height/2);

        unit_stat_level_t.setPosition(136,146);
        unit_stat_level_v.setPosition(136+370,146);
        unit_stat_exp_t.setPosition(136,180);
        unit_stat_exp_v.setPosition(136+370,180);
        unit_stat_hp_t.setPosition(136,214);
        unit_stat_hp_v.setPosition(136+370,214);
        unit_stat_dmg_t.setPosition(136,248);
        unit_stat_dmg_v.setPosition(136+370,248);
        unit_stat_atkspd_t.setPosition(136,282);
        unit_stat_atkspd_v.setPosition(136+370,282);

        unit_stat_critc_t.setPosition(540,146);
        unit_stat_critc_v.setPosition(540+370,146);
        unit_stat_kbc_t.setPosition(540,180);
        unit_stat_kbc_v.setPosition(540+370,180);
        unit_stat_stgc_t.setPosition(540,214);
        unit_stat_stgc_v.setPosition(540+370,214);
        unit_stat_firec_t.setPosition(540,248);
        unit_stat_firec_v.setPosition(540+370,248);
        unit_stat_icec_t.setPosition(540,282);
        unit_stat_icec_v.setPosition(540+370,282);

        unit_stat_level_t.draw(window);
        unit_stat_level_v.draw(window);
        unit_stat_exp_t.draw(window);
        unit_stat_exp_v.draw(window);
        unit_stat_hp_t.draw(window);
        unit_stat_hp_v.draw(window);
        unit_stat_dmg_t.draw(window);
        unit_stat_dmg_v.draw(window);
        unit_stat_atkspd_t.draw(window);
        unit_stat_atkspd_v.draw(window);

        unit_stat_critc_t.draw(window);
        unit_stat_critc_v.draw(window);
        unit_stat_kbc_t.draw(window);
        unit_stat_kbc_v.draw(window);
        unit_stat_stgc_t.draw(window);
        unit_stat_stgc_v.draw(window);
        unit_stat_firec_t.draw(window);
        unit_stat_firec_v.draw(window);
        unit_stat_icec_t.draw(window);
        unit_stat_icec_v.draw(window);


        /// equipped item + unit name text
        int equip_height = 53;

        if (enabled_positons[0]) /// Rarepon
        {
            t_unit_rarepon_name.setOrigin(0, 0);
            t_unit_rarepon_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_unit_icon.getPosition().y+8);
            s_unit_icon.setPosition(s_unit_icon.getPosition().x,s_unit_icon.getPosition().y);
            s_unit_icon.draw(window);
            t_unit_rarepon_name.draw(window);
        }
        if (enabled_positons[1]) /// wep1
        {
            t_slot_1_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_weapon_icon.getPosition().y+8);
            s_weapon_icon.setPosition(s_unit_icon.getPosition().x,s_weapon_icon.getPosition().y);
            s_weapon_icon.draw(window);
            t_slot_1_name.draw(window);

            t_eq_names[0].setPosition(s_unit_icon.getPosition().x+equip_height,s_weapon_icon.getPosition().y+8);
            t_eq_names[0].draw(window);
        }

        if (enabled_positons[2]) /// wep2
        {
            t_slot_2_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_weapon2_icon.getPosition().y+8);
            s_weapon2_icon.setPosition(s_unit_icon.getPosition().x,s_weapon2_icon.getPosition().y);
            s_weapon2_icon.draw(window);
            t_slot_2_name.draw(window);

            //t_eq_names[1].setPosition(s_unit_icon.getPosition().x+equip_height,s_weapon2_icon.getPosition().y+8);
            //t_eq_names[1].draw(window);
        }

        if (enabled_positons[3]) /// armour
        {
            t_slot_3_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_armour_icon.getPosition().y+8);
            s_armour_icon.setPosition(s_unit_icon.getPosition().x,s_armour_icon.getPosition().y);
            s_armour_icon.draw(window);
            t_slot_3_name.draw(window);

            /// //////////////////// ///
            /// hardcoded solution
            /// i didnt want to bother with right slot positions compared to t_eq_names
            /// slot[1] in ponregistry is armor
            /// armor is slot 2 (third) in equipment, not second
            /// //////////////////// ///
            t_eq_names[1].setPosition(s_unit_icon.getPosition().x+equip_height,s_armour_icon.getPosition().y+8);
            t_eq_names[1].draw(window);
        }

        if (enabled_positons[4]) /// mask
        {
            t_slot_4_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_armour_icon.getPosition().y+8);
            s_armour_icon.setPosition(s_unit_icon.getPosition().x,s_armour_icon.getPosition().y);
            s_armour_icon.draw(window);
            t_slot_4_name.draw(window);

            t_eq_names[3].setPosition(s_unit_icon.getPosition().x+equip_height,s_armour_icon.getPosition().y+8);
            t_eq_names[3].draw(window);
        }

        item_line_flash += 8.0 / fps;

        int bar_size = 266;
        int bar_offset = 78;
        mm_selected_item_line.setSize(sf::Vector2f(bar_size * res_ratio_x, 3 * res_ratio_y));
        mm_selected_item_line.setFillColor(sf::Color(239, 88, 98, 128+(sin(item_line_flash)*128)));
        mm_selected_item_line.setPosition((s_unit_icon.getPosition().x) * res_ratio_x,((s_unit_icon.getPosition().y + current_item_position * 50) + 47) * res_ratio_y);

        ctrlTips.x = 0;
        ctrlTips.y = (720 - ctrlTips.ySize);
        ctrlTips.draw(window);

        if(menu_mode)
        {
            inv_box.setPosition(40,366);
            inv_box.draw(window);

            for(int i=0; i<16; i++)
            {
                if(grid_offset_y * 4 + i < inventory_boxes.size())
                {
                    if(inventory_boxes.size() > 0)
                    {
                        int cur_item = grid_offset_y * 4 + i;

                        int grid_x = i % 4;
                        int grid_y = floor(i/4);

                        float xpos = 46 + (grid_x * 77);
                        float ypos = 37 + (grid_y * 54);

                        inventory_boxes[cur_item].r_outer.setPosition((40 + xpos)* res_ratio_x, (366 + ypos) * res_ratio_y);
                        inventory_boxes[cur_item].r_inner.setPosition((40 + xpos + 2.5) * res_ratio_x, (366 + ypos + 2.5) * res_ratio_y);
                        window.draw(inventory_boxes[cur_item].r_outer);
                        window.draw(inventory_boxes[cur_item].r_inner);

                        //inventory_boxes[i].num.setOrigin(inventory_boxes[i].num.getLocalBounds().width,inventory_boxes[i].num.getLocalBounds().height);
                        //inventory_boxes[i].num_shadow.setOrigin(inventory_boxes[i].num_shadow.getLocalBounds().width,inventory_boxes[i].num_shadow.getLocalBounds().height);

                        inventory_boxes[cur_item].icon.setScale(0.64,0.64);

                        if((inventory_boxes[cur_item].data->item_category == "key_items") || (inventory_boxes[cur_item].data->item_category == "consumables")) ///Bound to break
                        inventory_boxes[cur_item].icon.setScale(0.41,0.41);

                        inventory_boxes[cur_item].icon.setPosition(40 + xpos + 23 + 1.5, 366 + ypos + 23 + 1.5);
                        inventory_boxes[cur_item].icon.draw(window);

                        inventory_boxes[cur_item].num.setPosition(40 + xpos + 36 - 1, 366 + ypos + 29 - 2);
                        inventory_boxes[cur_item].num_shadow.setPosition(40 + xpos + 36, 366 + ypos + 29);

                        inventory_boxes[cur_item].num_shadow.draw(window);
                        inventory_boxes[cur_item].num.draw(window);

                        if(inventory_boxes[cur_item].highlight)
                        {
                            inventory_boxes[cur_item].r_highlight.setSize(sf::Vector2f(70.0 * res_ratio_x, 51.0 * res_ratio_y));
                            inventory_boxes[cur_item].r_highlight.setPosition((40 + xpos) * res_ratio_x, (366 + ypos) * res_ratio_y);
                            inventory_boxes[cur_item].r_highlight.setFillColor(sf::Color(0,0,0,192));
                            window.draw(inventory_boxes[cur_item].r_highlight);
                        }
                    }
                }
                else
                {
                    InvBox tmp_inv;

                    int grid_x = i % 4;
                    int grid_y = floor(i / 4);

                    float xpos = 46 + (grid_x * 77);
                    float ypos = 37 + (grid_y * 54);

                    tmp_inv.r_outer.setSize(sf::Vector2f(70.0 * res_ratio_x, 51.0 * res_ratio_y));
                    tmp_inv.r_outer.setFillColor(sf::Color(102,102,102,255));

                    tmp_inv.r_inner.setSize(sf::Vector2f(46.0 * res_ratio_x, 46.0 * res_ratio_y));
                    tmp_inv.r_inner.setFillColor(sf::Color(183,183,183,255));

                    tmp_inv.r_outer.setPosition((40 + xpos) * res_ratio_x, (366 + ypos) * res_ratio_y);
                    tmp_inv.r_inner.setPosition((40 + xpos + 2.5) * res_ratio_x, (366 + ypos + 2.5) * res_ratio_y);
                    window.draw(tmp_inv.r_outer);
                    window.draw(tmp_inv.r_inner);

                    tmp_inv.r_highlight.setSize(sf::Vector2f(70.0 * res_ratio_x, 51.0 * res_ratio_y));
                    tmp_inv.r_highlight.setPosition((40 + xpos) * res_ratio_x, (366 + ypos) * res_ratio_y);
                    tmp_inv.r_highlight.setFillColor(sf::Color(0,0,0,192));
                    window.draw(tmp_inv.r_highlight);
                }
            }

            r_sel.setSize(sf::Vector2f(70.0 * res_ratio_x, 51.0 * res_ratio_y));
            r_sel.setFillColor(sf::Color::Transparent);
            r_sel.setOutlineThickness(2);
            r_sel.setOutlineColor(sf::Color(255,0,32,255));
            r_sel.setPosition((40 + 46 + (grid_sel_x * 77)) * res_ratio_x, (366 + 37 + (grid_sel_y * 54)) * res_ratio_y);

            window.draw(r_sel);

            rr_itempreview_sh.Create(360+2, 260+2, 20, window.getSize().x / float(1280), sf::Color(0,0,0,96));
            rr_itempreview_sh.x = 650-1;
            rr_itempreview_sh.y = 490-1;
            rr_itempreview_sh.setOrigin(sf::Vector2f((360+40)/2, (220+40)/2));
            rr_itempreview_sh.Draw(window);

            rr_itempreview.Create(360, 260, 20, window.getSize().x / float(1280));
            rr_itempreview.x = 650;
            rr_itempreview.y = 490;
            rr_itempreview.setOrigin(sf::Vector2f((360+40)/2, (220+40)/2));
            rr_itempreview.Draw(window);

            item_title.setOrigin(item_title.getLocalBounds().width/2, item_title.getLocalBounds().height/2);
            item_title.setPosition(650, 390);
            item_title.draw(window);

            item_desc.setPosition(480, 440);
            item_desc.draw(window);
        }

        window.draw(mm_selected_item_line);

        vector<int> db_e; ///dialog box erase

        for(int i=0; i<dialog_boxes.size(); i++)
        {
            dialog_boxes[i].x = 640;
            dialog_boxes[i].y = 360;
            dialog_boxes[i].Draw(window, fps, inputCtrl);

            if(dialog_boxes[i].closed)
            db_e.push_back(i);
        }

        for(int i=0; i<db_e.size(); i++)
        {
            dialog_boxes.erase(dialog_boxes.begin()+db_e[i]-i);
        }

        window.setView(lastView);

        if(dialog_boxes.size() <= 0)
        {
            if(!menu_mode)
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

                if(inputCtrl.isKeyPressed(InputController::Keys::UP))
                {
                    /*current_item_position -=1;
                    if (current_item_position<0){
                        current_item_position = enabled_positons.size()-1;
                    }
                    while(!enabled_positons[current_item_position]){
                        current_item_position -=1;
                    }*/

                    cout << "Current item position: " << current_item_position << endl;

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
                                    cout << "Going to current_item_position=" << current_item_position << endl;
                                    break;
                                }
                            }
                            else
                            {
                                ///Go back to bottom
                                current_item_position = enabled_positons.size()-1;
                            }
                        }
                    }
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
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

                    cout << "Current item position: " << current_item_position << endl;

                    // if (4 < 4) ok
                    if (current_item_position < enabled_positons.size()-1)
                    {
                        bool found = false;

                        while (!found)
                        {
                            current_item_position++;

                            if (current_item_position <= enabled_positons.size()-1)
                            {
                                if (enabled_positons[current_item_position])
                                {
                                    found = true;
                                    cout << "Going to current_item_position=" << current_item_position << endl;
                                    break;
                                }
                            }
                            else
                            {
                                ///Go back to top
                                current_item_position = -1;
                            }
                        }
                    }
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::LEFT))
                {
                    if(current_selected_pon > 0)
                    current_selected_pon--;

                    refreshStats();
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
                {
                    if(current_selected_pon < 2)
                    current_selected_pon++;

                    refreshStats();
                }
            }
            else
            {
                if(inputCtrl.isKeyPressed(InputController::Keys::LEFT))
                {
                    grid_sel_x--;

                    if(grid_sel_x < 0)
                    grid_sel_x = 3;

                    updatePreviewText();
                }
                if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
                {
                    grid_sel_x++;

                    if(grid_sel_x > 3)
                    grid_sel_x = 0;

                    updatePreviewText();
                }
                if(inputCtrl.isKeyPressed(InputController::Keys::UP))
                {
                    grid_sel_y--;

                    if(grid_sel_y < 0)
                    {
                        if(grid_offset_y > 0)
                        {
                            grid_offset_y--;
                            grid_sel_y = 0;
                        }
                        else
                        {
                            grid_offset_y = ceil(inventory_boxes.size() / 4.0) - 4;

                            if(grid_offset_y < 0)
                            grid_offset_y = 0;

                            grid_sel_y = 3;
                        }
                    }

                    updatePreviewText();
                }
                if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
                {
                    grid_sel_y++;

                    if(grid_sel_y > 3)
                    {
                        if(inventory_boxes.size() > (4 + grid_offset_y) * 4)
                        {
                            grid_offset_y++;
                            grid_sel_y = 3;
                        }
                        else
                        {
                            grid_sel_y = 0;
                            grid_offset_y = 0;
                        }
                    }

                    updatePreviewText();
                }
            }

            if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
            {
                if(!menu_mode)
                {
                    menu_mode = true;
                    updateInputControls();

                    if (current_item_position == 0)
                    {
                        active_category = 5;
                    }
                    else if(current_item_position == 1)
                    {
                        active_category = 3;
                    }
                    else if(current_item_position == 4)
                    {
                        active_category = 2;
                    }
                    else if(current_item_position == 3)
                    {
                        active_category = 4;
                    }

                    loadInventory();
                    setInventoryPosition();
                }
                else
                {
                    ///Item selection was here
                    int invbox_id = ((grid_offset_y + grid_sel_y) * 4) + grid_sel_x;

                    if(inventory_boxes.size() > 0)
                    {
                        if(invbox_id < inventory_boxes.size())
                        {
                            ///Check if item isn't highlighted, that means it's accessible
                            if(!inventory_boxes[invbox_id].highlight)
                            {
                                InventoryData::InventoryItem currentItem = v4Core->saveReader.invData.items[inventory_boxes[invbox_id].inv_id];

                                cout << "InvID: " << inventory_boxes[invbox_id].inv_id << endl;

                                cout << "order_id is: ";

                                for(int i=0; i<currentItem.item->order_id.size(); i++)
                                cout << currentItem.item->order_id[i] << " ";

                                cout << endl;

                                if((inventory_boxes[invbox_id].amount > inventory_boxes[invbox_id].occ_amount) && (v4Core->saveReader.ponReg.pons[current_selected_pon].canEquip(currentItem.item->order_id, active_category-3))) ///I have put active_category-2 here because where=0 when you wanna equip spear and where=1 when you wanna equip helm. theres some confusion between gui slots and equipment slots in ponregistry. gotta fix it someday.
                                {
                                    v4Core->saveReader.ponReg.pons[current_selected_pon].giveItem(v4Core->saveReader.invData.getInvIDByItemID(currentItem.item->order_id), active_category-3);

                                    refreshStats();
                                    applyEquipment();
                                    updateInputControls();

                                    menu_mode = false;
                                }
                            }
                            else
                            {
                                cout << "Cannot apply. All items are occupied." << endl;
                            }
                        }
                    }
                }
            }

            if(inputCtrl.isKeyPressed(InputController::Keys::CIRCLE))
            {
                if (menu_mode)
                {
                    menu_mode=false;

                    updateInputControls();
                    refreshStats();
                }
                else
                {
                    parentMenu->screenFade.Create(thisConfig, 1, 1536);
                    parentMenu->goto_id = 2;

                    /*this->Hide();
                    this->isActive = false;
                    parentMenu->Show();
                    parentMenu->isActive=true;*/
                }
            }

            if(inputCtrl.isKeyPressed(InputController::Keys::START))
            {
                if(obelisk)
                {
                    std::vector<sf::String> a = {Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"nav_yes")),Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"nav_no"))};

                    PataDialogBox db;
                    db.Create(f_font, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_depart")), a, thisConfig->GetInt("textureQuality"));
                    db.id = 0;
                    dialog_boxes.push_back(db);
                }
            }
        }
        else
        {
            if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
            {
                switch(dialog_boxes[dialog_boxes.size()-1].CheckSelectedOption())
                {
                    case 0:
                    {
                        if(dialog_boxes[dialog_boxes.size()-1].id == 0)
                        {
                            cout << "Go on mission!" << endl;
                            dialog_boxes[dialog_boxes.size()-1].Close();

                            parentMenu->screenFade.Create(thisConfig, 1, 1536);
                            parentMenu->goto_id = 5;

                            /*sf::Thread loadingThreadInstance(v4core->LoadingThread,v4core);
                            v4core->continueLoading=true;
                            v4core->window.setActive(false);
                            loadingThreadInstance.launch();

                            currentController->Initialise(*config,config->GetString("mission1Background"),*v4core);
                            currentController->StartMission(mission_file,1,missionID,mission_multiplier);
                            this->Hide();
                            this->isActive = false;

                            missionStarted = true;

                            v4core->continueLoading=false;*/

                            break;
                        }
                    }

                    case 1:
                    {
                        cout << "Back to Barracks" << endl;
                        dialog_boxes[dialog_boxes.size()-1].Close();

                        break;
                    }
                }
            }
        }
    }
}

void Barracks::updateInputControls()
{
    if(!menu_mode)
    {
        if(!obelisk)
        ctrlTips.create(82, f_font, 20, sf::String(L"Left/Right: Select unit      Up/Down: Select equipment      X: Change equipment      O: Return to Patapolis"), quality_setting);
        else
        ctrlTips.create(82, f_font, 20, sf::String(L"Left/Right: Select unit      Up/Down: Select equipment      X: Change equipment      O: Return to World map      Start: Start mission"), quality_setting);
    }
    else
    {
        ctrlTips.create(82, f_font, 20, sf::String(L"Left/Right/Up/Down: Navigate item      X: Equip item      O: Cancel"), quality_setting);
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
