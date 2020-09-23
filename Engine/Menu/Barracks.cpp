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
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");

    patapon = new Patapon;
    patapon2 = new Patapon;
    patapon3 = new Patapon;

    isActive=false;
}
void Barracks::Initialise(Config *thisConfigs, V4Core *parent, Menu *curParentMenu)
{
    parent->SaveToDebugLog("Initializing Barracks...");
    Scene::Initialise(thisConfigs, parent);
    parentMenu = curParentMenu;
    currentController = &(v4core->currentController);
    config = thisConfigs;

    patapon->LoadConfig(thisConfigs);
    patapon->setAnimationSegment("idle_armed");
    patapon2->LoadConfig(thisConfigs);
    patapon2->setAnimationSegment("idle_armed");
    patapon3->LoadConfig(thisConfigs);
    patapon3->setAnimationSegment("idle_armed");

    patapon->applySpear(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(0)->weapon_invItem_id).item->equip_id);
    patapon2->applySpear(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(1)->weapon_invItem_id).item->equip_id);
    patapon3->applySpear(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(2)->weapon_invItem_id).item->equip_id);

    patapon->applyHelm(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(0)->armour_invItem_id).item->equip_id);
    patapon2->applyHelm(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(1)->armour_invItem_id).item->equip_id);
    patapon3->applyHelm(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(2)->armour_invItem_id).item->equip_id);

    int quality = thisConfigs->GetInt("textureQuality");
    q = quality;

    switch(quality)
    {
        case 0: ///low
        {
            ratioX = thisConfigs->GetInt("resX") / float(640);
            ratioY = thisConfigs->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = thisConfigs->GetInt("resX") / float(1280);
            ratioY = thisConfigs->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = thisConfigs->GetInt("resX") / float(1920);
            ratioY = thisConfigs->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = thisConfigs->GetInt("resX") / float(3840);
            ratioY = thisConfigs->GetInt("resY") / float(2160);
            break;
        }
    }

    resRatioX = thisConfigs->GetInt("resX") / float(1280);
    resRatioY = thisConfigs->GetInt("resY") / float(720);

    pataponY = 607;
    floorY = 980;

    mm_selected_item_line.setSize(sf::Vector2f(135*resRatioX,3*resRatioY));
    mm_selected_item_line.setFillColor(sf::Color::Red);

    t_title.createText(f_font, 57, sf::Color::White, "", quality, 2);
    t_itemtitle.createText(f_font, 42, sf::Color::Black, "", quality, 2);

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

    t_weapon_name.createText(f_font, 24, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 1);
    t_weapon_name.setOrigin(0,t_weapon_name.getLocalBounds().height/2);

    t_weapon2_name.createText(f_font, 24, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 1);
    t_weapon2_name.setOrigin(0,t_weapon2_name.getLocalBounds().height/2);

    t_armour_name.createText(f_font, 24, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 1);
    t_armour_name.setOrigin(0,t_armour_name.getLocalBounds().height/2);

    t_mask_name.createText(f_font, 24, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 1);
    t_mask_name.setOrigin(0,t_mask_name.getLocalBounds().height/2);

    inv_box.loadFromFile("resources/graphics/ui/mini_inventory.png", quality, 1);

    item_title.createText(f_font, 34, sf::Color::Black, "", quality, 1);
    item_desc.createText(f_font, 22, sf::Color::Black, "", quality, 1);

    currentMenuPosition = -1;
    enabledPositons.push_back(true);
    enabledPositons.push_back(true);
    enabledPositons.push_back(false);
    enabledPositons.push_back(true);
    enabledPositons.push_back(false);

    qualitySetting = quality;
    highlighted_pon.loadFromFile("resources/graphics/ui/highlighted_pon.png", qualitySetting, 1);

    ApplyEquipment();

    //mm_inventory_background.setSize(sf::Vector2f(mm_inventory_background.getSize().x+(40*resRatioX),mm_inventory_background.getSize().y+(40*resRatioX)));
    parent->SaveToDebugLog("Initializing Barracks finished.");

}
void Barracks::EventFired(sf::Event event)
{
    if(isActive)
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
void Barracks::ReloadInventory()
{
    inventory_boxes.clear();

    for(int i=0; i<v4core->savereader.invdata.items.size(); i++)
    {
        InventoryItem cur_invitem = v4core->savereader.invdata.GetItemByInvID(i);
        Item* cur_item = cur_invitem.item;

        if(cur_item->category_id == activeCategory)
        {
            bool ex = false;

            ///Check if InvBox with following item already exists
            for(int b=0; b<inventory_boxes.size(); b++)
            {
                if(inventory_boxes[b].data == cur_item)
                {
                    ex = true;
                    inventory_boxes[b].amount++;

                    if(cur_invitem.occupied)
                    inventory_boxes[b].occ_amount++;

                    inventory_boxes[b].invIDs.push_back(i);

                    if(inventory_boxes[b].amount == inventory_boxes[b].occ_amount)
                    inventory_boxes[b].highlight = true;
                    else
                    inventory_boxes[b].highlight = false;

                    inventory_boxes[b].num.setString(Func::num_padding(inventory_boxes[b].amount, 3));
                    inventory_boxes[b].num_shadow.setString(Func::num_padding(inventory_boxes[b].amount, 3));
                }
            }

            if(!ex)
            {
                InvBox tmp;
                tmp.data = cur_item;
                tmp.amount = 1;

                if(cur_invitem.occupied)
                {
                    tmp.occ_amount = 1;
                    tmp.highlight = true;
                }

                tmp.invIDs.push_back(i);

                tmp.r_outer.setSize({70.0*resRatioX, 51.0*resRatioY});
                tmp.r_outer.setFillColor(sf::Color(102,102,102,255));

                tmp.r_inner.setSize({46.0*resRatioX, 46.0*resRatioY});
                tmp.r_inner.setFillColor(sf::Color(183,183,183,255));

                switch(cur_item->category_id)
                {
                    case 0: ///materials
                    {
                        tmp.r_inner.setFillColor(sf::Color(146,173,217,255));

                        ///look up material's icon
                        tmp.icon.loadFromFile("resources/graphics/ui/altar/materials/"+Func::num_padding(cur_item->spritesheet_id, 4)+".png", q, 1);
                        tmp.icon.setOrigin(tmp.icon.getLocalBounds().width/2, tmp.icon.getLocalBounds().height/2);

                        break;
                    }

                    case 1: ///spears
                    {
                        tmp.r_inner.setFillColor(sf::Color(199,221,167,255));

                        ///look up material's icon
                        tmp.icon.loadFromFile("resources/graphics/ui/altar/equip/spear_1.png", q, 1);
                        tmp.icon.setOrigin(tmp.icon.getLocalBounds().width/2, tmp.icon.getLocalBounds().height/2);

                        break;
                    }

                    case 3: ///helms
                    {
                        tmp.r_inner.setFillColor(sf::Color(199,221,167,255));

                        ///look up material's icon
                        tmp.icon.loadFromFile("resources/graphics/ui/altar/equip/helm_1.png", q, 1);
                        tmp.icon.setOrigin(tmp.icon.getLocalBounds().width/2, tmp.icon.getLocalBounds().height/2);

                        break;
                    }

                    case 4: ///key items
                    {
                        tmp.r_inner.setFillColor(sf::Color(183,183,183,255));

                        ///look up material's icon
                        tmp.icon.loadFromFile("resources/graphics/ui/altar/materials/"+Func::num_padding(cur_item->spritesheet_id, 4)+".png", q, 1);
                        tmp.icon.setOrigin(tmp.icon.getLocalBounds().width/2, tmp.icon.getLocalBounds().height/2);

                        break;
                    }
                }

                tmp.num.createText(f_font, 20, sf::Color::White, "001", q, 1);
                tmp.num_shadow.createText(f_font, 20, sf::Color(136,136,136,255), "001", q, 1);

                inventory_boxes.push_back(tmp);
            }
        }
    }

    if(inventory_boxes.size() > 0)
    {
        std::sort(inventory_boxes.begin(), inventory_boxes.end(),
                  [](const InvBox& a, const InvBox& b)
                    {
                        return a.data->altar_order_id < b.data->altar_order_id;
                    });
    }

    RefreshStats();
    ApplyEquipment();
}

void Barracks::SetInventoryPosition()
{
    int invID = -1;

    Pon* currentPon = parentMenu->v4core->savereader.ponreg.GetPonByID(current_selected_pon);

    if (currentItemPosition==1)
    {
        invID = currentPon->weapon_invItem_id;
    }
    else if (currentItemPosition==2)
    {
        invID = currentPon->weapon2_invItem_id;
    }
    else if(currentItemPosition==4)
    {
        invID = currentPon->mask_invItem_id;
    }
    else if(currentItemPosition==3)
    {
        invID = currentPon->armour_invItem_id;
    }

    if(invID != -1)
    {
        Item* cur_item = v4core->savereader.invdata.GetItemByInvID(invID).item;

        int invbox_id = -1;

        for(int i=0; i<inventory_boxes.size(); i++)
        {
            if(inventory_boxes[i].data == cur_item)
            invbox_id = i;
        }

        if(invbox_id != -1)
        {
            gridOffsetY = ceil((invbox_id+1) / 4) - 4;

            if(gridOffsetY <= 0)
            gridOffsetY = 0;

            gridSelX = invbox_id % 4;

            gridSelY = ceil(((invbox_id+1) - (gridOffsetY*4)) / 4.0) - 1;

            cout << "invbox_id: " << invbox_id << " gridSelX: " << gridSelX << " gridSelY: " << gridSelY << " gridOffsetY: " << gridOffsetY << endl;
        }
        else
        {
            gridSelX = 0;
            gridSelY = 0;
            gridOffsetY = 0;
        }
    }
    else
    {
        gridSelX = 0;
        gridSelY = 0;
        gridOffsetY = 0;
    }

    RefreshStats();
    ApplyEquipment();
    UpdatePreviewText();
}

void Barracks::ApplyEquipment()
{
    patapon->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(0)->weapon_invItem_id).item->equip_id);
    patapon2->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(1)->weapon_invItem_id).item->equip_id);
    patapon3->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(2)->weapon_invItem_id).item->equip_id);

    patapon->applyHelm(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(0)->armour_invItem_id).item->equip_id);
    patapon2->applyHelm(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(1)->armour_invItem_id).item->equip_id);
    patapon3->applyHelm(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(2)->armour_invItem_id).item->equip_id);
}

