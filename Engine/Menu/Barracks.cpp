#include "Barracks.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
#include <sstream>
#include "Altar.h"

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

    patapon->applySpear(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(1)->weapon_invItem_id).item->equip_id);
    patapon2->applySpear(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(2)->weapon_invItem_id).item->equip_id);
    patapon3->applySpear(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(3)->weapon_invItem_id).item->equip_id);

    patapon->applyHelm(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(1)->armour_invItem_id).item->equip_id);
    patapon2->applyHelm(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(2)->armour_invItem_id).item->equip_id);
    patapon3->applyHelm(parent->savereader.invdata.GetItemByInvID(parent->savereader.ponreg.GetPonByID(3)->armour_invItem_id).item->equip_id);

    int quality = thisConfigs->GetInt("textureQuality");

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

    float resRatioX = thisConfigs->GetInt("resX") / float(1920);
    float resRatioY = thisConfigs->GetInt("resY") / float(1080);

    pataponY = 607;
    floorY = 980;

    mm_bigBox.setSize(sf::Vector2f(100*resRatioX,10*resRatioY));
    mm_bigBox.setFillColor(sf::Color(0, 0, 0));

    mm_selected_item_line.setSize(sf::Vector2f(200*resRatioX,4*resRatioY));
    mm_selected_item_line.setFillColor(sf::Color::Red);

    t_title.createText(f_font, 57, sf::Color::White, "", quality, 2);
    t_itemtitle.createText(f_font, 42, sf::Color::Black, "", quality, 2);

    ///             ####   BARRACKS MENU BACKGROUND
    s_background.loadFromFile("resources/graphics/bg/barracks/barracks.png",quality,2);

    s_menu_bkg.loadFromFile("resources/graphics/ui/barracks_menu.png",quality,2);
    s_menu_bkg.setPosition(50, 100);

    ///         highlighted unit
    s_pon_highlight.loadFromFile("resources/graphics/ui/highlight.png",quality,2);

    ///             ####   UNIT CLASS ICON
    s_class_icon.loadFromFile("resources/graphics/ui/yari_class_icon.png",quality,2);
    s_class_icon.setPosition(75, 125);

    ///             ####   UNIT ITEM ICON
    s_unit_icon.loadFromFile("resources/graphics/ui/unit_icon.png",quality,2);
    s_unit_icon.setPosition(1920-500, s_class_icon.getPosition().y+44);

    int equip_height = 76;

    ///             ####   WEAPON ITEM ICON
    s_weapon_icon.loadFromFile("resources/graphics/ui/sword_weapon_icon.png",quality,2);
    s_weapon_icon.setPosition(1920-500, s_unit_icon.getPosition().y+equip_height);

    ///             ####   WEAPON 2 (OTHER HAND) ITEM ICON
    s_weapon2_icon.loadFromFile("resources/graphics/ui/sword_weapon_icon.png",quality,2);
    s_weapon2_icon.setPosition(1920-500, s_weapon_icon.getPosition().y+equip_height);

    ///             ####   ARMOUR ITEM ICON
    s_armour_icon.loadFromFile("resources/graphics/ui/sword_weapon_icon.png",quality,2);
    s_armour_icon.setPosition(1920-500, s_weapon2_icon.getPosition().y+equip_height);

    ///             ####   MASK ITEM ICON
    s_mask_icon.loadFromFile("resources/graphics/ui/mask_icon.png",quality,2);
    s_mask_icon.setPosition(1920-500, s_armour_icon.getPosition().y+equip_height);

    t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis")));
    t_title.setOrigin(t_title.getLocalBounds().width/2,t_title.getLocalBounds().height/2);

    ///
    /// left half stats
    ///

    t_unit_name.createText(f_font, 50, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_testunitname")), quality, 2);
    t_unit_name.setOrigin(0,t_unit_name.getLocalBounds().height/2);

    t_unit_status_text.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_unitstatus")), quality, 2);
    t_unit_status_text.setOrigin(0,t_unit_status_text.getLocalBounds().height/2);

    t_unit_level.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_lvl")), quality, 2);
    t_unit_level.setOrigin(0,t_unit_level.getLocalBounds().height/2);

    t_unit_experience.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_exp")), quality, 2);
    t_unit_experience.setOrigin(0,t_unit_experience.getLocalBounds().height/2);

    t_unit_damage.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_dmg")), quality, 2);
    t_unit_damage.setOrigin(0,t_unit_damage.getLocalBounds().height/2);

    /// left half stat values

    s_unit_level.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")), quality, 2);
    s_unit_level.setOrigin(s_unit_level.getLocalBounds().width,s_unit_level.getLocalBounds().height/2);

    s_unit_experience.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")), quality, 2);
    s_unit_experience.setOrigin(s_unit_experience.getLocalBounds().width,s_unit_experience.getLocalBounds().height/2);

    s_unit_damage.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")), quality, 2);
    s_unit_damage.setOrigin(s_unit_damage.getLocalBounds().width,s_unit_damage.getLocalBounds().height/2);

    ///
    /// right half stats
    ///

    t_unit_status_text.createText(f_font, 33, sf::Color::Red, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_unitstatus")), quality, 2);
    t_unit_status_text.setOrigin(t_unit_status_text.getLocalBounds().width,t_unit_status_text.getLocalBounds().height/2);

    t_unit_hp.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_hp")), quality, 2);
    t_unit_hp.setOrigin(0,t_unit_hp.getLocalBounds().height/2);

    t_unit_crit.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_crit")), quality, 2);
    t_unit_crit.setOrigin(0,t_unit_crit.getLocalBounds().height/2);

    t_unit_attack_speed.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_attack_speed")), quality, 2);
    t_unit_attack_speed.setOrigin(0,t_unit_attack_speed.getLocalBounds().height/2);

    /// right half stat values

    s_unit_hp.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")), quality, 2);
    s_unit_hp.setOrigin(s_unit_hp.getLocalBounds().width,s_unit_hp.getLocalBounds().height/2);

    s_unit_crit.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")), quality, 2);
    s_unit_crit.setOrigin(s_unit_crit.getLocalBounds().width,s_unit_crit.getLocalBounds().height/2);

    s_unit_attack_speed.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")), quality, 2);
    s_unit_attack_speed.setOrigin(s_unit_attack_speed.getLocalBounds().width,s_unit_attack_speed.getLocalBounds().height/2);


    /// unit + item name text

    t_unit_rarepon_name.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 2);
    t_unit_rarepon_name.setOrigin(0,t_unit_rarepon_name.getLocalBounds().height/2);

    t_weapon_name.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 2);
    t_weapon_name.setOrigin(0,t_weapon_name.getLocalBounds().height/2);

    t_weapon2_name.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 2);
    t_weapon2_name.setOrigin(0,t_weapon2_name.getLocalBounds().height/2);

    t_armour_name.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 2);
    t_armour_name.setOrigin(0,t_armour_name.getLocalBounds().height/2);

    t_mask_name.createText(f_font, 33, sf::Color::Black, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")), quality, 2);
    t_mask_name.setOrigin(0,t_mask_name.getLocalBounds().height/2);

    possibleMenuPositions = {-2300, -1300,600,1500,3700};
    currentMenuPosition = -1;
    enabledPositons.push_back(true);
    enabledPositons.push_back(true);
    enabledPositons.push_back(false);
    enabledPositons.push_back(true);
    enabledPositons.push_back(false);
    mm_inventory_background.setSize(sf::Vector2f(496*resRatioX,360*resRatioY));
    mm_inventory_background.setFillColor(sf::Color(0,0,0));

    mm_description_background.setSize(sf::Vector2f(496*resRatioX,360*resRatioY));
    mm_description_background.setFillColor(sf::Color(220,220,220,180));

    float singleTileWidth = 122;
    float singleTileHeight = 88; //(thisConfig->GetInt("resX")*150)/1920.0;

    mm_highlighted_tile.setSize(sf::Vector2f(singleTileWidth*resRatioX,singleTileHeight*resRatioY));
    mm_highlighted_tile.setFillColor(sf::Color(255,255,255,80));

    ///             ####   MASK ITEM ICON
    mask_icon.loadFromFile("resources/graphics/item/mask_item.png",1,2);

    ///             ####   SPEAR ITEM ICON
    spear_icon.loadFromFile("resources/graphics/item/spear_item.png",1,2);

    ///             ####   MISC ITEM ICON
    misc_icon.loadFromFile("resources/graphics/item/misc_icon.png",1,2);

    ///             ####   ARMOUR ITEM ICON
    armour_icon.loadFromFile("resources/graphics/item/armour_icon.png",1,2);

    qualitySetting = quality;
    highlighted_pon.loadFromFile("resources/graphics/ui/highlighted_pon.png", qualitySetting, 1);

    ApplyEquipment();

    //mm_inventory_background.setSize(sf::Vector2f(mm_inventory_background.getSize().x+(40*resRatioX),mm_inventory_background.getSize().y+(40*resRatioX)));
    parent->SaveToDebugLog("Initializing Barracks finished.");

}
void Barracks::EventFired(sf::Event event){
    if(isActive)
    {
        if(event.type == sf::Event::KeyPressed)
        {


        } else if (event.type == sf::Event::MouseButtonReleased){
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        }
    }

}
void Barracks::OpenBarracksMenu(){
    int egg;
    Pon* currentPon = parentMenu->v4core->savereader.ponreg.GetPonByID(current_selected_pon);
    if (currentItemPosition==1) {
        egg = parentMenu->v4core->savereader.invdata.InvItemIdFromType(1,currentPon->weapon_invItem_id);
    } else if (currentItemPosition==2){
        egg = parentMenu->v4core->savereader.invdata.InvItemIdFromType(1,currentPon->weapon2_invItem_id);
    } else if(currentItemPosition==4) {
        egg = parentMenu->v4core->savereader.invdata.InvItemIdFromType(2,currentPon->mask_invItem_id);
    } else if(currentItemPosition==3) {
        egg = parentMenu->v4core->savereader.invdata.InvItemIdFromType(3,currentPon->armour_invItem_id);
    }
    cout<<egg<<'\n';
    inventoryGridXPos=egg%4;
    inventoryGridYPos=floor(egg/4);
    int totalItems = v4core->savereader.invdata.CountItemsByType(activeCategory);
    numItemRows = ceil(totalItems/(numItemColumns+0.0));
    RefreshStats();
    ApplyEquipment();
}

