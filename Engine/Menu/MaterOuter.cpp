#define SDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "../V4Core.h"
#include "../CoreManager.h"
#include "MaterOuter.h"
#include <math.h>
#include <spdlog/spdlog.h>
#include <span>
#include "../StateManager.h"
#include "../Constants.h"

MaterOuterMenu::MaterOuterMenu()
{
    is_active = false;
    SPDLOG_INFO("Initializing Altar...");
    Config* config = CoreManager::getInstance().getConfig();

    int quality = config->GetInt("textureQuality");
    q = quality;

    switch (quality)
    {
        case 0: ///low
        {
            ratio_x = config->GetInt("resX") / CANVAS_LOW_X;
            ratio_y = config->GetInt("resY") / CANVAS_LOW_Y;
            break;
        }

        case 1: ///med
        {
            ratio_x = config->GetInt("resX") / CANVAS_MED_X;
            ratio_y = config->GetInt("resY") / CANVAS_MED_Y;
            break;
        }

        case 2: ///high
        {
            ratio_x = config->GetInt("resX") / CANVAS_HIGH_X;
            ratio_y = config->GetInt("resY") / CANVAS_HIGH_Y;
            break;
        }

        case 3: ///ultra
        default:
        {
            ratio_x = config->GetInt("resX") / CANVAS_ULTRA_X;
            ratio_y = config->GetInt("resY") / CANVAS_ULTRA_Y;
            break;
        }
        
    }

    mater_main.loadFromFile("resources/graphics/ui/mater/mater_outer_bg.png");
    mater_selector.loadFromFile("resources/graphics/ui/mater/materui_squad_select.png");

    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_bg_asleep.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_member_asleep.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_slot_asleep.png");

    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_bg.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_member_awake.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/matersquad_slot_awake.png");

    up_arrow_prompt.loadFromFile("resources/graphics/ui/mater/up_prompt.png");
    down_arrow_prompt.loadFromFile("resources/graphics/ui/mater/down_prompt.png");

    ResourceManager::getInstance().loadSprite("resources/graphics/ui/mater/yaripon_icon.png");

    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    mater_title.defaultStyleSetFont(font);
    mater_title.defaultStyleSetCharSize(40);
    mater_title.defaultStyleSetColor(sf::Color(255, 234, 191, 255));
    mater_title.append(Func::GetStrFromKey("mater_title"));
    mater_title.defaultStyleSetFont(font);
    mater_title.defaultStyleSetCharSize(30);
    mater_title.defaultStyleSetColor(sf::Color(255, 234, 191, 255));
    mater_title.append("0 Ka-ching");
    mater_title.defaultStyleSetFont(font);
    mater_title.defaultStyleSetCharSize(30);
    mater_title.defaultStyleSetColor(sf::Color(255, 234, 191, 255));
    mater_title.append(Func::GetStrFromKey("yaripon_squad"));

    ctrlTips.create(54, font, 20, "mater_outer_ctrl_tips");

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
    int i = 0;
    for (const Squad& squad : CoreManager::getInstance().getSaveReader()->squadReg.m_squads ){
        SquadBox squadbox = SquadBox();
        squadbox.amount = 3;
        squadbox.maxSize = 6;
        squadbox.title = squad.getName();
        squadbox.y = static_cast<float>(165 + 100 * i);
        squadbox.squad_icon = squad.getImage();
        squads.push_back(squadbox);
        i++;
    }

    
    std::vector<MaterOuterMenu::SquadBox*> centered_squads = GetSquadsCentered();
    for (int a = 0; a < 5; a++)
    {
        centered_squads[a]->y = static_cast<float>(165 + 100 * a); // re-center the squads
    }
}

