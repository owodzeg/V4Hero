#define SDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "../V4Core.h"
#include "../CoreManager.h"
#include "MaterOuter.h"
#include "ButtonList.h"
#include <iostream>
#include <math.h>
#include <spdlog/spdlog.h>
#include <span>

MaterOuterMenu::MaterOuterMenu()
{
    is_active = false;
}

void MaterOuterMenu::initialise(Config* _thisConfig, V4Core* parent, PatapolisMenu* curParentMenu)
{
    SPDLOG_INFO("Initializing Altar...");
    //Scene::Initialise(_thisConfig, parent);
    parentMenu = curParentMenu;

    int quality = _thisConfig->GetInt("textureQuality");
    q = quality;

    switch (quality)
    {
        case 0: ///low
        {
            ratio_x = _thisConfig->GetInt("resX") / float(640);
            ratio_y = _thisConfig->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratio_x = _thisConfig->GetInt("resX") / float(1280);
            ratio_y = _thisConfig->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratio_x = _thisConfig->GetInt("resX") / float(1920);
            ratio_y = _thisConfig->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratio_x = _thisConfig->GetInt("resX") / float(3840);
            ratio_y = _thisConfig->GetInt("resY") / float(2160);
            break;
        }
    }

    res_ratio_x = _thisConfig->GetInt("resX") / float(1280);
    res_ratio_y = _thisConfig->GetInt("resY") / float(720);

    f_font.loadFromFile(_thisConfig->fontPath);

    ResourceManager::getInstance().loadSprite("path/to/sprite.png");

    mater_main.loadFromFile("resources/graphics/ui/mater/mater_outer_bg.png", quality, 1);
    mater_selector.loadFromFile("resources/graphics/ui/mater/materui_squad_select.png", quality, 1);

    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_bg_asleep.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_member_asleep.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_slot_asleep.png");

    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_bg.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_member_awake.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_slot_awake.png");

    up_arrow_prompt.loadFromFile("resources/graphics/ui/mater/up_prompt.png", quality, 1);
    down_arrow_prompt.loadFromFile("resources/graphics/ui/mater/down_prompt.png", quality, 1);

    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/yaripon_icon.png");

    mater_title.createText(f_font, 40, sf::Color(255, 234, 191, 255), Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("mater_title")), q, 1);
    altar_kaching.createText(f_font, 30, sf::Color(255, 234, 191, 255), "0 Ka-ching", q, 1);
    squad_title.createText(f_font, 30, sf::Color(255, 234, 191, 255),  Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("yaripon_squad")), q, 1);

    ctrlTips.create(54, f_font, 20, Func::ConvertToUtf8String(CoreManager::getInstance().getStrRepo()->GetString("mater_outer_ctrl_tips")), quality);

    SPDLOG_INFO("Initializing Altar finished.");
}
void MaterOuterMenu::showCategory()
{
}

void MaterOuterMenu::MoveSquadPos(int spaces)
{
    squadpos = squadpos + spaces;
    if (squadpos > squads[cursquad].maxSize-1)
    {
        squadpos = squadpos - squads[cursquad].maxSize;
    }
    if (squadpos < 0)
    {
        squadpos = squadpos + squads[cursquad].maxSize;
    }
}


void MaterOuterMenu::showMater()
{
    // TODO: check which squads player has unlocked memories for
    squads.clear();
    for (int i = 0; i < 9; i++)
    {
        SquadBox squad = SquadBox();
        squad.amount = 3;
        squad.maxSize = 6;
        squad.y = 165 + 100 * i;

        squads.push_back(squad);
    }
    squads[0].title = "Yaripon";
    squads[1].title = "Tatepon";
    squads[2].title = "Yumipon";
    squads[3].title = "Kibapon";
    squads[4].title = "Dekapon";
    squads[5].title = "Megapon";
    squads[6].title = "Toripon";
    squads[7].title = "Robopon";
    squads[8].title = "Mahopon";

    
    vector<MaterOuterMenu::SquadBox*> centered_squads = GetSquadsCentered();
    for (int i = 0; i < 5; i++)
    {
        centered_squads[i]->y = 165 + 100 * i; // re-center the squads
    }
}