void Barracks::RefreshStats()
{
    Pon* currentPon = parentMenu->v4core->savereader.ponreg.GetPonByID(current_selected_pon);

    t_unit_rarepon_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"rarepon_normal"))+" "+Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_lvl"))+" "+std::to_string(currentPon->pon_level));

    if(currentPon->weapon_invItem_id>=0)
    {
        InventoryItem wep = parentMenu->v4core->savereader.invdata.GetItemByInvID(currentPon->weapon_invItem_id);
        t_weapon_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(wep.item->item_name)));
    }

    if(currentPon->weapon2_invItem_id>=0)
    {
        InventoryItem wep2 = parentMenu->v4core->savereader.invdata.GetItemByInvID(currentPon->weapon2_invItem_id);
         t_weapon2_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(wep2.item->item_name)));
    }

    if(currentPon->armour_invItem_id>=0)
{
        InventoryItem armour = parentMenu->v4core->savereader.invdata.GetItemByInvID(currentPon->armour_invItem_id);
        t_armour_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(armour.item->item_name)));
    }

    if(currentPon->mask_invItem_id>=0)
    {
        InventoryItem mask = parentMenu->v4core->savereader.invdata.GetItemByInvID(currentPon->mask_invItem_id);
        t_mask_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(mask.item->item_name)));
    }


    //s_unit_level.setString(std::to_string(currentPon->pon_level));
    //s_unit_experience.setString(std::to_string(currentPon->pon_exp));
    //s_unit_experience.setString(std::to_string(currentPon->pon_exp));
    //s_unit_damage.setString(std::to_string(currentPon->pon_min_dmg)+"/"+std::to_string(currentPon->pon_max_dmg));
    //s_unit_hp.setString(std::to_string(currentPon->pon_hp));
    //s_unit_crit.setString(std::to_string(currentPon->pon_crit));
    //s_unit_attack_speed.setString(std::to_string(currentPon->pon_attack_speed));

    unit_stat_level_v.setString(std::to_string(currentPon->pon_level));
    unit_stat_exp_v.setString(std::to_string(currentPon->pon_exp)+"/1200");
    unit_stat_hp_v.setString(std::to_string(currentPon->pon_hp));
    unit_stat_dmg_v.setString(std::to_string(currentPon->pon_min_dmg)+"-"+std::to_string(currentPon->pon_max_dmg));
    unit_stat_atkspd_v.setString(to_string_with_precision(currentPon->pon_attack_speed, 2));
    unit_stat_critc_v.setString("0%");
    unit_stat_kbc_v.setString("0%");
    unit_stat_stgc_v.setString("0%");
    unit_stat_firec_v.setString("0%");
    unit_stat_icec_v.setString("0%");


    /*if (inventoryGridXPos+inventoryGridYPos*numItemColumns<v4core->savereader.invdata.ItemsByType(activeCategory).size())
    {
        Item* starting_item = v4core->savereader.invdata.ItemsByType(activeCategory)[inventoryGridXPos+inventoryGridYPos*numItemColumns].item;

        t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(starting_item->item_name)));
        t_itemtitle.setOrigin(t_itemtitle.getLocalBounds().width/2,t_itemtitle.getLocalBounds().height/2);
    }
    else
    {
        t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_none")));
        t_itemtitle.setOrigin(t_itemtitle.getLocalBounds().width/2,t_itemtitle.getLocalBounds().height/2);
    }*/
}