void Barracks::ApplyEquipment()
{
    patapon->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(1)->weapon_invItem_id).item->equip_id);
    patapon2->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(2)->weapon_invItem_id).item->equip_id);
    patapon3->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(3)->weapon_invItem_id).item->equip_id);

    patapon->applyHelm(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(1)->armour_invItem_id).item->equip_id);
    patapon2->applyHelm(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(2)->armour_invItem_id).item->equip_id);
    patapon3->applyHelm(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(3)->armour_invItem_id).item->equip_id);
}

void Barracks::RefreshStats()
{
    Pon* currentPon = parentMenu->v4core->savereader.ponreg.GetPonByID(current_selected_pon);

    t_unit_rarepon_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"rarepon_normal"))+" "+Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_lvl"))+" "+std::to_string(currentPon->pon_level));

    if(currentPon->weapon_invItem_id>=0){
        InventoryItem wep = parentMenu->v4core->savereader.invdata.GetItemByInvID(currentPon->weapon_invItem_id);
        t_weapon_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(wep.item->item_name)));
    }
    if(currentPon->weapon2_invItem_id>=0){
        InventoryItem wep2 = parentMenu->v4core->savereader.invdata.GetItemByInvID(currentPon->weapon2_invItem_id);
         t_weapon2_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(wep2.item->item_name)));
    }
    if(currentPon->armour_invItem_id>=0){
        InventoryItem armour = parentMenu->v4core->savereader.invdata.GetItemByInvID(currentPon->armour_invItem_id);
        t_armour_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(armour.item->item_name)));
    }
    if(currentPon->mask_invItem_id>=0){
        InventoryItem mask = parentMenu->v4core->savereader.invdata.GetItemByInvID(currentPon->mask_invItem_id);
        t_mask_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(mask.item->item_name)));
    }


    s_unit_level.setString(std::to_string(currentPon->pon_level));
    s_unit_experience.setString(std::to_string(currentPon->pon_exp));
    s_unit_experience.setString(std::to_string(currentPon->pon_exp));
    s_unit_damage.setString(std::to_string(currentPon->pon_min_dmg)+"/"+std::to_string(currentPon->pon_max_dmg));
    s_unit_hp.setString(std::to_string(currentPon->pon_hp));
    s_unit_crit.setString(std::to_string(currentPon->pon_crit));
    s_unit_attack_speed.setString(std::to_string(currentPon->pon_attack_speed));


    if (inventoryGridXPos+inventoryGridYPos*numItemColumns<v4core->savereader.invdata.ItemsByType(activeCategory).size())
    {
        Item* starting_item = v4core->savereader.invdata.ItemsByType(activeCategory)[inventoryGridXPos+inventoryGridYPos*numItemColumns].item;

        t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(starting_item->item_name)));
        t_itemtitle.setOrigin(t_itemtitle.getLocalBounds().width/2,t_itemtitle.getLocalBounds().height/2);


        /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
        /*
        std::vector<std::wstring> wordsinDesc = Func::Split(thisConfig->strRepo.GetUnicodeString(starting_item->item_description),' ');
        sf::String oldTotalString;
        sf::String currentTotalString;
        int maxWidth = mm_description_background.getSize().x * 0.9;
        /// we split it into words, then go word by word testing the width of the string
        t_itemdescription.clear();
        for (int i=0;i<wordsinDesc.size();i++){
            std::wstring currentWord = wordsinDesc[i];
            currentTotalString = currentTotalString + Func::ConvertToUtf8String(currentWord) + L" ";
            sf::Text t_newLine;
            t_newLine.setFont(f_font);
            t_newLine.setCharacterSize(16);
            t_newLine.setFillColor(sf::Color::Black);
            t_newLine.setString(currentTotalString);
            if (t_newLine.getLocalBounds().width>maxWidth){
                /// when the string is too long, we go back to the last string and lock it in, then start a new line
                currentTotalString = oldTotalString;
                t_newLine.setString(currentTotalString);
                t_newLine.setOrigin(t_newLine.getLocalBounds().width/2,t_newLine.getLocalBounds().height/2);
                t_itemdescription.push_back(t_newLine);
                oldTotalString = currentWord+L" ";
                currentTotalString = currentWord+L" ";
            }
            oldTotalString = currentTotalString;
            /// if there are no more words, finish up the current line
            if (i+1==wordsinDesc.size()){
                currentTotalString = oldTotalString;
                t_newLine.setString(currentTotalString);
                t_newLine.setOrigin(t_newLine.getLocalBounds().width/2,t_newLine.getLocalBounds().height/2);
                t_itemdescription.push_back(t_newLine);
                oldTotalString = "";
                currentTotalString = "";
            }
        }
        //s_menu_bkg.t.getSize().x*0.2*/
    }
    else
    {
        t_itemtitle.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"item_none")));
        t_itemtitle.setOrigin(t_itemtitle.getLocalBounds().width/2,t_itemtitle.getLocalBounds().height/2);

        /*
        /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
        std::vector<std::wstring> wordsinDesc = Func::Split(thisConfig->strRepo.GetUnicodeString(L"desc_none"),' ');
        sf::String oldTotalString;
        sf::String currentTotalString;
        int maxWidth = mm_description_background.getSize().x * 0.9;
        /// we split it into words, then go word by word testing the width of the string
        t_itemdescription.clear();
        for (int i=0;i<wordsinDesc.size();i++){
            std::wstring currentWord = wordsinDesc[i];
            currentTotalString = currentTotalString + Func::ConvertToUtf8String(currentWord) + L" ";
            sf::Text t_newLine;
            t_newLine.setFont(f_font);
            t_newLine.setCharacterSize(16);
            t_newLine.setFillColor(sf::Color::Black);
            t_newLine.setString(currentTotalString);
            if (t_newLine.getLocalBounds().width>maxWidth){
                /// when the string is too long, we go back to the last string and lock it in, then start a new line
                currentTotalString = oldTotalString;
                t_newLine.setString(currentTotalString);
                t_newLine.setOrigin(t_newLine.getLocalBounds().width/2,t_newLine.getLocalBounds().height/2);
                t_itemdescription.push_back(t_newLine);
                oldTotalString = currentWord+L" ";
                currentTotalString = currentWord+L" ";
            }
            oldTotalString = currentTotalString;
            /// if there are no more words, finish up the current line
            if (i+1==wordsinDesc.size()){
                currentTotalString = oldTotalString;
                t_newLine.setString(currentTotalString);
                t_newLine.setOrigin(t_newLine.getLocalBounds().width/2,t_newLine.getLocalBounds().height/2);
                t_itemdescription.push_back(t_newLine);
                oldTotalString = "";
                currentTotalString = "";
            }
        }*/
    }
}
void Barracks::Update(sf::RenderWindow &window, float fps, InputController& inputCtrl)
{
    if(isActive)
    {
        float resRatioX = window.getSize().x / float(1920);
        float resRatioY = window.getSize().y / float(1080);

        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        rs_cover.setSize(sf::Vector2f(1920 * resRatioX, 1080 * resRatioY));
        rs_cover.setFillColor(sf::Color::Black);
        rs_cover.setPosition(0,0);
        window.draw(rs_cover);

        //mm_bigBox.setSize(sf::Vector2f(window.getSize().x,window.getSize().y-115));

        //mm_bigBox.setPosition(0,0);

        //window.draw(mm_bigBox);

        s_background.setPosition(0,0);
        s_background.draw(window);

        int highlightWidth=225;
        int ponwidth=75;

        highlighted_pon.setPosition((468 + (75*(current_selected_pon-1))), 530);
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

        s_menu_bkg.draw(window);
        s_class_icon.draw(window);

        //t_title.setPosition(window.getSize().x/2,110);
        //window.draw(t_title);

        float leftStatNameTextX = s_menu_bkg.getPosition().x + 170;
        float gap = 400;

        float leftStatValueTextX = leftStatNameTextX + gap;

        float rightStatNameTextX = leftStatNameTextX + gap + 170;
        float rightStatValueTextX = leftStatNameTextX + gap + 170 + gap;

        /// left stats text

        t_unit_name.setPosition(leftStatNameTextX,s_class_icon.getPosition().y+2);
        t_unit_level.setPosition(leftStatNameTextX,s_class_icon.getPosition().y+60);
        t_unit_experience.setPosition(leftStatNameTextX,s_class_icon.getPosition().y+95);
        t_unit_damage.setPosition(leftStatNameTextX,s_class_icon.getPosition().y+130);
        t_unit_name.draw(window);
        t_unit_level.draw(window);
        t_unit_experience.draw(window);
        t_unit_damage.draw(window);

        /// left stats values

        s_unit_level.setPosition(leftStatValueTextX,s_class_icon.getPosition().y+60);
        s_unit_experience.setPosition(leftStatValueTextX,s_class_icon.getPosition().y+95);
        s_unit_damage.setPosition(leftStatValueTextX,s_class_icon.getPosition().y+130);
        s_unit_level.draw(window);
        s_unit_experience.draw(window);
        s_unit_damage.draw(window);

        /// right stats text
        t_unit_status_text.setPosition(1920-280,s_class_icon.getPosition().y-15);

        t_unit_hp.setPosition(rightStatNameTextX,s_class_icon.getPosition().y+60);
        t_unit_crit.setPosition(rightStatNameTextX,s_class_icon.getPosition().y+95);
        t_unit_attack_speed.setPosition(rightStatNameTextX,s_class_icon.getPosition().y+130);

        t_unit_status_text.draw(window);
        t_unit_hp.draw(window);
        t_unit_crit.draw(window);
        t_unit_attack_speed.draw(window);

        /// right stats values

        s_unit_hp.setPosition(rightStatValueTextX,s_class_icon.getPosition().y+60);
        s_unit_crit.setPosition(rightStatValueTextX,s_class_icon.getPosition().y+95);
        s_unit_attack_speed.setPosition(rightStatValueTextX,s_class_icon.getPosition().y+130);

        s_unit_hp.draw(window);
        s_unit_crit.draw(window);
        s_unit_attack_speed.draw(window);

        /// equipped item + unit name text
        int equip_height = 80;
        if (enabledPositons[0]){  /// unit name
            t_unit_rarepon_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_unit_icon.getPosition().y+13);
            s_unit_icon.setPosition(s_unit_icon.getPosition().x,s_unit_icon.getPosition().y);
            s_unit_icon.draw(window);
            t_unit_rarepon_name.draw(window);
        }
        if (enabledPositons[1]){  /// wep1
            t_weapon_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_weapon_icon.getPosition().y+13);
            s_weapon_icon.setPosition(s_unit_icon.getPosition().x,s_weapon_icon.getPosition().y);
            s_weapon_icon.draw(window);
            t_weapon_name.draw(window);
        }
        if (enabledPositons[2]){  /// wep2
            t_weapon2_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_weapon2_icon.getPosition().y+13);
            s_weapon2_icon.setPosition(s_unit_icon.getPosition().x,s_weapon2_icon.getPosition().y);
            s_weapon2_icon.draw(window);
            t_weapon2_name.draw(window);
        }
        if (enabledPositons[3]){  /// armour
            t_armour_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_armour_icon.getPosition().y+13);
            s_armour_icon.setPosition(s_unit_icon.getPosition().x,s_armour_icon.getPosition().y);
            s_armour_icon.draw(window);
            t_armour_name.draw(window);
        }
        if (enabledPositons[4]){  /// mask
            t_mask_name.setPosition(s_unit_icon.getPosition().x+equip_height,s_mask_icon.getPosition().y+13);
            s_mask_icon.setPosition(s_unit_icon.getPosition().x,s_mask_icon.getPosition().y);
            s_mask_icon.draw(window);
            t_mask_name.draw(window);
        }
        int barSize = 400;
        int bar_offset = 78;
        mm_selected_item_line.setSize(sf::Vector2f(barSize*resRatioX,4*resRatioY));
        mm_selected_item_line.setPosition((s_unit_icon.getPosition().x) * resRatioX,((s_unit_icon.getPosition().y+currentItemPosition*76)+71) * resRatioY);


        if(MenuMode){
        float smallOffset = 100;
        float tinyOffset = 20;
        mm_inventory_background.setPosition(s_menu_bkg.getPosition().x * resRatioX, 610 * resRatioY);
        window.draw(mm_inventory_background);
        int row_start = currentRow;
        int row_end = row_start+4;
        for (int y=0;y<numItemRows;y++){
            for (int x=0;x<numItemColumns;x++){
                int currentItemId = x+y*numItemColumns;
                if (currentItemId<v4core->savereader.invdata.ItemsByType(activeCategory).size() && (currentItemId/numItemColumns)>=row_start && (currentItemId/numItemColumns)<=row_end){
                    Item* starting_item = v4core->savereader.invdata.ItemsByType(activeCategory)[currentItemId].item;
                    switch (starting_item->category_id){

                    case 1:
                        spear_icon.setPosition(4+s_menu_bkg.getPosition().x+(mm_highlighted_tile.getSize().x/resRatioX)*x,4+(mm_inventory_background.getPosition().y / resRatioY)+(mm_highlighted_tile.getSize().y/resRatioY)*y-(mm_highlighted_tile.getSize().y/resRatioY)*currentRow);
                        spear_icon.draw(window);
                        break;
                    case 2:
                        mask_icon.setPosition(4+s_menu_bkg.getPosition().x+(mm_highlighted_tile.getSize().x/resRatioX)*x,4+(mm_inventory_background.getPosition().y / resRatioY)+(mm_highlighted_tile.getSize().y/resRatioY)*y-(mm_highlighted_tile.getSize().y/resRatioY)*currentRow);
                        mask_icon.draw(window);
                        break;
                    case 3:
                        armour_icon.setPosition(4+s_menu_bkg.getPosition().x+(mm_highlighted_tile.getSize().x/resRatioX)*x,4+(mm_inventory_background.getPosition().y / resRatioY)+(mm_highlighted_tile.getSize().y/resRatioY)*y-(mm_highlighted_tile.getSize().y/resRatioY)*currentRow);
                        armour_icon.draw(window);
                        break;
                    case 0:
                    default:
                        misc_icon.setPosition(4+s_menu_bkg.getPosition().x+(mm_highlighted_tile.getSize().x/resRatioX)*x,4+(mm_inventory_background.getPosition().y / resRatioY)+(mm_highlighted_tile.getSize().y/resRatioY)*y-(mm_highlighted_tile.getSize().y/resRatioY)*currentRow);
                        misc_icon.draw(window);
                        break;
                    }
                }
            }
        }

        mm_description_background.setPosition(((s_menu_bkg.getPosition().x + 20) * resRatioX) + (mm_inventory_background.getSize().x/resRatioX) * resRatioX, 610 * resRatioY);
        window.draw(mm_description_background);


        mm_highlighted_tile.setPosition((4+s_menu_bkg.getPosition().x+(mm_highlighted_tile.getSize().x/resRatioX)*inventoryGridXPos) * resRatioX,(4+(mm_inventory_background.getPosition().y/resRatioY)+(mm_highlighted_tile.getSize().y/resRatioY)*inventoryGridYPos-(mm_highlighted_tile.getSize().y/resRatioY)*currentRow)*resRatioY);
        window.draw(mm_highlighted_tile);


        t_itemtitle.setPosition((mm_description_background.getPosition().x/resRatioX)+(mm_description_background.getSize().x/resRatioX)/2,(mm_description_background.getPosition().y/resRatioY)+(mm_description_background.getSize().y/resRatioY)*1/8);
        t_itemtitle.setOrigin(t_itemtitle.getLocalBounds().width/2,t_itemtitle.getLocalBounds().height/2);
        t_itemtitle.draw(window);

        for (int i=0;i<t_itemdescription.size();i++){
            //sf::Text currentLine = t_itemdescription[i];

            //currentLine.setPosition(mm_description_background.getPosition().x+(mm_description_background.getSize().x)/2,mm_description_background.getPosition().y+((mm_description_background.getSize().y)*1/8)+28 + 22*i);

            //window.draw(currentLine);
        }
        }

        window.draw(mm_selected_item_line);

        ctrlTips.x = 0;
        ctrlTips.y = (720-ctrlTips.ySize);
        ctrlTips.draw(window);

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
                    if(current_selected_pon > 1)
                    current_selected_pon--;

                    RefreshStats();
                }

                if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
                {
                    if(current_selected_pon < 3)
                    current_selected_pon++;

                    RefreshStats();
                }
            }
            else
            {
                if(inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
                {
                    /// we need to move to the right
                    if (inventoryGridXPos<numItemColumns-1){
                        inventoryGridXPos+=1;
                    } else {
                        inventoryGridXPos = 0;
                    }
                }
                else if(inputCtrl.isKeyPressed(InputController::Keys::LEFT))
                {
                    /// we need to move to the left
                    if (inventoryGridXPos>0){
                        inventoryGridXPos-=1;
                    } else {
                        inventoryGridXPos = numItemColumns-1;
                    }

                }
                else if(inputCtrl.isKeyPressed(InputController::Keys::UP))
                {
                    /// we need to move up
                    if (inventoryGridYPos>0){

                        inventoryGridYPos-=1;
                        if (inventoryGridYPos - currentRow<0){
                                /// the position is above the top of the grid so we scroll up
                                currentRow-=1;
                        }
                    } else {
                        inventoryGridYPos = numItemRows-1;
                        if (numItemRows<5){
                            currentRow=inventoryGridYPos-numItemRows+1;
                        } else {
                            currentRow=inventoryGridYPos-4;
                        }
                    }

                }
                else if(inputCtrl.isKeyPressed(InputController::Keys::DOWN))
                {
                    /// we need to move down
                    if (inventoryGridYPos<numItemRows-1){
                        inventoryGridYPos+=1;
                        if (inventoryGridYPos - currentRow>=5){
                            /// the position is below the bottom of the grid so we scroll down
                            currentRow+=1;
                        }
                    } else {
                        inventoryGridYPos = 0;
                        currentRow=0;
                    }
                }

                RefreshStats();
            }
            if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
            {
                if (!MenuMode)
                {
                    MenuMode=true;

                    if (currentItemPosition==1 || currentItemPosition==2)
                    {
                        activeCategory=1;
                    }
                    else if(currentItemPosition==4)
                    {
                        activeCategory=2;
                    }
                    else if(currentItemPosition==3)
                    {
                        activeCategory=3;
                    }
                    OpenBarracksMenu();

                }
                else
                {
                    cout << "Apply item from coords: " << inventoryGridXPos << " " << inventoryGridYPos/numItemColumns << " (" << inventoryGridYPos << ", " << numItemColumns << ")" << endl;

                    InventoryItem currentItem = v4core->savereader.invdata.ItemsByType(activeCategory)[inventoryGridXPos+inventoryGridYPos*numItemColumns];
                    v4core->savereader.ponreg.pons[current_selected_pon].GiveItem(currentItem.inventoryId);
                    RefreshStats();
                    ApplyEquipment();
                    MenuMode=false;
                }
            }
            if(inputCtrl.isKeyPressed(InputController::Keys::CIRCLE))
            {
                if (MenuMode)
                {
                    MenuMode=false;

                    RefreshStats();
                }
                else
                {
                    this->Hide();
                    this->isActive = false;
                    parentMenu->Show();
                    parentMenu->isActive=true;
                    v4core->savereader.Save();
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
                            currentController->StartMission(mission_file,1,missionID);
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
    if(!obelisk)
    ctrlTips.create(82, f_font, 20, sf::String(L"Left/Right: Navigate      ×: Change class      △: Equip      〇: Return to Patapolis"), qualitySetting);
    else
    ctrlTips.create(82, f_font, 20, sf::String(L"Left/Right: Navigate      ×: Change class      △: Equip      〇: Return to World map      Start: Start mission"), qualitySetting);
}

void Barracks::UpdateButtons(){
    /// this should update the text on all the buttons
}
void Barracks::OnExit(){
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
Barracks::~Barracks()
{
    //dtor
}
