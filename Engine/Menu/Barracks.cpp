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
    t_title.setFont(f_font);
    t_title.setCharacterSize(38);
    t_title.setFillColor(sf::Color::White);

    mm_bigBox.setSize(sf::Vector2f(100,10));
    mm_bigBox.setFillColor(sf::Color(4,0,90));

    mm_selected_item_line.setSize(sf::Vector2f(200,4));
    mm_selected_item_line.setFillColor(sf::Color::Red);
    isActive=false;
}
void Barracks::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent, Menu *curParentMenu){
    Scene::Initialise(thisConfigs,keymap,parent);
    parentMenu = curParentMenu;


    ///             ####   BARRACKS MENU BACKGROUND
    PSprite ps_temp;
    ps_temp.loadFromFile("resources/graphics/ui/barracks_menu.png",1);
    ps_temp.setRepeated(false);
    ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(0,0);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(0,0);
    ps_temp.DoAutoScale = false;
    sf::Vector2f tmpp;
    tmpp.x = (thisConfig->GetInt("resX")*50.0)/1920.0;//(thisConfig->GetInt("resX")*1920.0)/1920.0-1200;
    tmpp.y = (thisConfig->GetInt("resY")*100.0)/1080.0;//(thisConfig->GetInt("resY")*400.0)/1080.0;
    s_menu_bkg = ps_temp;
    p_menu_bkg = tmpp;
    s_menu_bkg.scaleX=0.7f;
    s_menu_bkg.scaleY=0.6f;

    ///             ####   UNIT CLASS ICON
    ps_temp.loadFromFile("resources/graphics/ui/yari_class_icon.png",1);
    ps_temp.setRepeated(false);
    ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(0,0);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(0,0);
    ps_temp.DoAutoScale = false;
    tmpp.x = (thisConfig->GetInt("resX")*75.0)/1920.0;//(thisConfig->GetInt("resX")*1920.0)/1920.0-1200;
    tmpp.y = (thisConfig->GetInt("resY")*125.0)/1080.0;//(thisConfig->GetInt("resY")*400.0)/1080.0;
    s_class_icon = ps_temp;
    p_class_icon = tmpp;
    s_class_icon.scaleX=0.25f;
    s_class_icon.scaleY=0.25f;

    ///             ####   UNIT ITEM ICON
    ps_temp.loadFromFile("resources/graphics/ui/unit_icon.png",1);
    ps_temp.setRepeated(false);
    ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(ps_temp.t.getSize().x/2,ps_temp.t.getSize().y/2);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(0,0);
    ps_temp.DoAutoScale = false;
    tmpp.x = (thisConfig->GetInt("resX")*1920.0)/1920.0 - (550*thisConfig->GetInt("resX"))/1920.0;
    tmpp.y = p_class_icon.y+44;//(thisConfig->GetInt("resY")*400.0)/1080.0;
    s_unit_icon = ps_temp;
    p_unit_icon = tmpp;
    s_unit_icon.scaleX=0.15f;
    s_unit_icon.scaleY=0.15f;

    ///             ####   WEAPON ITEM ICON
    ps_temp.loadFromFile("resources/graphics/ui/sword_weapon_icon.png",1);
    ps_temp.setRepeated(false);
    ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(ps_temp.t.getSize().x/2,ps_temp.t.getSize().y/2);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(0,0);
    ps_temp.DoAutoScale = false;
    tmpp.x = (thisConfig->GetInt("resX")*1920.0)/1920.0 - (550*thisConfig->GetInt("resX"))/1920.0;
    tmpp.y = p_unit_icon.y+68;//(thisConfig->GetInt("resY")*400.0)/1080.0;
    s_weapon_icon = ps_temp;
    p_weapon_icon = tmpp;
    s_weapon_icon.scaleX=0.15f;
    s_weapon_icon.scaleY=0.15f;

    ///             ####   WEAPON 2 (OTHER HAND) ITEM ICON
    ps_temp.loadFromFile("resources/graphics/ui/sword_weapon_icon.png",1);
    ps_temp.setRepeated(false);
    ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(ps_temp.t.getSize().x/2,ps_temp.t.getSize().y/2);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(0,0);
    ps_temp.DoAutoScale = false;
    tmpp.x = (thisConfig->GetInt("resX")*1920.0)/1920.0 - (550*thisConfig->GetInt("resX"))/1920.0;
    tmpp.y = p_weapon_icon.y+68;//(thisConfig->GetInt("resY")*400.0)/1080.0;
    s_weapon2_icon = ps_temp;
    p_weapon2_icon = tmpp;
    s_weapon2_icon.scaleX=0.15f;
    s_weapon2_icon.scaleY=0.15f;

    ///             ####   ARMOUR ITEM ICON
    ps_temp.loadFromFile("resources/graphics/ui/sword_weapon_icon.png",1);
    ps_temp.setRepeated(false);
    ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(ps_temp.t.getSize().x/2,ps_temp.t.getSize().y/2);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(0,0);
    ps_temp.DoAutoScale = false;
    tmpp.x = (thisConfig->GetInt("resX")*1920.0)/1920.0 - (550*thisConfig->GetInt("resX"))/1920.0;
    tmpp.y = p_weapon2_icon.y+68;//(thisConfig->GetInt("resY")*400.0)/1080.0;
    s_armour_icon = ps_temp;
    p_armour_icon = tmpp;
    s_armour_icon.scaleX=0.15f;
    s_armour_icon.scaleY=0.15f;

    ///             ####   MASK ITEM ICON
    ps_temp.loadFromFile("resources/graphics/ui/mask_icon.png",1);
    ps_temp.setRepeated(false);
    ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(ps_temp.t.getSize().x/2,ps_temp.t.getSize().y/2);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(0,0);
    ps_temp.DoAutoScale = false;
    tmpp.x = (thisConfig->GetInt("resX")*1920.0)/1920.0 - (550*thisConfig->GetInt("resX"))/1920.0;
    tmpp.y = p_armour_icon.y+68;//(thisConfig->GetInt("resY")*400.0)/1080.0;
    s_mask_icon = ps_temp;
    p_mask_icon = tmpp;
    s_mask_icon.scaleX=0.15f;
    s_mask_icon.scaleY=0.15f;


    t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis")));
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);

    ///
    /// left half stats
    ///

    t_unit_name.setFont(f_font);
    t_unit_name.setCharacterSize(33);
    t_unit_name.setFillColor(sf::Color::Black);
    t_unit_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_testunitname")));
    t_unit_name.setOrigin(0,t_unit_name.getGlobalBounds().height/2);

    t_unit_status_text.setFont(f_font);
    t_unit_status_text.setCharacterSize(22);
    t_unit_status_text.setFillColor(sf::Color::Black);
    t_unit_status_text.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_unitstatus")));
    t_unit_status_text.setOrigin(0,t_unit_status_text.getGlobalBounds().height/2);

    t_unit_level.setFont(f_font);
    t_unit_level.setCharacterSize(22);
    t_unit_level.setFillColor(sf::Color::Black);
    t_unit_level.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_lvl")));
    t_unit_level.setOrigin(0,t_unit_level.getGlobalBounds().height/2);

    t_unit_experience.setFont(f_font);
    t_unit_experience.setCharacterSize(22);
    t_unit_experience.setFillColor(sf::Color::Black);
    t_unit_experience.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_exp")));
    t_unit_experience.setOrigin(0,t_unit_experience.getGlobalBounds().height/2);

    t_unit_damage.setFont(f_font);
    t_unit_damage.setCharacterSize(22);
    t_unit_damage.setFillColor(sf::Color::Black);
    t_unit_damage.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_dmg")));
    t_unit_damage.setOrigin(0,t_unit_damage.getGlobalBounds().height/2);

    /// left half stat values

    s_unit_level.setFont(f_font);
    s_unit_level.setCharacterSize(22);
    s_unit_level.setFillColor(sf::Color::Black);
    s_unit_level.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")));
    s_unit_level.setOrigin(s_unit_level.getGlobalBounds().width,s_unit_level.getGlobalBounds().height/2);

    s_unit_experience.setFont(f_font);
    s_unit_experience.setCharacterSize(22);
    s_unit_experience.setFillColor(sf::Color::Black);
    s_unit_experience.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")));
    s_unit_experience.setOrigin(s_unit_experience.getGlobalBounds().width,s_unit_experience.getGlobalBounds().height/2);

    s_unit_damage.setFont(f_font);
    s_unit_damage.setCharacterSize(22);
    s_unit_damage.setFillColor(sf::Color::Black);
    s_unit_damage.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")));
    s_unit_damage.setOrigin(s_unit_damage.getGlobalBounds().width,s_unit_damage.getGlobalBounds().height/2);

    ///
    /// right half stats
    ///

    t_unit_status_text.setFont(f_font);
    t_unit_status_text.setCharacterSize(22);
    t_unit_status_text.setFillColor(sf::Color::Red);
    t_unit_status_text.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_unitstatus")));
    t_unit_status_text.setOrigin(t_unit_status_text.getGlobalBounds().width,t_unit_status_text.getGlobalBounds().height/2);

    t_unit_hp.setFont(f_font);
    t_unit_hp.setCharacterSize(22);
    t_unit_hp.setFillColor(sf::Color::Black);
    t_unit_hp.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_hp")));
    t_unit_hp.setOrigin(0,t_unit_hp.getGlobalBounds().height/2);

    t_unit_crit.setFont(f_font);
    t_unit_crit.setCharacterSize(22);
    t_unit_crit.setFillColor(sf::Color::Black);
    t_unit_crit.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_crit")));
    t_unit_crit.setOrigin(0,t_unit_crit.getGlobalBounds().height/2);

    t_unit_attack_speed.setFont(f_font);
    t_unit_attack_speed.setCharacterSize(22);
    t_unit_attack_speed.setFillColor(sf::Color::Black);
    t_unit_attack_speed.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_attack_speed")));
    t_unit_attack_speed.setOrigin(0,t_unit_attack_speed.getGlobalBounds().height/2);

    /// right half stat values

    s_unit_hp.setFont(f_font);
    s_unit_hp.setCharacterSize(22);
    s_unit_hp.setFillColor(sf::Color::Black);
    s_unit_hp.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")));
    s_unit_hp.setOrigin(s_unit_hp.getGlobalBounds().width,s_unit_hp.getGlobalBounds().height/2);

    s_unit_crit.setFont(f_font);
    s_unit_crit.setCharacterSize(22);
    s_unit_crit.setFillColor(sf::Color::Black);
    s_unit_crit.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")));
    s_unit_crit.setOrigin(s_unit_crit.getGlobalBounds().width,s_unit_crit.getGlobalBounds().height/2);

    s_unit_attack_speed.setFont(f_font);
    s_unit_attack_speed.setCharacterSize(22);
    s_unit_attack_speed.setFillColor(sf::Color::Black);
    s_unit_attack_speed.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"barracks_test_stat_text")));
    s_unit_attack_speed.setOrigin(s_unit_attack_speed.getGlobalBounds().width,s_unit_attack_speed.getGlobalBounds().height/2);


    /// unit + item name text

    t_unit_rarepon_name.setFont(f_font);
    t_unit_rarepon_name.setCharacterSize(22);
    t_unit_rarepon_name.setFillColor(sf::Color::Black);
    t_unit_rarepon_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")));
    t_unit_rarepon_name.setOrigin(0,t_unit_rarepon_name.getGlobalBounds().height/2);

    t_weapon_name.setFont(f_font);
    t_weapon_name.setCharacterSize(22);
    t_weapon_name.setFillColor(sf::Color::Black);
    t_weapon_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")));
    t_weapon_name.setOrigin(0,t_weapon_name.getGlobalBounds().height/2);

    t_weapon2_name.setFont(f_font);
    t_weapon2_name.setCharacterSize(22);
    t_weapon2_name.setFillColor(sf::Color::Black);
    t_weapon2_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")));
    t_weapon2_name.setOrigin(0,t_weapon2_name.getGlobalBounds().height/2);

    t_armour_name.setFont(f_font);
    t_armour_name.setCharacterSize(22);
    t_armour_name.setFillColor(sf::Color::Black);
    t_armour_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")));
    t_armour_name.setOrigin(0,t_armour_name.getGlobalBounds().height/2);

    t_mask_name.setFont(f_font);
    t_mask_name.setCharacterSize(22);
    t_mask_name.setFillColor(sf::Color::Black);
    t_mask_name.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"wooden_spear")));
    t_mask_name.setOrigin(0,t_mask_name.getGlobalBounds().height/2);

    possibleMenuPositions = {-2300, -1300,600,1500,3700};
    currentMenuPosition = -1;
    enabledPositons.push_back(true);
    enabledPositons.push_back(true);
    enabledPositons.push_back(false);
    enabledPositons.push_back(true);
    enabledPositons.push_back(true);
}
void Barracks::EventFired(sf::Event event){
    if(isActive){
        if(event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == thisConfig->GetInt("keybindBack")){
                this->Hide();
                this->isActive = false;
                parentMenu->Show();
                parentMenu->isActive=true;
            }
            int keyCode = event.key.code;
            if(keyCode == thisConfig->GetInt("keybindChaka") || keyCode == thisConfig->GetInt("secondaryKeybindChaka"))
            {
                currentItemPosition -=1;
                if (currentItemPosition<0){
                    currentItemPosition = enabledPositons.size()-1;
                }
                while(!enabledPositons[currentItemPosition]){
                    currentItemPosition -=1;
                }

            }
            if(keyCode == thisConfig->GetInt("keybindDon") || keyCode == thisConfig->GetInt("secondaryKeybindDon"))
            {
                currentItemPosition +=1;
                if (currentItemPosition>enabledPositons.size()-1){
                    currentItemPosition = 0;
                }
                while(!enabledPositons[currentItemPosition]){
                    currentItemPosition +=1;
                }
            }

        } else if (event.type == sf::Event::MouseButtonReleased){
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        }
    }

}
void Barracks::Update(sf::RenderWindow &window, float fps)
{
    if(isActive){

        mm_bigBox.setSize(sf::Vector2f(window.getSize().x,window.getSize().y-200));

        mm_bigBox.setPosition(0,85);


        window.draw(mm_bigBox);



        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        auto lastView = window.getView();
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos,lastView);

        //t_pressToContinue.setPosition(window.getSize().x/2,300);
        //window.draw(t_pressToContinue);

        s_menu_bkg.setPosition(p_menu_bkg.x,p_menu_bkg.y);
        s_menu_bkg.draw(window);


        s_class_icon.setPosition(p_class_icon.x,p_class_icon.y);
        s_class_icon.draw(window);




        window.setView(window.getDefaultView());

        //t_title.setPosition(window.getSize().x/2,110);
        //window.draw(t_title);

        float leftStatNameTextX = p_class_icon.x+120;
        float leftStatValueTextX = (window.getSize().x/2)-300-50;

        float rightStatNameTextX = (window.getSize().x/2)-300+50;
        float rightStatValueTextX = (window.getSize().x)-650;

        /// left stats text

        t_unit_name.setPosition(leftStatNameTextX,p_class_icon.y+11);
        t_unit_level.setPosition(leftStatNameTextX,p_class_icon.y+44);
        t_unit_experience.setPosition(leftStatNameTextX,p_class_icon.y+66);
        t_unit_damage.setPosition(leftStatNameTextX,p_class_icon.y+88);
        window.draw(t_unit_name);
        window.draw(t_unit_level);
        window.draw(t_unit_experience);
        window.draw(t_unit_damage);

        /// left stats values

        s_unit_level.setPosition(leftStatValueTextX,p_class_icon.y+44);
        s_unit_experience.setPosition(leftStatValueTextX,p_class_icon.y+66);
        s_unit_damage.setPosition(leftStatValueTextX,p_class_icon.y+88);
        window.draw(s_unit_level);
        window.draw(s_unit_experience);
        window.draw(s_unit_damage);

        /// right stats text
        t_unit_status_text.setPosition((window.getSize().x*1.0)-70,p_class_icon.y-11);

        t_unit_hp.setPosition(rightStatNameTextX,p_class_icon.y+44);
        t_unit_crit.setPosition(rightStatNameTextX,p_class_icon.y+66);
        t_unit_attack_speed.setPosition(rightStatNameTextX,p_class_icon.y+88);

        window.draw(t_unit_status_text);
        window.draw(t_unit_hp);
        window.draw(t_unit_crit);
        window.draw(t_unit_attack_speed);

        /// right stats values

        s_unit_hp.setPosition(rightStatValueTextX,p_class_icon.y+44);
        s_unit_crit.setPosition(rightStatValueTextX,p_class_icon.y+66);
        s_unit_attack_speed.setPosition(rightStatValueTextX,p_class_icon.y+88);

        window.draw(s_unit_hp);
        window.draw(s_unit_crit);
        window.draw(s_unit_attack_speed);

        /// equipped item + unit name text

        if (enabledPositons[0]){  /// unit name
            t_unit_rarepon_name.setPosition(p_unit_icon.x+68,p_unit_icon.y);
            s_unit_icon.setPosition(p_unit_icon.x,p_unit_icon.y);
            s_unit_icon.draw(window);
            window.draw(t_unit_rarepon_name);
        }
        if (enabledPositons[1]){  /// wep1
            t_weapon_name.setPosition(p_unit_icon.x+68,p_weapon_icon.y);
            s_weapon_icon.setPosition(p_unit_icon.x,p_weapon_icon.y);
            s_weapon_icon.draw(window);
            window.draw(t_weapon_name);
        }
        if (enabledPositons[2]){  /// wep2
            t_weapon2_name.setPosition(p_unit_icon.x+68,p_weapon2_icon.y);
            s_weapon2_icon.setPosition(p_unit_icon.x,p_weapon2_icon.y);
            s_weapon2_icon.draw(window);
            window.draw(t_weapon2_name);
        }
        if (enabledPositons[3]){  /// armour
            t_armour_name.setPosition(p_unit_icon.x+68,p_armour_icon.y);
            s_armour_icon.setPosition(p_unit_icon.x,p_armour_icon.y);
            s_armour_icon.draw(window);
            window.draw(t_armour_name);
        }
        if (enabledPositons[4]){  /// mask
            t_mask_name.setPosition(p_unit_icon.x+68,p_mask_icon.y);
            s_mask_icon.setPosition(p_unit_icon.x,p_mask_icon.y);
            s_mask_icon.draw(window);
            window.draw(t_mask_name);
        }

        mm_selected_item_line.setSize(sf::Vector2f(400,4));
        mm_selected_item_line.setPosition(p_weapon_icon.x-30,p_unit_icon.y+30+currentItemPosition*68);
        window.draw(mm_selected_item_line);
    }
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