void Barracks::UpdatePreviewText()
{
    int invbox_id = ((gridOffsetY+gridSelY)*4)+gridSelX;

    if(inventory_boxes.size() > 0)
    {
        if(invbox_id < inventory_boxes.size())
        {
            item_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(inventory_boxes[invbox_id].data->item_name)));
            item_desc.setString(Func::wrap_text(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(inventory_boxes[invbox_id].data->item_description)), 340, f_font, 22));
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

void Barracks::Update(sf::RenderWindow &window, float fps, InputController& inputCtrl)
{
    if(isActive)
    {
        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        rs_cover.setSize(sf::Vector2f(1280 * resRatioX, 720 * resRatioY));
        rs_cover.setFillColor(sf::Color::Black);
        rs_cover.setPosition(0,0);
        window.draw(rs_cover);

        s_background.setPosition(0,0);
        s_background.draw(window);

        int highlightWidth=225;
        int ponwidth=75;

        highlighted_pon.setPosition((468 + (75*(current_selected_pon))), 530);
        highlighted_pon.draw(window);

        patapon->setGlobalPosition(sf::Vector2f((highlightWidth*2+ponwidth*0.7),pataponY));
        patapon->fps = fps;
        patapon->Draw(window);

        patapon2->setGlobalPosition(sf::Vector2f((highlightWidth*2+ponwidth*1.7),pataponY));
        patapon2->fps = fps;
        patapon2->Draw(window);

        patapon3->setGlobalPosition(sf::Vector2f((highlightWidth*2+ponwidth*2.7),pataponY));
        patapon3->fps = fps;
        patapon3->Draw(window);

        s_pon_highlight.setPosition(highlightWidth*2,675);
        s_pon_highlight.draw(window);

        rr_main_sh.Create(1100+2, 220+2, 20, window.getSize().x / float(1280), sf::Color(0,0,0,96));
        rr_main_sh.x = 670-1;
        rr_main_sh.y = 190-1;
        rr_main_sh.setOrigin(sf::Vector2f((1100+40)/2, (220+40)/2));
        rr_main_sh.Draw(window);

        rr_uniticon_sh.Create(176+2, 8+2, 34, window.getSize().x / float(1280), sf::Color(0,0,0,96));
        rr_uniticon_sh.x = 188-1;
        rr_uniticon_sh.y = 98-1;
        rr_uniticon_sh.setOrigin(sf::Vector2f((176+68)/2, (8+68)/2));
        rr_uniticon_sh.Draw(window);

        rr_unitstatus_sh.Create(175+2, 20+2, 25, window.getSize().x / float(1280), sf::Color(0,0,0,96));
        rr_unitstatus_sh.x = 1127-1;
        rr_unitstatus_sh.y = 64-1;
        rr_unitstatus_sh.setOrigin(sf::Vector2f((175+50)/2, (12+50)/2));
        rr_unitstatus_sh.Draw(window);

        rr_main.Create(1100, 220, 20, window.getSize().x / float(1280));
        rr_main.x = 670;
        rr_main.y = 190;
        rr_main.setOrigin(sf::Vector2f((1100+40)/2, (220+40)/2));
        rr_main.Draw(window);

        rr_uniticon.Create(176, 8, 34, window.getSize().x / float(1280));
        rr_uniticon.x = 188;
        rr_uniticon.y = 98;
        rr_uniticon.setOrigin(sf::Vector2f((176+68)/2, (8+68)/2));
        rr_uniticon.Draw(window);

        rr_uniticon.Create(175, 20, 25, window.getSize().x / float(1280));
        rr_uniticon.x = 1127;
        rr_uniticon.y = 64;
        rr_uniticon.setOrigin(sf::Vector2f((175+50)/2, (12+50)/2));
        rr_uniticon.Draw(window);

        class_icon.draw(window);

        unit_status.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_unit_status"))+" "+to_string(current_selected_pon+1)+"/3");
        unit_status.setPosition(1048, 38);

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

        if (enabledPositons[0]) /// Rarepon
        {
            t_unit_rarepon_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_unit_icon.getPosition().y+8);
            s_unit_icon.setPosition(s_unit_icon.getPosition().x,s_unit_icon.getPosition().y);
            s_unit_icon.draw(window);
            t_unit_rarepon_name.draw(window);
        }

        if (enabledPositons[1]) /// wep1
        {
            t_weapon_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_weapon_icon.getPosition().y+8);
            s_weapon_icon.setPosition(s_unit_icon.getPosition().x,s_weapon_icon.getPosition().y);
            s_weapon_icon.draw(window);
            t_weapon_name.draw(window);
        }

        if (enabledPositons[2]) /// wep2
        {
            t_weapon2_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_weapon2_icon.getPosition().y+8);
            s_weapon2_icon.setPosition(s_unit_icon.getPosition().x,s_weapon2_icon.getPosition().y);
            s_weapon2_icon.draw(window);
            t_weapon2_name.draw(window);
        }

        if (enabledPositons[3]) /// armour
        {
            t_armour_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_armour_icon.getPosition().y+8);
            s_armour_icon.setPosition(s_unit_icon.getPosition().x,s_armour_icon.getPosition().y);
            s_armour_icon.draw(window);
            t_armour_name.draw(window);
        }

        if (enabledPositons[4]) ///mask
        {
            t_mask_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_mask_icon.getPosition().y+13);
            s_mask_icon.setPosition(s_unit_icon.getPosition().x,s_mask_icon.getPosition().y);
            s_mask_icon.draw(window);
            t_mask_name.draw(window);
        }

        item_line_flash += 8.0 / fps;

        int barSize = 266;
        int bar_offset = 78;
        mm_selected_item_line.setSize(sf::Vector2f(barSize*resRatioX,3*resRatioY));
        mm_selected_item_line.setFillColor(sf::Color(239, 88, 98, 128+(sin(item_line_flash)*128)));
        mm_selected_item_line.setPosition((s_unit_icon.getPosition().x) * resRatioX,((s_unit_icon.getPosition().y+currentItemPosition*50)+47) * resRatioY);

        ctrlTips.x = 0;
        ctrlTips.y = (720-ctrlTips.ySize);
        ctrlTips.draw(window);

        if(MenuMode)
        {
            inv_box.setPosition(40,366);
            inv_box.draw(window);

            for(int i=0; i<16; i++)
            {
                if(gridOffsetY*4 + i < inventory_boxes.size())
                {
                    if(inventory_boxes.size() > 0)
                    {
                        int curItem = gridOffsetY*4 + i;

                        int gridX = i % 4;
                        int gridY = floor(i/4);

                        float xpos = 46 + (gridX*77);
                        float ypos = 37 + (gridY*54);

                        inventory_boxes[curItem].r_outer.setPosition((40 + xpos)*resRatioX, (366 + ypos) * resRatioY);
                        inventory_boxes[curItem].r_inner.setPosition((40 + xpos + 2.5)*resRatioX, (366 + ypos + 2.5) * resRatioY);
                        window.draw(inventory_boxes[curItem].r_outer);
                        window.draw(inventory_boxes[curItem].r_inner);

                        //inventory_boxes[i].num.setOrigin(inventory_boxes[i].num.getLocalBounds().width,inventory_boxes[i].num.getLocalBounds().height);
                        //inventory_boxes[i].num_shadow.setOrigin(inventory_boxes[i].num_shadow.getLocalBounds().width,inventory_boxes[i].num_shadow.getLocalBounds().height);

                        inventory_boxes[curItem].icon.setScale(0.64,0.64);

                        if((inventory_boxes[curItem].data->category_id == 0) || (inventory_boxes[curItem].data->category_id == 4))
                        inventory_boxes[curItem].icon.setScale(0.41,0.41);

                        inventory_boxes[curItem].icon.setPosition(40 + xpos + 23 + 1.5, 366 + ypos + 23 + 1.5);
                        inventory_boxes[curItem].icon.draw(window);

                        inventory_boxes[curItem].num.setPosition(40+xpos+36-1, 366+ypos+29-2);
                        inventory_boxes[curItem].num_shadow.setPosition(40+xpos+36, 366+ypos+29);

                        inventory_boxes[curItem].num_shadow.draw(window);
                        inventory_boxes[curItem].num.draw(window);

                        if(inventory_boxes[curItem].highlight)
                        {
                            inventory_boxes[curItem].r_highlight.setSize({70.0*resRatioX, 51.0*resRatioY});
                            inventory_boxes[curItem].r_highlight.setPosition((40 + xpos)*resRatioX, (366 + ypos) * resRatioY);
                            inventory_boxes[curItem].r_highlight.setFillColor(sf::Color(0,0,0,192));
                            window.draw(inventory_boxes[curItem].r_highlight);
                        }
                    }
                }
                else
                {
                    InvBox tmp_inv;

                    int gridX = i % 4;
                    int gridY = floor(i/4);

                    float xpos = 46 + (gridX*77);
                    float ypos = 37 + (gridY*54);

                    tmp_inv.r_outer.setSize({70.0*resRatioX, 51.0*resRatioY});
                    tmp_inv.r_outer.setFillColor(sf::Color(102,102,102,255));

                    tmp_inv.r_inner.setSize({46.0*resRatioX, 46.0*resRatioY});
                    tmp_inv.r_inner.setFillColor(sf::Color(183,183,183,255));

                    tmp_inv.r_outer.setPosition((40 + xpos)*resRatioX, (366 + ypos) * resRatioY);
                    tmp_inv.r_inner.setPosition((40 + xpos + 2.5)*resRatioX, (366 + ypos + 2.5) * resRatioY);
                    window.draw(tmp_inv.r_outer);
                    window.draw(tmp_inv.r_inner);

                    tmp_inv.r_highlight.setSize({70.0*resRatioX, 51.0*resRatioY});
                    tmp_inv.r_highlight.setPosition((40 + xpos)*resRatioX, (366 + ypos) * resRatioY);
                    tmp_inv.r_highlight.setFillColor(sf::Color(0,0,0,192));
                    window.draw(tmp_inv.r_highlight);
                }
            }

            r_sel.setSize({70.0*resRatioX, 51.0*resRatioY});
            r_sel.setFillColor(sf::Color::Transparent);
            r_sel.setOutlineThickness(2);
            r_sel.setOutlineColor(sf::Color(255,0,32,255));
            r_sel.setPosition((40 + 46 + (gridSelX*77))*resRatioX, (366 + 37 + (gridSelY*54))*resRatioY);

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

        for(int i=0; i<dialogboxes.size(); i++)
        {
            dialogboxes[i].x = 640;
            dialogboxes[i].y = 360;
            dialogboxes[i].Draw(window, fps, inputCtrl);

            if(dialogboxes[i].closed)
            db_e.push_back(i);
        }

        for(int i=0; i<db_e.size(); i++)
        {
            dialogboxes.erase(dialogboxes.begin()+db_e[i]-i);
        }

        window.setView(lastView);

        if(dialogboxes.size() <= 0)
        {
            if(!MenuMode)
            {
                if(inputCtrl.isKeyPressed(InputController::Keys::UP))
                {
                    currentItemPosition -=1;
                    if (currentItemPosition<0){
                        currentItemPosition = enabledPositons.size()-1;
                    }
                    while(!enabledPositons[currentItemPosition]){
                        currentItemPosition -=1;
                    }
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
                {
                    currentItemPosition +=1;

                    if(currentItemPosition > enabledPositons.size()-1)
                    {
                        currentItemPosition = 0;
                    }

                    while(!enabledPositons[currentItemPosition])
                    {
                        currentItemPosition +=1;
                    }

                    if(currentItemPosition > enabledPositons.size()-1)
                    {
                        currentItemPosition = 0;
                    }
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::LEFT))
                {
                    if(current_selected_pon > 0)
                    current_selected_pon--;

                    RefreshStats();
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
                {
                    if(current_selected_pon < 2)
                    current_selected_pon++;

                    RefreshStats();
                }
            }
            else
            {
                if(inputCtrl.isKeyPressed(InputController::Keys::LEFT))
                {
                    gridSelX--;

                    if(gridSelX < 0)
                    gridSelX = 3;

                    UpdatePreviewText();
                }
                if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
                {
                    gridSelX++;

                    if(gridSelX > 3)
                    gridSelX = 0;

                    UpdatePreviewText();
                }
                if(inputCtrl.isKeyPressed(InputController::Keys::UP))
                {
                    gridSelY--;

                    if(gridSelY < 0)
                    {
                        if(gridOffsetY > 0)
                        {
                            gridOffsetY--;
                            gridSelY = 0;
                        }
                        else
                        {
                            gridOffsetY = ceil(inventory_boxes.size() / 4.0) - 4;

                            if(gridOffsetY < 0)
                            gridOffsetY = 0;

                            gridSelY = 3;
                        }
                    }

                    UpdatePreviewText();
                }
                if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
                {
                    gridSelY++;

                    if(gridSelY > 3)
                    {
                        if(inventory_boxes.size() > (4+gridOffsetY)*4)
                        {
                            gridOffsetY++;
                            gridSelY = 3;
                        }
                        else
                        {
                            gridSelY = 0;
                            gridOffsetY = 0;
                        }
                    }

                    UpdatePreviewText();
                }
            }

            if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
            {
                if(!MenuMode)
                {
                    MenuMode = true;
                    UpdateInputControls();

                    if (currentItemPosition == 0)
                    {
                        activeCategory = 5;
                    }
                    else if(currentItemPosition == 1)
                    {
                        activeCategory = 1;
                    }
                    else if(currentItemPosition == 4)
                    {
                        activeCategory = 2;
                    }
                    else if(currentItemPosition == 3)
                    {
                        activeCategory = 3;
                    }

                    ReloadInventory();
                    SetInventoryPosition();
                }
                else
                {
                    ///Item selection was here
                    int invbox_id = ((gridOffsetY+gridSelY)*4)+gridSelX;

                    if(inventory_boxes.size() > 0)
                    {
                        if(invbox_id < inventory_boxes.size())
                        {
                            ///Check if item isn't highlighted, that means it's accessible
                            if(!inventory_boxes[invbox_id].highlight)
                            {
                                ///Can apply the item :)
                                for(int i=0; i<inventory_boxes[invbox_id].invIDs.size(); i++)
                                {
                                    InventoryItem currentItem = v4core->savereader.invdata.GetItemByInvID(inventory_boxes[invbox_id].invIDs[i]);

                                    if(!currentItem.occupied)
                                    {
                                        v4core->savereader.ponreg.pons[current_selected_pon].GiveItem(currentItem.inventoryId);

                                        RefreshStats();
                                        ApplyEquipment();
                                        UpdateInputControls();

                                        MenuMode = false;
                                    }
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
                if (MenuMode)
                {
                    MenuMode=false;

                    UpdateInputControls();
                    RefreshStats();
                }
                else
                {
                    this->Hide();
                    this->isActive = false;
                    parentMenu->Show();
                    parentMenu->isActive=true;
                }
            }

            if(inputCtrl.isKeyPressed(InputController::Keys::START))
            {
                if(obelisk)
                {
                    std::vector<std::string> a = {Func::ConvertToUtf8String(config->strRepo.GetUnicodeString(L"nav_yes")),Func::ConvertToUtf8String(config->strRepo.GetUnicodeString(L"nav_no"))};

                    PataDialogBox db;
                    db.Create(f_font, Func::ConvertToUtf8String(config->strRepo.GetUnicodeString(L"barracks_depart")), a, config->GetInt("textureQuality"));
                    db.id = 0;
                    dialogboxes.push_back(db);
                }
            }
        }
        else
        {
            if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
            {
                switch(dialogboxes[dialogboxes.size()-1].CheckSelectedOption())
                {
                    case 0:
                    {
                        if(dialogboxes[dialogboxes.size()-1].id == 0)
                        {
                            cout << "Go on mission!" << endl;
                            dialogboxes[dialogboxes.size()-1].Close();

                            sf::Thread loadingThreadInstance(v4core->LoadingThread,v4core);
                            v4core->continueLoading=true;
                            v4core->window.setActive(false);
                            loadingThreadInstance.launch();

                            currentController->Initialise(*config,config->GetString("mission1Background"),*v4core);
                            currentController->StartMission(mission_file,1,missionID,mission_multiplier);
                            this->Hide();
                            this->isActive = false;

                            missionStarted = true;

                            v4core->continueLoading=false;

                            break;
                        }
                    }

                    case 1:
                    {
                        cout << "Back to Barracks" << endl;
                        dialogboxes[dialogboxes.size()-1].Close();

                        break;
                    }
                }
            }
        }
    }
}

void Barracks::UpdateInputControls()
{
    if(!MenuMode)
    {
        if(!obelisk)
        ctrlTips.create(82, f_font, 20, sf::String(L"Left/Right: Select unit      Up/Down: Select equipment      ×: Change equipment      〇: Return to Patapolis"), qualitySetting);
        else
        ctrlTips.create(82, f_font, 20, sf::String(L"Left/Right: Select unit      Up/Down: Select equipment      ×: Change equipment      〇: Return to World map      Start: Start mission"), qualitySetting);
    }
    else
    {
        ctrlTips.create(82, f_font, 20, sf::String(L"Left/Right/Up/Down: Navigate item      ×: Equip item      〇: Cancel"), qualitySetting);
    }
}

void Barracks::UpdateButtons()
{
    /// this should update the text on all the buttons
}

void Barracks::OnExit()
{
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}

Barracks::~Barracks()
{
    //dtor
}