void MaterOuterMenu::DrawAsleepSquad(MaterOuterMenu::SquadBox& squad, int squad_alpha)
{
    PSprite& bg = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_bg_asleep.png");
    PSprite& slot = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_slot_asleep.png");
    PSprite& pon = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_member_asleep.png");

    
    PSprite& icon = ResourceManager::getInstance().getSprite("resources/graphics/"+squad.squad_icon+".png");

    bg.setOrigin(bg.getLocalBounds().size.x / 2, bg.getLocalBounds().size.y / 2);
    bg.setPosition(990, squad.y);
    bg.setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(squad_alpha)));
    bg.draw();

    icon.setOrigin(icon.getLocalBounds().size.x / 2, icon.getLocalBounds().size.y / 2);
    icon.setPosition(745, squad.y-32);
    icon.setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(squad_alpha)));
    icon.draw();

    squad_title.append(squad.title);
    squad_title.setGlobalOrigin(squad_title.getGlobalBounds().size.x / 2, squad_title.getGlobalBounds().size.y / 2);
    squad_title.setGlobalPosition(875, squad.y - 65);
    squad_title.defaultStyleSetColor(sf::Color(0, 0, 0, static_cast<uint8_t>(squad_alpha)));
    squad_title.draw();

    pon.setOrigin(pon.getLocalBounds().size.x / 2, pon.getLocalBounds().size.y / 2);
    slot.setOrigin(pon.getLocalBounds().size.x / 2, pon.getLocalBounds().size.y / 2);

    pon.setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(squad_alpha)));
    slot.setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(squad_alpha)));

    for (int j = 0; j < squad.maxSize; j++)
    {
        if (j < squad.amount)
        {
            pon.setPosition(850 + static_cast<float>(j) * 73, squad.y + 15);
            pon.draw();
        } else
        {
            slot.setPosition(850 + static_cast<float>(j) * 73, squad.y + 15);
            slot.draw();
        }
    }
}

std::vector<MaterOuterMenu::SquadBox*> MaterOuterMenu::GetSquadsCentered()
{
    std::vector<MaterOuterMenu::SquadBox*> out;
    for (int i = -2; i < 3; i++)
    {
        int pos = cursquad + i;
        if (pos < 0) {
            pos = pos + static_cast<int>(squads.size());
        }
        if (pos > static_cast<int>(squads.size()) - 1)
        {
            pos = pos - static_cast<int>(squads.size());
        }
        out.push_back(&squads[pos]);
    }
    return out;
}