void MaterOuterMenu::eventFired(sf::Event event)
{
    if (is_active)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            // do something here;
        } else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) 
            {
                SPDLOG_DEBUG("Mouse clicked at {},{}", mouseX, mouseY);
                // need to convert mouse pos to standard coords.
                sf::Vector2i mousepos = sf::Vector2i(mouseX, mouseY);
                for (auto i = 0; i < squads.size(); i++)
                {
                    SquadBox& squad = squads[i];
                    sf::IntRect rect(680, squad.y-50, 120, 100);
                    if (rect.contains(mousepos)) {
                        cursquad = i;
                    }
                }
            }
        } 
        else if (event.type == sf::Event::MouseMoved)
        {
            mouseX = event.mouseMove.x;
            mouseY = event.mouseMove.y;
        }
    }
}
void MaterOuterMenu::DrawAsleepSquad(MaterOuterMenu::SquadBox& squad, int squad_alpha, sf::RenderWindow& window)
{
    PSprite& bg = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_bg_asleep.png");
    PSprite& slot = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_slot_asleep.png");
    PSprite& pon = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_member_asleep.png");

    PSprite& icon = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/yaripon_icon.png");

    bg.setOrigin(bg.getLocalBounds().width / 2, bg.getLocalBounds().height / 2);
    bg.setPosition(990, squad.y);
    bg.setColor(sf::Color(255, 255, 255, squad_alpha));
    bg.draw(window);

    icon.setOrigin(icon.getLocalBounds().width / 2, icon.getLocalBounds().height / 2);
    icon.setPosition(745, squad.y-32);
    icon.setColor(sf::Color(255, 255, 255, squad_alpha));
    icon.draw(window);

    squad_title.setString(squad.title);
    squad_title.setOrigin(squad_title.getLocalBounds().width / 2, squad_title.getLocalBounds().height / 2);
    squad_title.setPosition(875, squad.y - 65);
    squad_title.setColor(sf::Color(0, 0, 0, squad_alpha));
    squad_title.draw(window);

    pon.setOrigin(pon.getLocalBounds().width / 2, pon.getLocalBounds().height / 2);
    slot.setOrigin(pon.getLocalBounds().width / 2, pon.getLocalBounds().height / 2);

    pon.setColor(sf::Color(255, 255, 255, squad_alpha));
    slot.setColor(sf::Color(255, 255, 255, squad_alpha));

    for (int j = 0; j < squad.maxSize; j++)
    {
        if (j < squad.amount)
        {
            pon.setPosition(850 + j * 73, squad.y + 15);
            pon.draw(window);
        } else
        {
            slot.setPosition(850 + j * 73, squad.y + 15);
            slot.draw(window);
        }
    }
}

vector<MaterOuterMenu::SquadBox*> MaterOuterMenu::GetSquadsCentered()
{
    vector<MaterOuterMenu::SquadBox*> out;
    for (int i = -2; i < 3; i++)
    {
        int pos = cursquad + i;
        if (pos < 0) {
            pos = pos + squads.size();
        }
        if (pos > squads.size()-1)
        {
            pos = pos - squads.size();
        }
        out.push_back(&squads[pos]);
    }
    return out;
}