void MaterOuterMenu::Update()
{
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    float fps = CoreManager::getInstance().getCore()->getFPS();
    highlight_x += 7.0f / fps;

    ctrlTips.x = 0;
    ctrlTips.y = (720 - ctrlTips.ySize);
    ctrlTips.draw();

    mater_main.setOrigin(mater_main.getLocalBounds().size.x / 2, mater_main.getLocalBounds().size.y / 2);
    mater_main.setPosition(1050, 322);

    mater_main.draw();

    mater_title.setGlobalOrigin(mater_title.getGlobalBounds().size.x / 2, mater_title.getGlobalBounds().size.y / 2);
    altar_kaching.setGlobalOrigin(altar_kaching.getGlobalBounds().size.x / 2, altar_kaching.getGlobalBounds().size.y / 2);

    mater_title.setGlobalPosition(1050, 15);
    altar_kaching.setGlobalPosition(1050, 45);

    mater_title.draw();
    altar_kaching.draw();

    std::vector < MaterOuterMenu::SquadBox*> centered_squads = GetSquadsCentered();

    std::span<SquadBox*> beforesquads = std::span(centered_squads).subspan(0, 2);
    int draw_n = 0;
    for (auto it = beforesquads.begin(); it != beforesquads.end(); ++it)
    {
        SquadBox& squad = **it;
                
                
        int squad_alpha = 255 - 80 * (static_cast<int>(beforesquads.size()) - draw_n - 1);
        if (squad_alpha < 0)
        {
            squad_alpha = 0;
        }
        DrawAsleepSquad(squad, squad_alpha);
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


        int squad_alpha = 255 - 80 * (static_cast<int>(aftersquads.size()) - draw_n - 1);
        if (squad_alpha < 0)
        {
            squad_alpha = 0;
        }
        DrawAsleepSquad(squad, squad_alpha);
        draw_n++;
    }
        

    // finally, we draw the current selected squad, so its on top of everything
    SquadBox& squad = *centered_squads[2];

    PSprite& bg = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_bg.png");
    PSprite& slot = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_slot_awake.png");
    PSprite& pon = ResourceManager::getInstance().getSprite("resources/graphics/ui/mater/matersquad_member_awake.png");
        
    PSprite& icon = ResourceManager::getInstance().getSprite("resources/graphics/"+squad.squad_icon+".png");

    bg.setOrigin(bg.getLocalBounds().size.x / 2, bg.getLocalBounds().size.y / 2);
    bg.setPosition(990, squad.y);
    bg.draw();

    icon.setOrigin(icon.getLocalBounds().size.x / 2, icon.getLocalBounds().size.y / 2);
    icon.setPosition(745, squad.y - 32);
    icon.draw();

    squad_title.append(squad.title);
    squad_title.setGlobalOrigin(squad_title.getGlobalBounds().size.x / 2, squad_title.getGlobalBounds().size.y / 2);
    squad_title.setGlobalPosition(875, squad.y - 65);
    squad_title.defaultStyleSetColor(sf::Color(0, 0, 0, 255));
    squad_title.draw();
    GetSquadsCentered();

    pon.setOrigin(pon.getLocalBounds().size.x / 2, pon.getLocalBounds().size.y / 2);
    slot.setOrigin(slot.getLocalBounds().size.x / 2, slot.getLocalBounds().size.y / 2);
    for (int j = 0; j < squad.maxSize; j++)
    {
        if (j < squad.amount)
        {
            pon.setPosition(850 + static_cast<float>(j) * 73, squad.y + 15);
            pon.draw();
        } else
        {
            slot.setPosition(850 + static_cast<float>(j) * 73, squad.y + 15);
            slot.draw();
        }
    }
    mater_selector.setOrigin(mater_selector.getLocalBounds().size.x / 2, mater_selector.getLocalBounds().size.y / 2);
    mater_selector.setPosition(850 + static_cast<float>(squadpos) * 73, squad.y + 15);
    mater_selector.draw();

    up_arrow_prompt.setOrigin(up_arrow_prompt.getLocalBounds().size.x / 2, up_arrow_prompt.getLocalBounds().size.y / 2);
    down_arrow_prompt.setOrigin(down_arrow_prompt.getLocalBounds().size.x / 2, down_arrow_prompt.getLocalBounds().size.y / 2);

    up_arrow_prompt.setPosition(1050, 75);
    down_arrow_prompt.setPosition(1050, 655);

    up_arrow_prompt.draw();
    down_arrow_prompt.draw();

    for (int i = 0; i < squads.size(); i++)
    {
        squad = squads[i];
            
        // TODO: check what type the squad is and adjust icon etc based on that
        // TODO: make this a spinny wheel instead of a list (ask rugnir if he hasnt done it)
        // TODO: think about adding support for different pon types (megapon, dekapon etc)
        // TODO: think about different rarepon pictures too.
    }
    if (inputCtrl->isKeyPressed(Input::Keys::LEFT))
    {
        this->MoveSquadPos(-1);
    }
    if (inputCtrl->isKeyPressed(Input::Keys::RIGHT))
    {
        this->MoveSquadPos(1);
    }
    if (inputCtrl->isKeyPressed(Input::Keys::UP))
    {
        cursquad--;
        if (cursquad < 0)
        {
            cursquad = cursquad + static_cast<int>(squads.size());
        }
        squadpos = 0; // reset squad pos in case squad has different max size

        centered_squads = GetSquadsCentered();
        for (int i = 0; i < 5; i++)
        {
            centered_squads[i]->y = 165 + 100 * static_cast<float>(i); // re-center the squads
        }
    }
    if (inputCtrl->isKeyPressed(Input::Keys::DOWN))
    {
        cursquad++;
        if (cursquad > static_cast<int>(squads.size()) - 1)
        {
            cursquad = cursquad - static_cast<int>(squads.size());
        }
        squadpos = 0; // reset squad pos in case squad has different max size

        centered_squads = GetSquadsCentered();
        for (int i = 0; i < 5; i++)
        {
            centered_squads[i]->y = 165 + 100 * static_cast<float>(i); // re-center the squads
        }
    }
    if (inputCtrl->isKeyPressed(Input::Keys::CIRCLE))
    {
        is_active = false;
        StateManager::getInstance().setState(StateManager::PATAPOLIS);
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