void MaterOuterMenu::update(sf::RenderWindow& window, float fps, InputController& inputCtrl)
{
    if (is_active)
    {
        highlight_x += 7.0 / fps;

        ctrlTips.x = 0;
        ctrlTips.y = (720 - ctrlTips.ySize);
        ctrlTips.draw(window);

        mater_main.setOrigin(mater_main.getLocalBounds().width / 2, mater_main.getLocalBounds().height / 2);
        mater_main.setPosition(1050, 322);

        mater_main.draw(window);

        mater_title.setOrigin(mater_title.getLocalBounds().width / 2, mater_title.getLocalBounds().height / 2);
        altar_kaching.setOrigin(altar_kaching.getLocalBounds().width / 2, altar_kaching.getLocalBounds().height / 2);

        mater_title.setPosition(1050, 15);
        altar_kaching.setPosition(1050, 45);

        mater_title.draw(window);
        altar_kaching.draw(window);

        vector<MaterOuterMenu::SquadBox*> centered_squads = GetSquadsCentered();


        std::span<SquadBox*> beforesquads = std::span(centered_squads).subspan(0, 2);
        int draw_n = 0;
        for (auto it = beforesquads.begin(); it != beforesquads.end(); ++it)
        {
            SquadBox& squad = **it;
                
                
            int squad_alpha = 255 - 80 * (beforesquads.size() - draw_n - 1);
            if (squad_alpha < 0)
            {
                squad_alpha = 0;
            }
            DrawAsleepSquad(squad, squad_alpha, window);
            draw_n++;
        }
        

        // draw the squads after selected squad in reverse order
        
        // aftersquads start at cursquad+1 and go to end 
        std::span<SquadBox*> aftersquads = std::span(centered_squads).subspan(3, 2);
        draw_n = 0;
        // rbegin to rend - reverse iteration
        for (auto it = aftersquads.rbegin(); it != aftersquads.rend(); ++it)
        {
            SquadBox& squad = **it;


            int squad_alpha = 255 - 80 * (aftersquads.size() - draw_n - 1);
            if (squad_alpha < 0)
            {
                squad_alpha = 0;
            }
            DrawAsleepSquad(squad, squad_alpha, window);
            draw_n++;
        }
        

        // finally, we draw the current selected squad, so its on top of everything
        SquadBox& squad = *centered_squads[2];

        PSprite& bg = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_bg.png");
        PSprite& slot = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_slot_awake.png");
        PSprite& pon = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_member_awake.png");
        PSprite& icon = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/yaripon_icon.png");

        bg.setOrigin(bg.getLocalBounds().width / 2, bg.getLocalBounds().height / 2);
        bg.setPosition(990, squad.y);
        bg.draw(window);

        icon.setOrigin(icon.getLocalBounds().width / 2, icon.getLocalBounds().height / 2);
        icon.setPosition(745, squad.y - 32);
        icon.draw(window);

        squad_title.setString(squad.title);
        squad_title.setOrigin(squad_title.getLocalBounds().width / 2, squad_title.getLocalBounds().height / 2);
        squad_title.setPosition(875, squad.y - 65);
        squad_title.setColor(sf::Color(0, 0, 0, 255));
        squad_title.draw(window);
        GetSquadsCentered();

        pon.setOrigin(pon.getLocalBounds().width / 2, pon.getLocalBounds().height / 2);
        slot.setOrigin(slot.getLocalBounds().width / 2, slot.getLocalBounds().height / 2);
        for (int j = 0; j < squad.maxSize; j++)
        {
            if (j < squad.amount)
            {
                pon.setPosition(850 + j * 73, squad.y + 15);
                pon.draw(window);
            } else
            {
                slot.setPosition(850 + j * 73, squad.y + 15);
                slot.draw(window);
            }
        }
        mater_selector.setOrigin(mater_selector.getLocalBounds().width / 2, mater_selector.getLocalBounds().height / 2);
        mater_selector.setPosition(850 + squadpos * 73, squad.y + 15);
        mater_selector.draw(window);

        up_arrow_prompt.setOrigin(up_arrow_prompt.getLocalBounds().width / 2, up_arrow_prompt.getLocalBounds().height / 2);
        down_arrow_prompt.setOrigin(down_arrow_prompt.getLocalBounds().width / 2, down_arrow_prompt.getLocalBounds().height / 2);

        up_arrow_prompt.setPosition(1050, 75);
        down_arrow_prompt.setPosition(1050, 655);

        up_arrow_prompt.draw(window);
        down_arrow_prompt.draw(window);

        for (int i = 0; i < squads.size(); i++)
        {
            SquadBox& squad = squads[i];

            int squad_height = 115;
            
            // TODO: check what type the squad is and adjust icon etc based on that
            // TODO: make this a spinny wheel instead of a list (ask rugnir if he hasnt done it)
            // TODO: think about adding support for different pon types (megapon, dekapon etc)
            // TODO: think about different rarepon pictures too.

            
        }
        if (inputCtrl.isKeyPressed(InputController::Keys::LEFT))
        {
            this->MoveSquadPos(-1);
        }
        if (inputCtrl.isKeyPressed(InputController::Keys::RIGHT))
        {
            this->MoveSquadPos(1);
        }
        if (inputCtrl.isKeyPressed(InputController::Keys::UP))
        {
            cursquad--;
            if (cursquad < 0)
            {
                cursquad = cursquad + squads.size();
            }
            squadpos = 0; // reset squad pos in case squad has different max size

            vector<MaterOuterMenu::SquadBox*> centered_squads = GetSquadsCentered();
            for (int i = 0; i < 5; i++)
            {
                centered_squads[i]->y = 165 + 100 * i; // re-center the squads
            }
        }
        if (inputCtrl.isKeyPressed(InputController::Keys::DOWN))
        {
            cursquad++;
            if (cursquad > squads.size()-1)
            {
                cursquad = cursquad - squads.size();
            }
            squadpos = 0; // reset squad pos in case squad has different max size

            vector<MaterOuterMenu::SquadBox*> centered_squads = GetSquadsCentered();
            for (int i = 0; i < 5; i++)
            {
                centered_squads[i]->y = 165 + 100 * i; // re-center the squads
            }
        }
        if (inputCtrl.isKeyPressed(InputController::Keys::CIRCLE))
        {
            this->is_active = false;
        }
    }
}

void MaterOuterMenu::updateButtons()
{
}

void MaterOuterMenu::onExit()
{
}

MaterOuterMenu::~MaterOuterMenu()
{
    //dtor
}
