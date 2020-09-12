#include "MissionController.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include "Units/Projectile.h"
#include "../Math/PVector.h"
#include "Units/HitboxFrame.h"
#include "../V4Core.h"
#include "../Item/InventoryItem.h"

///move to Func::numDigits later
template <class T>
int numDigits(T number) //stolen from stackoverflow
{
    int digits = 0;
    if (number < 0) digits = 1;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

MissionController::MissionController()
{
}

float MissionController::Smoothstep(float time) ///use time from 0.00 to 1.00
{
    time = Clamp(time, 0.0, 1.0);
    return time * time * (3 - 2 * time);
}

float MissionController::Clamp(float x, float lowerlimit, float upperlimit)
{
    if (x < lowerlimit)
        x = lowerlimit;
    if (x > upperlimit)
        x = upperlimit;

    return x;
}

void MissionController::addDmgCounter(int type, int damage, float baseX, float baseY, int q, int r)
{
    DamageCounter tmp;
    tmp.type = type;
    tmp.damage = damage;

    cout << "MissionController::addDmgCounter(" << type << ", " << damage << ", " << baseX << ", " << baseY << ")" << endl;

    int digits = numDigits(damage);
    string sdigits = to_string(damage);

    cout << "Digits: " << digits << " " << sdigits << endl;

    int separator = 0;
    float init_scale = 1;
    float dg_scale = 4;

    if(damage < 100)
    type = 2;
    else
    type = 3;

    switch(type)
    {
        case 0:
        {
            separator = 12;
            init_scale = 0.8;
            dg_scale = 2.8;
            break;
        }

        case 1:
        {
            separator = 12;
            init_scale = 0.8;
            dg_scale = 2.8;
            break;
        }

        case 2:
        {
            separator = 16;
            init_scale = 0.8;
            dg_scale = 3.5;
            break;
        }

        case 3:
        {
            separator = 18;
            init_scale = 0.8;
            dg_scale = 3.9;
            break;
        }

        case 4:
        {
            separator = 20;
            init_scale = 0.8;
            dg_scale = 4.1;
            break;
        }
    }

    for(int i=0; i<digits; i++)
    {
        string sdigit = string()+sdigits[i];
        int digit = atoi(sdigit.c_str());

        PSprite dg_spr;
        dg_spr.setTexture(dmg_spritesheet.t);
        dg_spr.setTextureRect(dmg_spritesheet.get_bounds((digit*5)+type)); ///rect of the specific damage digit from spritesheet
        dg_spr.setOrigin(dg_spr.getLocalBounds().width/2, dg_spr.getLocalBounds().height);
        dg_spr.qualitySetting = q;
        dg_spr.resSetting = r;

        sf::Vector2f dg_pos(baseX+(i*separator), baseY);

        tmp.spr.push_back(dg_spr);
        tmp.pos.push_back(dg_pos);
        tmp.scale.push_back(dg_scale);
        tmp.scale_goal.push_back(init_scale);
        tmp.mode.push_back(true);
        tmp.alpha.push_back(0);
    }

    dmgCounters.push_back(tmp);
}

void MissionController::addItemsCounter(int id, float baseX, float baseY)
{

}

void MissionController::spawnEntity(string entityName, int entityID, int baseHP, int baseX, int randX, int baseY, int spr_goal, int spr_range, int statLevel, sf::Color color, bool collidable, bool attackable, int layer, int parent, float overrideY, float overrideHP, vector<Entity::Loot> loot_table, vector<string> additional_data)
{
    ///need to somehow optimize this to not copy paste the same code over and over

    cout << "Spawning entity " << entityName << " (ID: " << entityID << ") " << baseHP << " " << baseX << " " << randX << " " << baseY << " " << spr_goal << " " << spr_range << " " << statLevel << endl;

    bool spawn = false;

    if(spr_range != 0)
    {
        if(rand() % spr_range == spr_goal)
        {
            spawn = true;
        }
    }
    else
    {
        spawn = true;
    }

    switch(entityID)
    {
        case 0:
        {
            unique_ptr<EndFlag> entity = make_unique<EndFlag>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::PASSIVE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 1:
        {
            unique_ptr<FeverWorm> entity = make_unique<FeverWorm>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::PASSIVE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 2:
        {
            unique_ptr<Kacheek> entity = make_unique<Kacheek>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 3:
        {
            unique_ptr<Grass1> entity = make_unique<Grass1>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::NEUTRAL;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 4:
        {
            unique_ptr<Grass2> entity = make_unique<Grass2>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::NEUTRAL;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 5:
        {
            unique_ptr<DroppedItem> entity = make_unique<DroppedItem>();
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::NEUTRAL;

            string spritesheet = additional_data[0];
            int spritesheet_id = stoi(additional_data[1]);
            int picked_item = stoi(additional_data[2]);

            entity->spritesheet = spritesheet;
            entity->spritesheet_id = spritesheet_id;
            entity->picked_item = picked_item;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 6:
        {
            unique_ptr<Kirajin_Yari_1> entity = make_unique<Kirajin_Yari_1>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 7:
        {
            unique_ptr<TreasureChest> entity = make_unique<TreasureChest>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 8:
        {
            unique_ptr<RockBig> entity = make_unique<RockBig>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 9:
        {
            unique_ptr<RockSmall> entity = make_unique<RockSmall>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 10:
        {
            unique_ptr<WoodenSpikes> entity = make_unique<WoodenSpikes>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 11:
        {
            unique_ptr<RockPile> entity = make_unique<RockPile>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 12:
        {
            unique_ptr<KirajinHut> entity = make_unique<KirajinHut>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 13:
        {
            unique_ptr<KirajinGuardTower> entity = make_unique<KirajinGuardTower>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 14:
        {
            unique_ptr<KirajinPoweredTowerSmall> entity = make_unique<KirajinPoweredTowerSmall>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 15:
        {
            unique_ptr<KirajinPoweredTowerBig> entity = make_unique<KirajinPoweredTowerBig>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
        case 16:
        {
            unique_ptr<Kirajin_Yari_2> entity = make_unique<Kirajin_Yari_2>();
            entity.get()->additional_data = additional_data;
            entity.get()->LoadConfig(missionConfig);

            ///To be replaced with param file
            entity.get()->entityType = Entity::EntityTypes::HOSTILE;

            if(spawn)
            tangibleLevelObjects.push_back(std::move(entity));

            break;
        }
    }

    Entity* entity = tangibleLevelObjects[tangibleLevelObjects.size()-1].get();

    entity->spawnOrderID = tangibleLevelObjects.size()-1;

    if(entityID != 5) ///ID 5 = dropped item, it has an exclusive loading type
    {
        entity->setEntityID(entityID);

        entity->floorY = baseY; ///in case Y gets overriden, save the position where the floor is

        if(overrideHP > 0)
        baseHP = overrideHP;

        if(overrideY != 0)
        baseY = overrideY;

        if(randX > 0)
        entity->setGlobalPosition(sf::Vector2f(baseX + (rand() % randX),baseY));
        else
        entity->setGlobalPosition(sf::Vector2f(baseX,baseY));

        entity->setColor(color);

        entity->isCollidable = collidable;
        entity->isAttackable = attackable;
        entity->loot_table = loot_table;
        entity->curHP = baseHP;
        entity->maxHP = baseHP;

        entity->layer = layer;
        entity->parent = parent;
        entity->additional_data = additional_data;
    }
    else
    {
        entity->setEntityID(entityID);
        entity->manual_mode = true;

        ///This unique entity needs to be loaded differently, read additional data for spritesheet info to be passed from the item registry.
        string spritesheet = additional_data[0];
        int spritesheet_id = stoi(additional_data[1]);
        int picked_item = stoi(additional_data[2]);

        cout << "[DroppedItem] Selecting spritesheet " << spritesheet << " with id " << spritesheet_id << endl;

        cout << "[DroppedItem] Loading from memory" << endl;
        vector<char> di_data = droppeditem_spritesheet[spritesheet].retrieve_char();
        cout << "[DroppedItem] Vector loaded. Size: " << di_data.size() << endl;
        entity->objects[0].tex_obj.loadFromMemory(&di_data[0], di_data.size());
        cout << "[DroppedItem] Setting smooth" << endl;
        entity->objects[0].tex_obj.setSmooth(true);
        cout << "[DroppedItem] Setting texture" << endl;
        entity->objects[0].s_obj.setTexture(entity->objects[0].tex_obj);
        cout << "[DroppedItem] Marking as unexported" << endl;
        entity->objects[0].exported = false;
        cout << "[DroppedItem] Loading done." << endl;

        entity->objects[0].s_obj.qualitySetting = qualitySetting;
        entity->objects[0].s_obj.resSetting = resSetting;

        //entity->objects[0].s_obj.setOrigin(entity->objects[0].s_obj.getLocalBounds().width/2, entity->objects[0].s_obj.getLocalBounds().height/2);

        entity->cur_pos = float(spritesheet_id-1) / 60.0;

        entity->animation_bounds[0] = droppeditem_spritesheet[spritesheet].retrieve_rect_as_map();

        entity->setGlobalPosition(sf::Vector2f(baseX,baseY));

        entity->setColor(color);

        entity->isCollidable = collidable;
        entity->isAttackable = attackable;

        entity->layer = 9999;
        entity->parent = -1;
    }

    cout << "Loading finished" << endl;
}

void MissionController::spawnProjectile(float xPos, float yPos, float speed, float hspeed, float vspeed, float angle, float maxdmg, float mindmg, float crit, bool enemy)
{
    unique_ptr<Spear> p = make_unique<Spear>(s_proj);

    p.get()->xPos = xPos;
    p.get()->yPos = yPos;
    p.get()->speed = speed;
    p.get()->hspeed = hspeed;
    p.get()->vspeed = vspeed;
    p.get()->angle = angle;
    p.get()->maxdmg = maxdmg;
    p.get()->mindmg = mindmg;
    p.get()->crit = crit;
    p.get()->enemy = enemy;

    levelProjectiles.push_back(std::move(p));
}

void MissionController::addPickedItem(std::string spritesheet, int spritesheet_id, int picked_item)
{
    cout << "MissionController::addPickedItem(" << spritesheet << ", " << spritesheet_id << ", " << picked_item << ")" << endl;

    PickedItem tmp;
    tmp.circle.setFillColor(sf::Color(255,255,255,192));
    //tmp.circle.setRadius(50 * resRatioX);
    ///set radius in draw loop to get appropriate resratiox size
    tmp.item_id = picked_item;

    ///This unique entity needs to be loaded differently, read additional data for spritesheet info to be passed from the item registry.
    vector<char> di_data = droppeditem_spritesheet[spritesheet].retrieve_char();

    sf::Texture tex_obj;
    tex_obj.loadFromMemory(&di_data[0], di_data.size());
    tex_obj.setSmooth(true);

    tmp.item.setTexture(tex_obj);
    tmp.item.setTextureRect(droppeditem_spritesheet[spritesheet].retrieve_rect_as_map()[spritesheet_id-1]);
    tmp.bounds = sf::Vector2f(droppeditem_spritesheet[spritesheet].retrieve_rect_as_map()[spritesheet_id-1].width, droppeditem_spritesheet[spritesheet].retrieve_rect_as_map()[spritesheet_id-1].height);

    tmp.item.qualitySetting = qualitySetting;
    tmp.item.resSetting = resSetting;

    pickedItems.push_back(tmp);
}

void MissionController::submitPickedItems()
{
    for(int i=0; i<pickedItems.size(); i++)
    {
        InventoryItem invItem;
        invItem.item = v4core->savereader.itemreg.GetItemByID(pickedItems[i].item_id);
        invItem.inventoryId = v4core->savereader.invdata.items.size();
        v4core->savereader.invdata.items.push_back(invItem);

        if(pickedItems[i].item_id == 23)
        {
            if((!v4core->savereader.isMissionUnlocked(3)) && (!v4core->savereader.isMissionUnlocked(2)) && (v4core->savereader.locationsUnlocked == 1))
            {
                v4core->savereader.missionsUnlocked.push_back(2);
                v4core->savereader.locationsUnlocked = 2;
            }
        }
    }

}

void MissionController::updateMissions()
{
    ///When this function is called, the mission has been completed successfully
    cout << "MissionController::updateMissions(): " << curMissionID << endl;

    switch(curMissionID)
    {
        case 2:
        {
            if(!v4core->savereader.isMissionUnlocked(3))
            {
                v4core->savereader.missionsUnlocked.push_back(3);
                v4core->savereader.missionLevels[3] = 2;

                auto it = std::find(v4core->savereader.missionsUnlocked.begin(), v4core->savereader.missionsUnlocked.end(), 2);
                v4core->savereader.missionsUnlocked.erase(it);
            }

            break;
        }
    }

    if(v4core->savereader.missionLevels[curMissionID] != 0)
    v4core->savereader.missionLevels[curMissionID] += 1;
}

void MissionController::addUnitThumb(int unit_id)
{
    UnitThumb tmp;
    tmp.unit_id = unit_id;
    tmp.hpbar_back.loadFromFile("resources/graphics/mission/hpbar_back.png", qualitySetting, 1);
    tmp.hpbar_ins.loadFromFile("resources/graphics/mission/hpbar_ins.png", qualitySetting, 1);
    tmp.unit_count.createText(f_font, 26, sf::Color::White, "", qualitySetting, 1);
    tmp.unit_count_shadow.createText(f_font, 26, sf::Color::Black, "", qualitySetting, 1);
    tmp.width = tmp.hpbar_ins.getLocalBounds().width;
    unitThumbs.push_back(tmp);
}

void MissionController::Initialise(Config &config,std::string backgroundString,V4Core &v4core_)
{
    v4core = &v4core_;
    sf::Context context;

    PSprite ps_temp;
    ps_temp.loadFromFile("resources/graphics/item/icon/spear.png",1);
    ps_temp.setRepeated(false);
    ps_temp.setTextureRect(sf::IntRect(0,0,ps_temp.t.getSize().x,ps_temp.t.getSize().y)); ///affect later with ratio
    ps_temp.setOrigin(ps_temp.t.getSize().x,0);
    ps_temp.setColor(sf::Color(255,255,255,255));
    ps_temp.setPosition(0,0);

    s_proj = ps_temp;
    s_proj.scaleX=0.15f;
    s_proj.scaleY=0.15f;

    int q = config.GetInt("textureQuality");
    qualitySetting = q;
    resSetting = 1;

    dmg_spritesheet.load("resources/graphics/mission/damagesheet.png", q, 1);

    for(int i=tangibleLevelObjects.size()-1; i>=0; i--)
    {
        tangibleLevelObjects.erase(tangibleLevelObjects.begin()+i);
    }

    tangibleLevelObjects.clear();

    for(int i=units.size()-1; i>=0; i--)
    {
        units.erase(units.begin()+i);
    }

    units.clear();
    levelProjectiles.clear();

    droppeditem_spritesheet.clear();
    dmgCounters.clear();
    droppedItems.clear();
    pickedItems.clear();
    unitThumbs.clear();

    missionEnd = false;
    failure = false;

    //ctor
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    f_moji.loadFromFile("resources/fonts/mojipon.otf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    t_timerMenu.setFont(f_font);
    t_timerMenu.setCharacterSize(38);
    t_timerMenu.setFillColor(sf::Color::White);
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    //t_cutscene_text.setFont(f_font);

    //t_cutscene_text.setCharacterSize(35);
    //t_cutscene_text.setFillColor(sf::Color::White);
    //t_cutscene_text.setString(Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(L"intro_cutscene_1")));
    //t_cutscene_text.setOrigin(t_cutscene_text.getGlobalBounds().width/2,t_cutscene_text.getGlobalBounds().height/2);

    missionConfig = &config;

    isInitialized = true;
    // this is outside the loop
    startAlpha = 255;
    endAlpha = 0;
    targetTime = sf::seconds(10);


    fade.setPosition(sf::Vector2f(0,0));
    fade.setFillColor(sf::Color(0,0,0,0));
    fade.setSize(sf::Vector2f(800,600));
    currentCutsceneId=0;

    sb_win_jingle.loadFromFile("resources/sfx/level/victory.ogg");
    sb_lose_jingle.loadFromFile("resources/sfx/level/failure.ogg");

    sb_cheer1.loadFromFile("resources/sfx/level/cheer1.ogg");
    sb_cheer2.loadFromFile("resources/sfx/level/cheer2.ogg");
    sb_cheer3.loadFromFile("resources/sfx/level/cheer1.ogg");

    t_win.createText(f_moji, 56, sf::Color(222, 83, 0, 255), Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(L"mission_complete")), q, 1);
    t_win_outline.createText(f_moji, 56, sf::Color(255, 171, 0, 255), Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(L"mission_complete")), q, 1);
    t_win_outline.setOutlineColor(sf::Color(255, 171, 0, 255));
    t_win_outline.setOutlineThickness(10);
    t_lose.createText(f_moji, 56, sf::Color(138, 15, 26, 255), Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(L"mission_failed")), q, 1);
    t_lose_outline.createText(f_moji, 56, sf::Color(254, 48, 55, 255),Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(L"mission_failed")), q, 1);
    t_lose_outline.setOutlineColor(sf::Color(254, 48, 55, 255));
    t_lose_outline.setOutlineThickness(10);

    t_win.setOrigin(t_win.getLocalBounds().width/2, t_win.getLocalBounds().height/2);
    t_win_outline.setOrigin(t_win_outline.getLocalBounds().width/2, t_win_outline.getLocalBounds().height/2);

    t_lose.setOrigin(t_lose.getLocalBounds().width/2, t_lose.getLocalBounds().height/2);
    t_lose_outline.setOrigin(t_lose_outline.getLocalBounds().width/2, t_lose_outline.getLocalBounds().height/2);

    bar_win.loadFromFile("resources/graphics/mission/bar_win.png", q, 1);
    bar_lose.loadFromFile("resources/graphics/mission/bar_lose.png", q, 1);

    bar_win.setOrigin(bar_win.getLocalBounds().width/2, bar_win.getLocalBounds().height/2);
    bar_lose.setOrigin(bar_lose.getLocalBounds().width/2, bar_lose.getLocalBounds().height/2);

    ifstream sprdata("resources/graphics/item/itemdata/item_spritesheets.dat");
    string buff="";

    while(getline(sprdata, buff))
    {
        if(buff.find("#") == std::string::npos)
        {
            ///Valid spritesheet. Load it
            cout << "[Item spritesheets] Loading spritesheet " << "resources/graphics/item/itemdata/"+buff+".png" << endl;
            droppeditem_spritesheet[buff].load("resources/graphics/item/itemdata/"+buff+".png", qualitySetting, resSetting);
        }
    }

    ctrlTips.create(110, f_font, 28, sf::String(L"Onward: □-□-□-〇       Attack: 〇-〇-□-〇        Defend: △-△-□-〇              Charge: 〇-〇-△-△\nRetreat: 〇-□-〇-□          Jump: ×-×-△-△          Party: □-〇-×-△          Summon: ×-××-××") ,q, sf::Color(128,128,128,255));

    cout << "initialization finished" << endl;
}
void MissionController::StartMission(std::string missionFile, bool showCutscene, int missionID)
{
    missionConfig->thisCore->SaveToDebugLog("Starting mission");

    curMissionID = missionID;

    fade_alpha = 255;
    missionEnd = false;
    playJingle = false;
    textBounce = false;
    textCurX = -1280;
    barCurX = 1920;
    textDestX = 640;
    barDestX = 640;
    textCurScale = 1;
    textDestScale = 1;
    fade_alpha = 255;
    fadeout_alpha = 0;
    playCheer[0] = false;
    playCheer[1] = false;
    playCheer[2] = false;

    sf::Context context;
    int quality = missionConfig->GetInt("textureQuality");
    float ratioX, ratioY;

    army_X=0;
    camera.camera_x=480;
    showTimer = false;

    switch(quality)
    {
        case 0: ///low
        {
            ratioX = missionConfig->GetInt("resX") / float(640);
            ratioY = missionConfig->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = missionConfig->GetInt("resX") / float(1280);
            ratioY = missionConfig->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = missionConfig->GetInt("resX") / float(1920);
            ratioY = missionConfig->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = missionConfig->GetInt("resX") / float(3840);
            ratioY = missionConfig->GetInt("resY") / float(2160);
            break;
        }
    }

    pataponY = 720 - 141;
    floorY = 720 - 100;

    /**
    if(showCutscene)
    {
        cutscene_text_identifiers.push_back(L"intro_cutscene_1");
        cutscene_text_identifiers.push_back(L"intro_cutscene_2");
        cutscene_text_identifiers.push_back(L"intro_cutscene_3");
        cutscene_text_identifiers.push_back(L"intro_cutscene_4");
        cutscene_text_identifiers.push_back(L"intro_cutscene_5");
        cutscene_lengths.push_back(4);
        cutscene_lengths.push_back(4);
        cutscene_lengths.push_back(3);
        cutscene_lengths.push_back(3);
        cutscene_lengths.push_back(2);
        cutscene_blackscreens.push_back(true);
        cutscene_blackscreens.push_back(true);
        cutscene_blackscreens.push_back(true);
        cutscene_blackscreens.push_back(true);
        cutscene_blackscreens.push_back(true);
        currentCutsceneId=0;
        cutscenesLeft=true;
        isFinishedLoading=true;

    }
    else
    {
        inCutscene = false;
        cutscene_blackscreens.clear();
        cutscene_lengths.clear();
        cutscene_text_identifiers.clear();
        cutscenesLeft=false;
    }*/

    tangibleLevelObjects.clear();
    levelProjectiles.clear();

    string bgName; ///background
    string songName; ///bgm
    string missionName; ///rpc_name
    string missionImg; ///rpc_img

    string buff;

    ifstream elist("resources/units/entitylist.dat");

    vector<string> entity_list;

    while(getline(elist, buff))
    {
        if(buff[0] != '#')
        {
            if(buff.size() > 0)
            {
                entity_list.push_back(buff.substr(buff.find_last_of(",")+1));
            }
        }
    }

    elist.close();

    ifstream mf("resources/missions/"+missionFile);
    cout << "Attempting to read a mission " << "resources/missions/" << missionFile << endl;

    bool accepted = false;
    float ver = 0.0;

    while(getline(mf, buff))
    {
        if(buff[0] != '#')
        {
            if(buff.size() > 0)
            {
                cout << "[P4M] " << buff << endl;

                if(buff.find("Patafour Mission Format v1.0") != std::string::npos)
                {
                    cout << "Reading Patafour Mission Format v1.0" << endl;

                    accepted = true;
                    ver = 1.0;
                }

                if(accepted)
                {
                    if(ver == 1.0)
                    {
                        if(buff.find("background=") != std::string::npos)
                        {
                            bgName = buff.substr(buff.find_first_of("=")+1);
                        }

                        if(buff.find("bgm=") != std::string::npos)
                        {
                            songName = buff.substr(buff.find_first_of("=")+1);
                        }

                        if(buff.find("rpc_name=") != std::string::npos)
                        {
                            missionName = buff.substr(buff.find_first_of("=")+1);
                        }

                        if(buff.find("rpc_img=") != std::string::npos)
                        {
                            missionImg = buff.substr(buff.find_first_of("=")+1);
                        }

                        if(buff.find("spawn=") != std::string::npos)
                        {
                            cout << "Found a spawn registry. Spawning entity" << endl;

                            string sp = buff.substr(buff.find_first_of("=")+1);
                            vector<string> spawn = Func::Split(sp, ',');

                            cout << "Entity ID: " << spawn[0] << endl;

                            int entityID = atoi(spawn[0].c_str());
                            int baseY = 0;
                            int baseHP = 0;
                            bool collidable = false;
                            bool attackable = false;
                            vector<Entity::Loot> loot_table;

                            ifstream entityParam("resources/units/entity/"+entity_list[entityID]+".p4p");
                            string buff2;

                            while(getline(entityParam, buff2))
                            {
                                if(buff2[0] != '#')
                                {
                                    if(buff2.size() > 0)
                                    {
                                        if(buff2.find("baseY=") != std::string::npos)
                                        {
                                            string by = buff2.substr(buff2.find_last_of("=")+1);
                                            baseY = atoi(by.c_str());
                                        }

                                        if(buff2.find("baseHP=") != std::string::npos)
                                        {
                                            string by = buff2.substr(buff2.find_last_of("=")+1);
                                            baseHP = atoi(by.c_str());
                                        }

                                        if(buff2.find("collidable=") != std::string::npos)
                                        {
                                            string by = buff2.substr(buff2.find_last_of("=")+1);
                                            collidable = atoi(by.c_str());
                                        }

                                        if(buff2.find("attackable=") != std::string::npos)
                                        {
                                            string by = buff2.substr(buff2.find_last_of("=")+1);
                                            attackable = atoi(by.c_str());
                                        }

                                        if(buff2.find("loot=") != std::string::npos)
                                        {
                                            string by = buff2.substr(buff2.find_last_of("=")+1);
                                            vector<string> b = Func::Split(by,';');

                                            for(int e=0; e<b.size(); e++)
                                            {
                                                vector<string> bb = Func::Split(b[e], ',');

                                                Entity::Loot tmp;
                                                tmp.item_id = stoi(bb[0]);
                                                tmp.item_chance = stoi(bb[1]);
                                                loot_table.push_back(tmp);

                                                cout << "[Loot table] Item: " << v4core->savereader.itemreg.GetItemByID(tmp.item_id)->icon_path << " with " << tmp.item_chance << "% drop rate" << endl;
                                            }
                                        }
                                    }
                                }
                            }

                            entityParam.close();

                            vector<string> additional_data;

                            if(spawn.size() >= 15)
                            {
                                ///additional data exists
                                additional_data = Func::Split(spawn[14], ';');
                            }

                            cout << "Spawning an entity: " << entity_list[entityID] << endl;
                            spawnEntity(entity_list[entityID],entityID,baseHP,atoi(spawn[1].c_str()),atoi(spawn[2].c_str()),baseY,atoi(spawn[3].c_str()),atoi(spawn[4].c_str()),atoi(spawn[5].c_str()),sf::Color(atoi(spawn[6].c_str()),atoi(spawn[7].c_str()),atoi(spawn[8].c_str()),atoi(spawn[9].c_str())), collidable, attackable, atoi(spawn[10].c_str()), atoi(spawn[13].c_str()), atoi(spawn[12].c_str()), atoi(spawn[11].c_str()), loot_table, additional_data);
                        }
                    }
                }
            }
        }
    }

    mf.close();

    ///make this unit load based on how the army is built later

    unique_ptr<Patapon> p1 = make_unique<Patapon>();
    unique_ptr<Patapon> p2 = make_unique<Patapon>();
    unique_ptr<Patapon> p3 = make_unique<Patapon>();
    unique_ptr<Hatapon> h = make_unique<Hatapon>();

    p1.get()->LoadConfig(missionConfig);
    p2.get()->LoadConfig(missionConfig);
    p3.get()->LoadConfig(missionConfig);
    h.get()->LoadConfig(missionConfig);

    p1.get()->setUnitID(1);
    p2.get()->setUnitID(1);
    p3.get()->setUnitID(1);
    h.get()->setUnitID(0);

    ///Apply the stats
    p1.get()->mindmg = v4core->savereader.ponreg.GetPonByID(1)->pon_min_dmg;
    p1.get()->maxdmg = v4core->savereader.ponreg.GetPonByID(1)->pon_max_dmg;

    p2.get()->mindmg = v4core->savereader.ponreg.GetPonByID(2)->pon_min_dmg;
    p2.get()->maxdmg = v4core->savereader.ponreg.GetPonByID(2)->pon_max_dmg;

    p3.get()->mindmg = v4core->savereader.ponreg.GetPonByID(3)->pon_min_dmg;
    p3.get()->maxdmg = v4core->savereader.ponreg.GetPonByID(3)->pon_max_dmg;

    p1.get()->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(1)->weapon_invItem_id).item->equip_id);
    p2.get()->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(2)->weapon_invItem_id).item->equip_id);
    p3.get()->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(3)->weapon_invItem_id).item->equip_id);


    p1.get()->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(1)->weapon_invItem_id).item->equip_id);
    p2.get()->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(2)->weapon_invItem_id).item->equip_id);
    p3.get()->applySpear(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(3)->weapon_invItem_id).item->equip_id);

    p1.get()->applyHelm(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(1)->armour_invItem_id).item->equip_id);
    p2.get()->applyHelm(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(2)->armour_invItem_id).item->equip_id);
    p3.get()->applyHelm(v4core->savereader.invdata.GetItemByInvID(v4core->savereader.ponreg.GetPonByID(3)->armour_invItem_id).item->equip_id);

    units.push_back(std::move(p1));
    units.push_back(std::move(p2));
    units.push_back(std::move(p3));
    units.push_back(std::move(h));

    addUnitThumb(0);
    addUnitThumb(1);

    isFinishedLoading=true;
    v4core->LoadingWaitForKeyPress();

    cout << "Loading background " << bgName << endl;
    test_bg.Load(bgName, *missionConfig);//config.GetString("debugBackground"));

    string fm = "Playing mission: "+missionName;
    v4core->ChangeRichPresence(fm.c_str(), missionImg.c_str(), "logo");

    rhythm.config = *missionConfig;
    rhythm.LoadTheme(songName); // missionConfig->GetString("debugTheme")
    missionTimer.restart();

    cout << "MissionController::StartMission(): finished" << endl;
    missionConfig->thisCore->SaveToDebugLog("Mission loading finished.");
}
void MissionController::StopMission()
{
    rhythm.Stop();
    isInitialized = false;
}
void MissionController::DoKeyboardEvents(sf::RenderWindow &window, float fps, InputController& inputCtrl)
{
    /**
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
    {
        missionEnd = true;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
    {
        if(!debug_map_drop)
        {
            auto item = v4core->savereader.itemreg.GetItemByID(23);
            vector<string> data = {item->spritesheet, to_string(item->spritesheet_id), to_string(23)};

            spawnEntity("droppeditem",5,0,500,0,600,0,0,1,sf::Color::White,0,0,vector<Entity::Loot>(), data);

            debug_map_drop = true;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        for(int i=0; i<units.size(); i++)
        {

                units[i].get()->setUnitHP(units[i].get()->getUnitHP() - (25.0/fps));

                if(units[i].get()->getUnitHP() <= 0)
                {
                    units[i].get()->setUnitHP(0);
                }
        }
    }**/


    if((inputCtrl.isKeyHeld(InputController::Keys::LTRIGGER)) && (inputCtrl.isKeyHeld(InputController::Keys::RTRIGGER)) && (inputCtrl.isKeyHeld(InputController::Keys::SQUARE)))
    {
        if(inputCtrl.isKeyPressed(InputController::Keys::SELECT))
        {
            std::vector<std::string> a = {"Show hitboxes","Hide hitboxes","Heal units"};

            PataDialogBox db;
            db.Create(f_font, "Debug menu", a, missionConfig->GetInt("textureQuality"));
            db.id = 999;
            dialogboxes.push_back(db);
        }
    }
    else if(inputCtrl.isKeyPressed(InputController::Keys::START))
    {
        std::vector<std::string> a = {Func::ConvertToUtf8String(missionConfig->strRepo.GetUnicodeString(L"nav_yes")),Func::ConvertToUtf8String(missionConfig->strRepo.GetUnicodeString(L"nav_no"))};

        PataDialogBox db;
        db.Create(f_font, Func::ConvertToUtf8String(missionConfig->strRepo.GetUnicodeString(L"mission_backtopatapolis")), a, missionConfig->GetInt("textureQuality"));
        dialogboxes.push_back(db);
    }
}

vector<MissionController::CollisionEvent> MissionController::DoCollisionForObject(HitboxFrame* currentObjectHitBoxFrame,float currentObjectX,float currentObjectY,int collisionObjectID,vector<string> collisionData)
{
    ///Added vector because there can be more than just one collision events! Like colliding with grass AND with opponent
    vector<MissionController::CollisionEvent> collisionEvents;

    for(int i=0; i<tangibleLevelObjects.size(); i++)
    {
        for(int h=0; h<tangibleLevelObjects[i]->hitboxes.size(); h++)
        {
            //cout << "tangibleLevelObjects[" << i << "][" << h << "]" << endl;

            /// NEW COLLISION SYSTEM:
            /// Separating axis theorem
            /// we check an axis at a time
            /// 8 axes in total, aligned with the normal of each face of each shape
            /// thankfully because we are only using rectangles, there are two pairs of parallel sides
            /// so we only need to check 4 axes, as the other 4 are all parallel.
            ///
            /// in each axis we calculate the vector projection onto the axis between the origin and each corner of each box
            /// and find the maximum projection and minimum projection for each shape
            /// then we check if min2>max1 or min1>max2 there has been a collision in this axis
            /// there has to be a collision in ALL axes for actual collision to be confirmed,
            /// so we can stop checking if we find a single non-collision.




            /// axis 1: obj1 "sideways" We start with sideways because it is less likely to contain a collision
            //cout<<"Collision step for projectile at X: "<<currentObjectX<<" against object at x: "<<tangibleLevelObjects[i]->global_x<<endl;

            float currentAxisAngle = currentObjectHitBoxFrame->rotation;
            vector<sf::Vector2f> currentVertices = currentObjectHitBoxFrame->getCurrentVertices();
            PVector pv1 = PVector::getVectorCartesian(0,0,currentVertices[0].x+currentObjectX,currentVertices[0].y+currentObjectY);
            PVector pv2 = PVector::getVectorCartesian(0,0,currentVertices[1].x+currentObjectX,currentVertices[1].y+currentObjectY);
            PVector pv3 = PVector::getVectorCartesian(0,0,currentVertices[2].x+currentObjectX,currentVertices[2].y+currentObjectY);
            PVector pv4 = PVector::getVectorCartesian(0,0,currentVertices[3].x+currentObjectX,currentVertices[3].y+currentObjectY);

            pv1.angle =-atan2(currentVertices[0].y+currentObjectY, currentVertices[0].x+currentObjectX);
            pv2.angle =-atan2(currentVertices[1].y+currentObjectY, currentVertices[1].x+currentObjectX);
            pv3.angle =-atan2(currentVertices[2].y+currentObjectY, currentVertices[2].x+currentObjectX);
            pv4.angle =-atan2(currentVertices[3].y+currentObjectY, currentVertices[3].x+currentObjectX);

            float proj1 = pv1.GetScalarProjectionOntoAxis(currentAxisAngle);
            float proj2 = pv2.GetScalarProjectionOntoAxis(currentAxisAngle);
            float proj3 = pv3.GetScalarProjectionOntoAxis(currentAxisAngle);
            float proj4 = pv4.GetScalarProjectionOntoAxis(currentAxisAngle);

            CollidableObject* target = tangibleLevelObjects[i].get();

            bool isCollision = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,currentObjectHitBoxFrame,currentObjectX,currentObjectY);
            if (!isCollision)
            {
                continue;
            }
            //cout<<"COLLISION FOUND IN axis 1"<<endl;

            /// axis 2: obj1 "up"
            currentAxisAngle = 3.14159265358/2+currentObjectHitBoxFrame->rotation;
            bool isCollision2 = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,currentObjectHitBoxFrame,currentObjectX,currentObjectY);
            if (!isCollision2)
            {
                continue;
            }
            //cout<<"COLLISION FOUND IN axis 2 (up)"<<endl;

            /// axis 3: obj2 "up" (we add the 90 degrees from before to its current rotation)
            currentAxisAngle = target->hitboxes[h].hitboxObject.rotation + 3.14159265358/2;

            bool isCollision3 = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,currentObjectHitBoxFrame,currentObjectX,currentObjectY);
            if (!isCollision3)
            {
                continue;
            }
            //cout<<"COLLISION FOUND IN axis 3 (up2)"<<endl;

            /// axis 4: obj2 "sideways"
            currentAxisAngle = target->hitboxes[h].hitboxObject.rotation;

            bool isCollision4 = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,currentObjectHitBoxFrame,currentObjectX,currentObjectY);
            if (!isCollision4)
            {
                continue;
            }

            /// we have a collision
            if (isCollision&&isCollision2&&isCollision3&&isCollision4)
            {
                std::cout << "[COLLISION_SYSTEM]: Found a collision"<<endl;
                CollisionEvent cevent;
                cevent.collided = true;
                //cevent.collidedEntityID = -1;
                cevent.isAttackable = target->isAttackable;
                cevent.isCollidable = target->isCollidable;

                target->OnCollide(target, collisionObjectID, collisionData);

                collisionEvents.push_back(cevent);
            }
            else
            {
                cout<<"Something is very wrong"<<endl;
            }
        }
    }

    return collisionEvents;
}

vector<MissionController::CollisionEvent> MissionController::DoCollisionForUnit(HitboxFrame* currentObjectHitBoxFrame,float currentObjectX,float currentObjectY,int collisionObjectID,vector<string> collisionData)
{
    ///Added vector because there can be more than just one collision events! Like colliding with grass AND with opponent
    vector<MissionController::CollisionEvent> collisionEvents;

    for(int i=0; i<units.size(); i++)
    {
        for(int h=0; h<units[i]->hitboxes.size(); h++)
        {
            //cout << "tangibleLevelObjects[" << i << "][" << h << "]" << endl;

            /// NEW COLLISION SYSTEM:
            /// Separating axis theorem
            /// we check an axis at a time
            /// 8 axes in total, aligned with the normal of each face of each shape
            /// thankfully because we are only using rectangles, there are two pairs of parallel sides
            /// so we only need to check 4 axes, as the other 4 are all parallel.
            ///
            /// in each axis we calculate the vector projection onto the axis between the origin and each corner of each box
            /// and find the maximum projection and minimum projection for each shape
            /// then we check if min2>max1 or min1>max2 there has been a collision in this axis
            /// there has to be a collision in ALL axes for actual collision to be confirmed,
            /// so we can stop checking if we find a single non-collision.




            /// axis 1: obj1 "sideways" We start with sideways because it is less likely to contain a collision
            //cout<<"Collision step for projectile at X: "<<currentObjectX<<" against object at x: "<<tangibleLevelObjects[i]->global_x<<endl;

            float currentAxisAngle = currentObjectHitBoxFrame->rotation;
            vector<sf::Vector2f> currentVertices = currentObjectHitBoxFrame->getCurrentVertices();
            PVector pv1 = PVector::getVectorCartesian(0,0,currentVertices[0].x+currentObjectX,currentVertices[0].y+currentObjectY);
            PVector pv2 = PVector::getVectorCartesian(0,0,currentVertices[1].x+currentObjectX,currentVertices[1].y+currentObjectY);
            PVector pv3 = PVector::getVectorCartesian(0,0,currentVertices[2].x+currentObjectX,currentVertices[2].y+currentObjectY);
            PVector pv4 = PVector::getVectorCartesian(0,0,currentVertices[3].x+currentObjectX,currentVertices[3].y+currentObjectY);

            pv1.angle =-atan2(currentVertices[0].y+currentObjectY, currentVertices[0].x+currentObjectX);
            pv2.angle =-atan2(currentVertices[1].y+currentObjectY, currentVertices[1].x+currentObjectX);
            pv3.angle =-atan2(currentVertices[2].y+currentObjectY, currentVertices[2].x+currentObjectX);
            pv4.angle =-atan2(currentVertices[3].y+currentObjectY, currentVertices[3].x+currentObjectX);

            float proj1 = pv1.GetScalarProjectionOntoAxis(currentAxisAngle);
            float proj2 = pv2.GetScalarProjectionOntoAxis(currentAxisAngle);
            float proj3 = pv3.GetScalarProjectionOntoAxis(currentAxisAngle);
            float proj4 = pv4.GetScalarProjectionOntoAxis(currentAxisAngle);

            CollidableObject* target = units[i].get();

            bool isCollision = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,currentObjectHitBoxFrame,currentObjectX,currentObjectY);
            if (!isCollision)
            {
                continue;
            }
            //cout<<"COLLISION FOUND IN axis 1"<<endl;

            /// axis 2: obj1 "up"
            currentAxisAngle = 3.14159265358/2+currentObjectHitBoxFrame->rotation;
            bool isCollision2 = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,currentObjectHitBoxFrame,currentObjectX,currentObjectY);
            if (!isCollision2)
            {
                continue;
            }
            //cout<<"COLLISION FOUND IN axis 2 (up)"<<endl;

            /// axis 3: obj2 "up" (we add the 90 degrees from before to its current rotation)
            currentAxisAngle = target->hitboxes[h].hitboxObject.rotation + 3.14159265358/2;

            bool isCollision3 = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,currentObjectHitBoxFrame,currentObjectX,currentObjectY);
            if (!isCollision3)
            {
                continue;
            }
            //cout<<"COLLISION FOUND IN axis 3 (up2)"<<endl;

            /// axis 4: obj2 "sideways"
            currentAxisAngle = target->hitboxes[h].hitboxObject.rotation;

            bool isCollision4 = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,currentObjectHitBoxFrame,currentObjectX,currentObjectY);
            if (!isCollision4)
            {
                continue;
            }

            /// we have a collision
            if (isCollision&&isCollision2&&isCollision3&&isCollision4)
            {
                std::cout << "[COLLISION_SYSTEM]: Found a collision"<<endl;
                CollisionEvent cevent;
                cevent.collided = true;
                //cevent.collidedEntityID = -1;
                cevent.isAttackable = target->isAttackable;
                cevent.isCollidable = target->isCollidable;

                target->OnCollide(target, collisionObjectID, collisionData);

                collisionEvents.push_back(cevent);
            }
            else
            {
                cout<<"Something is very wrong"<<endl;
            }
        }
    }

    return collisionEvents;
}

float MissionController::pataponMaxProjection(float axisAngle, int id)
{
    PlayableUnit* target = units[id].get();

    float currentAxisAngle = 0;
    HitboxFrame tmp = target->hitboxes[0].getRect();

    std::vector<sf::Vector2f> currentVertices = tmp.getCurrentVertices();

    PVector pv1 = PVector::getVectorCartesian(0,0,currentVertices[0].x+target->getGlobalPosition().x,currentVertices[0].y+target->getGlobalPosition().y);
    PVector pv2 = PVector::getVectorCartesian(0,0,currentVertices[1].x+target->getGlobalPosition().x,currentVertices[1].y+target->getGlobalPosition().y);
    PVector pv3 = PVector::getVectorCartesian(0,0,currentVertices[2].x+target->getGlobalPosition().x,currentVertices[2].y+target->getGlobalPosition().y);
    PVector pv4 = PVector::getVectorCartesian(0,0,currentVertices[3].x+target->getGlobalPosition().x,currentVertices[3].y+target->getGlobalPosition().y);

    pv1.angle =-atan2(currentVertices[0].y+target->getGlobalPosition().y, currentVertices[0].x+target->getGlobalPosition().x);
    pv2.angle =-atan2(currentVertices[1].y+target->getGlobalPosition().y, currentVertices[1].x+target->getGlobalPosition().x);
    pv3.angle =-atan2(currentVertices[2].y+target->getGlobalPosition().y, currentVertices[2].x+target->getGlobalPosition().x);
    pv4.angle =-atan2(currentVertices[3].y+target->getGlobalPosition().y, currentVertices[3].x+target->getGlobalPosition().x);

    float proj1 = pv1.GetScalarProjectionOntoAxis(axisAngle);
    float proj2 = pv2.GetScalarProjectionOntoAxis(axisAngle);
    float proj3 = pv3.GetScalarProjectionOntoAxis(axisAngle);
    float proj4 = pv4.GetScalarProjectionOntoAxis(axisAngle);

    /*if(axisAngle!=0){
        cout<<"NEW MAX TEST"<<endl;
        cout<<"Angle: "<<pv1.angle<<" distance: "<<pv1.distance<<" current X: "<<currentVertices[0].x+target->x<<" current Y: "<<currentVertices[0].y+target->y<<" proj: "<<proj1<<endl;
        cout<<"Angle: "<<pv2.angle<<" distance: "<<pv2.distance<<" current X: "<<currentVertices[1].x+target->x<<" current Y: "<<currentVertices[1].y+target->y<<" proj: "<<proj2<<endl;
        cout<<"Angle: "<<pv3.angle<<" distance: "<<pv3.distance<<" current X: "<<currentVertices[2].x+target->x<<" current Y: "<<currentVertices[2].y+target->y<<" proj: "<<proj3<<endl;
        cout<<"Angle: "<<pv4.angle<<" distance: "<<pv4.distance<<" current X: "<<currentVertices[3].x+target->x<<" current Y: "<<currentVertices[3].y+target->y<<" proj: "<<proj4<<endl;
    }*/
    float maxProjectionObj1 = max(max(max(proj1,proj2),proj3),proj4);
    float minProjectionObj1 = min(min(min(proj1,proj2),proj3),proj4);
    return maxProjectionObj1;
}

float MissionController::pataponMinProjection(float axisAngle, int id)
{
    PlayableUnit* target = units[id].get();

    float currentAxisAngle = 0;
    HitboxFrame tmp = target->hitboxes[0].getRect();

    std::vector<sf::Vector2f> currentVertices = tmp.getCurrentVertices();

    PVector pv1 = PVector::getVectorCartesian(0,0,currentVertices[0].x+target->getGlobalPosition().x,currentVertices[0].y+target->getGlobalPosition().y);
    PVector pv2 = PVector::getVectorCartesian(0,0,currentVertices[1].x+target->getGlobalPosition().x,currentVertices[1].y+target->getGlobalPosition().y);
    PVector pv3 = PVector::getVectorCartesian(0,0,currentVertices[2].x+target->getGlobalPosition().x,currentVertices[2].y+target->getGlobalPosition().y);
    PVector pv4 = PVector::getVectorCartesian(0,0,currentVertices[3].x+target->getGlobalPosition().x,currentVertices[3].y+target->getGlobalPosition().y);
    pv1.angle =-atan2(currentVertices[0].y+target->getGlobalPosition().y, currentVertices[0].x+target->getGlobalPosition().x);
    pv2.angle =-atan2(currentVertices[1].y+target->getGlobalPosition().y, currentVertices[1].x+target->getGlobalPosition().x);
    pv3.angle =-atan2(currentVertices[2].y+target->getGlobalPosition().y, currentVertices[2].x+target->getGlobalPosition().x);
    pv4.angle =-atan2(currentVertices[3].y+target->getGlobalPosition().y, currentVertices[3].x+target->getGlobalPosition().x);

    float proj1 = pv1.GetScalarProjectionOntoAxis(axisAngle);
    float proj2 = pv2.GetScalarProjectionOntoAxis(axisAngle);
    float proj3 = pv3.GetScalarProjectionOntoAxis(axisAngle);
    float proj4 = pv4.GetScalarProjectionOntoAxis(axisAngle);


    float maxProjectionObj1 = max(max(max(proj1,proj2),proj3),proj4);
    float minProjectionObj1 = min(min(min(proj1,proj2),proj3),proj4);
    return minProjectionObj1;
}
bool MissionController::DoCollisionStepInAxis(float currentAxisAngle,HitboxFrame* currentHitboxFrame,AnimatedObject* targetObject, HitboxFrame* currentObjectHitBoxFrame,float currentObjectX,float currentObjectY)
{
    std::vector<sf::Vector2f> currentVertices = currentObjectHitBoxFrame->getCurrentVertices();

    if(currentVertices.size() < 4)
    cout << "Vertices alert!!! " << currentVertices.size() << endl;

    if(currentVertices.size() >= 4)
    {
        PVector pv1 = PVector::getVectorCartesian(0,0,currentVertices[0].x+currentObjectX,currentVertices[0].y+currentObjectY);
        PVector pv2 = PVector::getVectorCartesian(0,0,currentVertices[1].x+currentObjectX,currentVertices[1].y+currentObjectY);
        PVector pv3 = PVector::getVectorCartesian(0,0,currentVertices[2].x+currentObjectX,currentVertices[2].y+currentObjectY);
        PVector pv4 = PVector::getVectorCartesian(0,0,currentVertices[3].x+currentObjectX,currentVertices[3].y+currentObjectY);
        pv1.angle =-atan2(currentVertices[0].y+currentObjectY, currentVertices[0].x+currentObjectX);
        pv2.angle =-atan2(currentVertices[1].y+currentObjectY, currentVertices[1].x+currentObjectX);
        pv3.angle =-atan2(currentVertices[2].y+currentObjectY, currentVertices[2].x+currentObjectX);
        pv4.angle =-atan2(currentVertices[3].y+currentObjectY, currentVertices[3].x+currentObjectX);

        float proj1 = pv1.GetScalarProjectionOntoAxis(currentAxisAngle);
        float proj2 = pv2.GetScalarProjectionOntoAxis(currentAxisAngle);
        float proj3 = pv3.GetScalarProjectionOntoAxis(currentAxisAngle);
        float proj4 = pv4.GetScalarProjectionOntoAxis(currentAxisAngle);


        float maxProjectionObj1 = max(max(max(proj1,proj2),proj3),proj4);
        float minProjectionObj1 = min(min(min(proj1,proj2),proj3),proj4);

        float maxProjectionObj2 = currentHitboxFrame->maxProjection(currentAxisAngle, targetObject->global_x+targetObject->local_x,targetObject->global_y+targetObject->local_y);
        float minProjectionObj2 = currentHitboxFrame->minProjection(currentAxisAngle, targetObject->global_x+targetObject->local_x,targetObject->global_y+targetObject->local_y);
        if(maxProjectionObj1>minProjectionObj2 && minProjectionObj1<maxProjectionObj2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
void MissionController::DoMovement(sf::RenderWindow &window, float fps, InputController& inputCtrl)
{
    /** Make Patapon walk (temporary) **/
    float booster=1.0;
    if (rhythm.current_perfect == 4)
    {
        booster=1.2;
    }
    if(missionEnd)
    {
        booster = 1.0;
    }

    /** Find the farthest unit in your army (for calculations) **/
    int farthest_id = -1;
    float temp_pos = -9999;

    for(int i=0; i<units.size(); i++)
    {
        PlayableUnit* unit = units[i].get();

        if(temp_pos <= unit->getGlobalPosition().x+unit->local_x)
        {
            temp_pos = unit->getGlobalPosition().x+unit->local_x;
            farthest_id = i;
        }
    }

    /** Patapon movement **/

    if(farthest_id != -1)
    {
        PlayableUnit* farthest_unit = units[farthest_id].get();

        bool foundCollision = false;

        for(int i=0; i<tangibleLevelObjects.size(); i++)
        {
            for(int h=0; h<tangibleLevelObjects[i]->hitboxes.size(); h++)
            {
                //cout << "tangibleLevelObjects[" << i << "][" << h << "]" << endl;

                float proposedXPos = farthest_unit->getGlobalPosition().x + farthest_unit->local_x + 40;

                /// NEW COLLISION SYSTEM:
                /// Separating axis theorem
                /// we check an axis at a time
                /// 8 axes in total, aligned with the normal of each face of each shape
                /// thankfully because we are only using rectangles, there are two pairs of parallel sides
                /// so we only need to check 4 axes, as the other 4 are all parallel.
                ///
                /// in each axis we calculate the vector projection onto the axis between the origin and each corner of each box
                /// and find the maximum projection and minimum projection for each shape
                /// then we check if min2>max1 or min1>max2 there has been a collision in this axis
                /// there has to be a collision in ALL axes for actual collision to be confirmed,
                /// so we can stop checking if we find a single non-collision.

                /// axis 1: obj1 "sideways" We start with sideways because it is less likely to contain a collision

                float currentAxisAngle = 0;
                HitboxFrame tmp = farthest_unit->hitboxes[0].getRect();

                CollidableObject* target = tangibleLevelObjects[i].get();

                bool isCollision = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,&tmp,proposedXPos,farthest_unit->getGlobalPosition().y);
                if (!isCollision)
                {
                    continue;
                }
                //cout<<"COLLISION FOUND IN axis 1"<<endl;

                /// axis 2: obj1 "up"
                currentAxisAngle = 3.14159265358/2;
                bool isCollision2 = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,&tmp,proposedXPos,farthest_unit->getGlobalPosition().y);
                if (!isCollision2)
                {
                    continue;
                }
                //cout<<"COLLISION FOUND IN axis 2 (up)"<<endl;

                /// axis 3: obj2 "up" (we add the 90 degrees from before to its current rotation)
                currentAxisAngle = target->hitboxes[h].hitboxObject.rotation + currentAxisAngle;

                bool isCollision3 = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,&tmp,proposedXPos,farthest_unit->getGlobalPosition().y);
                if (!isCollision3)
                {
                    continue;
                }
                //cout<<"COLLISION FOUND IN axis 3 (up2)"<<endl;

                /// axis 4: obj2 "sideways"
                currentAxisAngle = target->hitboxes[h].hitboxObject.rotation;

                bool isCollision4 = DoCollisionStepInAxis(currentAxisAngle,&(target->hitboxes[h].hitboxObject),target,&tmp,proposedXPos,farthest_unit->getGlobalPosition().y);
                if (!isCollision4)
                {
                    continue;
                }

                /// we have a collision
                if (isCollision&&isCollision2&&isCollision3&&isCollision4)
                {
                    ///check if unit should be prevented from passing through
                    if(target->isCollidable)
                    foundCollision = true;

                    ///the entity can still react
                    target->OnCollide(target);

                    std::cout << "[COLLISION_SYSTEM]: Found a collision"<<endl;
                }
                else
                {
                    cout<<"Something is very wrong"<<endl;
                }
            }
        }

        if((camera.walk) || ((missionEnd) && (!failure)))
        {
            float pataDistance = 240 * booster;

            float diff = (Smoothstep(walkClock.getElapsedTime().asSeconds()/2)*pataDistance)-(Smoothstep(prevTime/2)*pataDistance);
            prevTime = walkClock.getElapsedTime().asSeconds();

            float proposedXPos = farthest_unit->getGlobalPosition().x + diff;

            camera.pataSpeed = (2 * 60 * booster);

            //cout << "global_x: " << farthest_unit->global_x << endl;
            //cout << "proposedXPos = " << proposedXPos << endl;
            /// use the right hand side of the patapon sprite to check for collisions. This should be changed if the patapon walks to the left
            //float proposedXPosRight = proposedXPos + farthest_unit->hitBox.left + farthest_unit->hitBox.width;
            /// need to have it check for collision and stop if blocked by kacheek here.

            /// right now it is very basic checking only in X axis. Jumping over a
            /// kacheek will not be possible.

            /// if the new position is inside a kacheek, don't move. If we found anything,
            if (!foundCollision)
            {
                if(!missionEnd)
                army_X += diff;
                else
                army_X += 120.0 / fps;
            }
        }
    }

    /** Set global positions for the units **/

    for(int i=0; i<units.size(); i++)
    {
        PlayableUnit* unit = units[i].get();

        if(!unit->dead)
        {
            if(unit->local_x < unit->dest_local_x)
            unit->local_x += 200.0 / fps;
            if(unit->local_x > unit->dest_local_x)
            unit->local_x -= 200.0 / fps;
        }

        switch(unit->getUnitID())
        {
            case 0: ///Hatapon
            {
                unit->setGlobalPosition(sf::Vector2f(army_X,500));
                break;
            }

            case 1: ///Yaripon
            {
                unit->setGlobalPosition(sf::Vector2f(army_X+100+(50*i),pataponY));
                break;
            }
        }
    }
}

void MissionController::DoRhythm(InputController& inputCtrl)
{
    /** Call Rhythm functions **/

        if(rhythm.current_song == "patapata")
        {
            //cout << "set walk true" << endl;
            camera.walk = true;

            if(!startWalking)
            {
                walkClock.restart();
                prevTime = 0;

                startWalking = true;
            }
        }
        else
        {
            //cout << "set walk false" << endl;
            camera.walk = false;

            startWalking = false;
        }

        if((rhythm.rhythmController.current_drum == "pata") or (rhythm.rhythmController.current_drum == "pon") or (rhythm.rhythmController.current_drum == "chaka") or (rhythm.rhythmController.current_drum == "don"))
        {
            rhythm.rhythmController.current_drum = "";
            rhythm.current_song = "";
        }

        rhythm.rhythmController.config = *missionConfig;
        rhythm.config = *missionConfig;

        rhythm.doRhythm(inputCtrl);
}

void MissionController::DoMissionEnd(sf::RenderWindow& window, float fps)
{
    /** Make the missionEndTimer unusable until the mission is not finished **/
    if(!missionEnd)
    missionEndTimer.restart();

    /** Mission end cheering **/

    if(missionEnd)
    {
        if(!failure)
        {
            if(missionEndTimer.getElapsedTime().asMilliseconds() >= 2500)
            {
                if(!playCheer[0])
                {
                    s_cheer.stop();
                    s_cheer.setBuffer(sb_cheer1);
                    s_cheer.setVolume(float(missionConfig->GetInt("masterVolume"))*(float(missionConfig->GetInt("sfxVolume"))/100.f));
                    s_cheer.play();
                    playCheer[0] = true;
                }
            }

            if(missionEndTimer.getElapsedTime().asMilliseconds() >= 4500)
            {
                if(!playCheer[1])
                {
                    s_cheer.stop();
                    s_cheer.setBuffer(sb_cheer2);
                    s_cheer.setVolume(float(missionConfig->GetInt("masterVolume"))*(float(missionConfig->GetInt("sfxVolume"))/100.f));
                    s_cheer.play();
                    playCheer[1] = true;
                }
            }

            if(missionEndTimer.getElapsedTime().asMilliseconds() >= 6500)
            {
                if(!playCheer[2])
                {
                    s_cheer.stop();
                    s_cheer.setBuffer(sb_cheer3);
                    s_cheer.setVolume(float(missionConfig->GetInt("masterVolume"))*(float(missionConfig->GetInt("sfxVolume"))/100.f));
                    s_cheer.play();
                    playCheer[2] = true;
                }
            }

            if(missionEndTimer.getElapsedTime().asMilliseconds() >= 8000)
            {
                if(!playJingle)
                {
                    s_jingle.setBuffer(sb_win_jingle);
                    s_jingle.setVolume(float(missionConfig->GetInt("masterVolume"))*(float(missionConfig->GetInt("sfxVolume"))/100.f));
                    s_jingle.play();
                    playJingle = true;
                }
            }
        }
        else
        {
            if(!playJingle)
            {
                s_jingle.setBuffer(sb_lose_jingle);
                s_jingle.setVolume(float(missionConfig->GetInt("masterVolume"))*(float(missionConfig->GetInt("sfxVolume"))/100.f));
                s_jingle.play();
                playJingle = true;
            }
        }
    }

    /** Make the camera follow Patapons until the jingle is played **/

    if(missionEndTimer.getElapsedTime().asMilliseconds() < 7700)
    {
        camera.followobject_x = army_X * (window.getSize().x / float(1280));
    }

    /** Mission fade in and fade out **/

    if(!missionEnd)
    {
        if(fade_alpha > 0)
        {
            fade_alpha -= float(500) / fps;
        }

        if(fade_alpha <= 0)
        {
            fade_alpha = 0;
        }
    }
    else
    {
        if(!failure)
        {
            if(missionEndTimer.getElapsedTime().asMilliseconds() >= 11000)
            {
                if(fade_alpha < 255)
                {
                    fade_alpha += float(250) / fps;
                }

                if(fade_alpha >= 255)
                {
                    fade_alpha = 255;
                }
            }
        }
        else
        {
            if(missionEndTimer.getElapsedTime().asMilliseconds() >= 1000)
            {
                if(fade_alpha < 255)
                {
                    fade_alpha += float(250) / fps;
                }

                if(fade_alpha >= 255)
                {
                    fade_alpha = 255;
                }
            }
        }
    }

    fade_box.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    fade_box.setFillColor(sf::Color(0,0,0,fade_alpha));
    window.draw(fade_box);

    /** Mission end event (Mission complete/Mission failed screen + transition to Patapolis **/

    if(!failure) ///Victory
    {
        if(missionEndTimer.getElapsedTime().asMilliseconds() >= 11500)
        {
            if(!textBounce)
            {
                if(missionEndTimer.getElapsedTime().asMilliseconds() >= 13050)
                {
                    textCurScale = 1.4;
                    textBounce = true;
                }
            }

            t_win.setOrigin(t_win.getLocalBounds().width/2, t_win.getLocalBounds().height/2);
            t_win_outline.setOrigin(t_win_outline.getLocalBounds().width/2, t_win_outline.getLocalBounds().height/2);

            if(barCurX > barDestX)
            {
                barCurX -= (abs(barCurX - barDestX) * 5) / fps;
            }
            else
            {
                barCurX = barDestX;
            }

            if(textCurX < textDestX)
            {
                textCurX += (abs(textCurX - textDestX) * 5) / fps;
            }
            else
            {
                textCurX = textDestX;
            }
            if(textCurScale > textDestScale)
            {
                textCurScale -= (abs(textCurScale - textDestScale) * 5) / fps;
            }
            else
            {
                textCurScale = textDestScale;
            }

            t_win.setScale(textCurScale);
            t_win_outline.setScale(textCurScale);

            bar_win.setPosition(barCurX, 360);
            t_win.setPosition(textCurX-7, 360-14);
            t_win_outline.setPosition(textCurX+2, 360-4);

            bar_win.draw(window);
            t_win_outline.draw(window);
            t_win.draw(window);

            if(missionEndTimer.getElapsedTime().asMilliseconds() >= 17000)
            {
                if(fadeout_alpha < 255)
                {
                    fadeout_alpha += float(250) / fps;
                }

                if(fadeout_alpha >= 255)
                {
                    fadeout_alpha = 255;
                }

                fadeout_box.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
                fadeout_box.setFillColor(sf::Color(0,0,0,fadeout_alpha));
                window.draw(fadeout_box);
            }

            if(missionEndTimer.getElapsedTime().asMilliseconds() > 19000)
            {
                cout << "End mission" << endl;
                /// A wall is unyielding, so it does nothing when collided with.

                /// note we don't call the parent function. It does nothing, it just serves
                /// as an incomplete function to be overridden by child classes.
                /// end the mission

                StopMission();

                cout << "Add the picked up items to item repository" << endl;
                submitPickedItems();
                updateMissions();

                cout << "Go to Patapolis" << endl;

                sf::Thread loadingThreadInstance(v4core->LoadingThread,v4core);
                v4core->continueLoading=true;
                v4core->window.setActive(false);
                loadingThreadInstance.launch();

                v4core->mainMenu.patapolisMenu.doWaitKeyPress = false;
                v4core->mainMenu.patapolisMenu.Show();
                v4core->mainMenu.patapolisMenu.isActive = true;

                if (!v4core->mainMenu.patapolisMenu.initialised)
                {
                    /// patapolis might not be initialised because we could be running the pre-patapolis scripted first mission.
                    cout << "[ENDFLAG] Initialize Patapolis for the first time" << endl;
                    v4core->mainMenu.patapolisMenu.Initialise(missionConfig,v4core,&v4core->mainMenu);
                }
                else
                {
                    cout << "Don't initialize Patapolis, just show it again" << endl;
                }

                v4core->mainMenu.patapolisMenu.location = 3;
                v4core->mainMenu.patapolisMenu.SetTitle(3);
                v4core->mainMenu.patapolisMenu.camPos = v4core->mainMenu.patapolisMenu.locations[3];
                v4core->mainMenu.patapolisMenu.fade_alpha = 255;

                while(missionEndTimer.getElapsedTime().asMilliseconds() < 21000)
                {
                    ///halt loading for a second
                }

                v4core->LoadingWaitForKeyPress();

                v4core->continueLoading=false;

                v4core->ChangeRichPresence("In Patapolis", "logo", "");
            }
        }
    }
    else ///Failure
    {
        if(missionEndTimer.getElapsedTime().asMilliseconds() >= 2500)
        {
            t_lose.setOrigin(t_lose.getLocalBounds().width/2, t_lose.getLocalBounds().height/2);
            t_lose_outline.setOrigin(t_lose_outline.getLocalBounds().width/2, t_lose_outline.getLocalBounds().height/2);

            if(barCurX > barDestX)
            {
                barCurX -= (abs(barCurX - barDestX) * 5) / fps;
            }
            else
            {
                barCurX = barDestX;
            }

            if(textCurX < textDestX)
            {
                textCurX += (abs(textCurX - textDestX) * 5) / fps;
            }
            else
            {
                textCurX = textDestX;
            }

            t_lose.setScale(textCurScale);
            t_lose_outline.setScale(textCurScale);

            bar_lose.setPosition(barCurX, 360);
            t_lose.setPosition(textCurX-7, 360-14);
            t_lose_outline.setPosition(textCurX+2, 360-4);

            bar_lose.draw(window);
            t_lose_outline.draw(window);
            t_lose.draw(window);

            if(missionEndTimer.getElapsedTime().asMilliseconds() >= 6000)
            {
                if(fadeout_alpha < 255)
                {
                    fadeout_alpha += float(250) / fps;
                }

                if(fadeout_alpha >= 255)
                {
                    fadeout_alpha = 255;
                }

                fadeout_box.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
                fadeout_box.setFillColor(sf::Color(0,0,0,fadeout_alpha));
                window.draw(fadeout_box);
            }

            if(missionEndTimer.getElapsedTime().asMilliseconds() >= 8000)
            {
                /** End flag executes the mission victory code, so mission failed code needs to be executed separately here. **/

                cout << "End mission" << endl;

                StopMission();

                cout << "Go to Patapolis" << endl;

                sf::Thread loadingThreadInstance(v4core->LoadingThread,v4core);
                v4core->continueLoading=true;
                v4core->window.setActive(false);
                loadingThreadInstance.launch();

                v4core->mainMenu.patapolisMenu.doWaitKeyPress = false;
                v4core->mainMenu.patapolisMenu.Show();
                v4core->mainMenu.patapolisMenu.isActive = true;

                if (!v4core->mainMenu.patapolisMenu.initialised)
                {
                    /// patapolis might not be initialised because we could be running the pre-patapolis scripted first mission.
                    cout << "[ENDFLAG] Initialize Patapolis for the first time" << endl;
                    v4core->mainMenu.patapolisMenu.Initialise(missionConfig,v4core,&v4core->mainMenu);
                }
                else
                {
                    cout << "Don't initialize Patapolis, just show it again" << endl;
                }

                v4core->mainMenu.patapolisMenu.location = 3;
                v4core->mainMenu.patapolisMenu.SetTitle(3);
                v4core->mainMenu.patapolisMenu.camPos = v4core->mainMenu.patapolisMenu.locations[3];
                v4core->mainMenu.patapolisMenu.fade_alpha = 255;

                while(missionEndTimer.getElapsedTime().asMilliseconds() < 10000)
                {
                    ///halt loading for a second
                }

                v4core->LoadingWaitForKeyPress();

                v4core->continueLoading=false;

                v4core->ChangeRichPresence("In Patapolis", "logo", "");
            }
        }
    }
}

void MissionController::DoVectorCleanup(vector<int> units_rm, vector<int> dmg_rm, vector<int> tlo_rm, vector<int> pr_rm)
{
    //cout << "MissionController::DoVectorCleanup" << endl;
    //cout << units_rm.size() << " " << dmg_rm.size() << " " << tlo_rm.size() << " " << pr_rm.size() << endl;

    for(int i=0; i<units_rm.size(); i++)
    {
        units.erase(units.begin()+(units_rm[i]-i));
        cout << "Erased unit " << units_rm[i] << endl;
    }

    for(int i=0; i<dmg_rm.size(); i++)
    {
        dmgCounters.erase(dmgCounters.begin()+(dmg_rm[i] - i));
        cout << "Erased dmgCounter " << dmg_rm[i] << endl;
    }

    for(int i=0; i<tlo_rm.size(); i++)
    {
        tangibleLevelObjects.erase(tangibleLevelObjects.begin()+(tlo_rm[i]-i));
        cout << "Erased tangibleLevelObject " << tlo_rm[i] << endl;
    }

    for(int i=0; i<pr_rm.size(); i++)
    {
        levelProjectiles.erase(levelProjectiles.begin()+(pr_rm[i]-i));
        cout << "Erased levelProjectile " << pr_rm[i] << endl;
    }
}

std::vector<int> MissionController::DrawProjectiles(sf::RenderWindow& window)
{
    /** Projectile management **/

    vector<int> pr_e; ///projectiles to erase

    /// step 1: all projectiles have gravity applied to them
    for(int i=0; i<levelProjectiles.size(); i++)
    {
        Projectile* p = levelProjectiles[i].get();
        float xspeed = p->GetXSpeed();
        float yspeed = p->GetYSpeed();
        yspeed += (gravity/fps);
        p->SetNewSpeedVector(xspeed,yspeed);
        p->Update(window,fps);
    }

    /// step 3: any projectiles that hit any collidableobject are informed
    for(int i=0; i<levelProjectiles.size(); i++)
    {
        bool removeProjectile = false;

        Projectile* p = levelProjectiles[i].get();
        float ypos = p->yPos;
        float xpos = p->xPos;
        HitboxFrame tmp;
        tmp.time = 0;
        tmp.g_x = 0;
        tmp.g_y = 0;
        tmp.clearVertices();
        tmp.addVertex(-3,-1); /// "top left"
        tmp.addVertex(3,-1); /// "top right"
        tmp.addVertex(-3,1); /// "bottom left"
        tmp.addVertex(3,1); /// "bottom right"
        tmp.rotation = -p->angle;

        if(ypos>floorY)
        {
            removeProjectile = true;
        }

        ///calculate projectile damage
        ///and pass it to a special vector called collisionData
        ///which passes whatever you'd like to the collided animation object
        ///so you can put anything and react with it in the individual entity classes
        ///in projectiles' case, im transferring the damage dealt

        int minDmg = p->mindmg;
        int maxDmg = p->maxdmg;
        int bound = maxDmg-minDmg+1;
        int ranDmg = rand() % bound;
        int total = minDmg + ranDmg;

        ///sending damage dealt
        vector<string> collisionData = {to_string(total)};

        ///retrieve collision event
        vector<CollisionEvent> cevent;

        ///check whether the projectile was on enemy side or ally side
        if(!p->enemy)
        {
            ///Do collisions for all entities (ally projectiles)
            cevent = DoCollisionForObject(&tmp,xpos,ypos,p->projectileID,collisionData);
        }
        else
        {
            ///Do collisions for all units (enemy projectiles)
            cevent = DoCollisionForUnit(&tmp,xpos,ypos,p->projectileID,collisionData);
        }

        for(int e=0; e<cevent.size(); e++)
        {
            if(cevent[e].collided)
            {
                if(cevent[e].isCollidable)
                {
                    removeProjectile = true;
                }

                ///add damage counter
                if(cevent[e].isAttackable)
                addDmgCounter(0, total, xpos, ypos, qualitySetting, resSetting);
            }
        }

        p->Draw(window,fps);

        if(removeProjectile)
        pr_e.push_back(i);
    }

    return pr_e;
}

void MissionController::DrawUnitThumbs(sf::RenderWindow& window)
{
    for(int i=0; i<unitThumbs.size(); i++)
    {
        float resRatioX = window.getSize().x / float(1280);
        float resRatioY = window.getSize().y / float(720);

        int farthest_id = -1;
        float temp_pos = -9999;

        int curunits = 0;
        float maxunithp = 0;
        float curunithp = 0;

        for(int u=0; u<units.size(); u++)
        {
            if(units[u].get()->getUnitID() == unitThumbs[i].unit_id)
            {
                curunits++;

                PlayableUnit* unit = units[u].get();

                if(temp_pos <= unit->getGlobalPosition().x)
                {
                    temp_pos = unit->getGlobalPosition().x;
                    farthest_id = u;
                }

                maxunithp += unit->getUnitMaxHP();
                curunithp += unit->getUnitHP();
            }
        }

        if(farthest_id != -1)
        {
            float hp_percentage = curunithp / maxunithp;

            unitThumbs[i].circle.setRadius(28*resRatioX);
            unitThumbs[i].circle.setOrigin(unitThumbs[i].circle.getLocalBounds().width/2,unitThumbs[i].circle.getLocalBounds().height/2);
            unitThumbs[i].circle.setPosition((52+(64*i))*resRatioX, (72*resRatioY));
            window.draw(unitThumbs[i].circle);

            PlayableUnit* farthest_unit = units[farthest_id].get();
            unitThumbs[i].thumb = farthest_unit->objects[0].s_obj;
            unitThumbs[i].thumb.setScale(0.7,0.7);

            int manual_x,manual_y;

            if(unitThumbs[i].unit_id == 0)
            {
                manual_x = 6;
                manual_y = -40;
            }

            if(unitThumbs[i].unit_id == 1)
            {
                manual_x = 0;
                manual_y = 14;
            }

            unitThumbs[i].thumb.setPosition(52+(64*i)+manual_x, 60+manual_y);
            unitThumbs[i].thumb.draw(window);

            unitThumbs[i].hpbar_back.setOrigin(unitThumbs[i].hpbar_back.getLocalBounds().width/2, unitThumbs[i].hpbar_back.getLocalBounds().height/2);
            unitThumbs[i].hpbar_back.setPosition(52+(64*i), 32);
            unitThumbs[i].hpbar_back.draw(window);

            unitThumbs[i].hpbar_ins.setOrigin(0, unitThumbs[i].hpbar_ins.getLocalBounds().height/2);
            unitThumbs[i].hpbar_ins.setTextureRect(sf::IntRect(0,0,unitThumbs[i].width*hp_percentage,unitThumbs[i].hpbar_ins.getLocalBounds().height));
            unitThumbs[i].hpbar_ins.setPosition(52+(64*i)-27, 32);

            if(hp_percentage > 0.70)
            unitThumbs[i].hpbar_ins.setColor(sf::Color(0,255,0,255));
            else if(hp_percentage > 0.35)
            unitThumbs[i].hpbar_ins.setColor(sf::Color(245,230,66,255));
            else
            unitThumbs[i].hpbar_ins.setColor(sf::Color(212,0,0,255));

            unitThumbs[i].hpbar_ins.draw(window);

            if(unitThumbs[i].unit_id != 0)
            {
                unitThumbs[i].unit_count_shadow.setString(to_string(curunits));
                unitThumbs[i].unit_count_shadow.setOrigin(unitThumbs[i].unit_count_shadow.getLocalBounds().width/2, unitThumbs[i].unit_count_shadow.getLocalBounds().height/2);
                unitThumbs[i].unit_count_shadow.setPosition(52+(64*i)+28, 98);
                unitThumbs[i].unit_count_shadow.draw(window);

                unitThumbs[i].unit_count.setString(to_string(curunits));
                unitThumbs[i].unit_count.setOrigin(unitThumbs[i].unit_count.getLocalBounds().width/2, unitThumbs[i].unit_count.getLocalBounds().height/2);
                unitThumbs[i].unit_count.setPosition(52+(64*i)+26, 96);
                unitThumbs[i].unit_count.draw(window);
            }
        }
    }
}

void MissionController::DrawPickedItems(sf::RenderWindow& window)
{
    for(int i=0; i<pickedItems.size(); i++)
    {
        float resRatioX = window.getSize().x / float(1280);
        float resRatioY = window.getSize().y / float(720);

        pickedItems[i].circle.setRadius(25*resRatioX);
        pickedItems[i].circle.setOrigin(pickedItems[i].circle.getLocalBounds().width/2,pickedItems[i].circle.getLocalBounds().height/2);
        pickedItems[i].circle.setPosition((1230 - 54*i)*resRatioX, (50*resRatioY));
        window.draw(pickedItems[i].circle);

        pickedItems[i].item.setOrigin(pickedItems[i].bounds.x/2, pickedItems[i].bounds.y/2);
        pickedItems[i].item.setPosition(1230 - 54*i, 50);
        pickedItems[i].item.setScale(0.8,0.8);
        pickedItems[i].item.draw(window);
    }
}

void MissionController::DrawHitboxes(sf::RenderWindow& window)
{
    for(int i=0; i<units.size(); i++)
        {
            PlayableUnit* unit = units[i].get();

            for(int h=0; h<unit->hitboxes.size(); h++)
            {
                HitboxFrame* currentHitbox = &(unit->hitboxes[h].hitboxObject);

                sf::ConvexShape convex;
                convex.setFillColor(sf::Color(150, 50, 250));
                // resize it to 5 points
                std::vector<sf::Vector2f> currentVertices = currentHitbox->getCurrentVertices();
                convex.setPointCount(currentVertices.size());

                for (int j=0; j<currentVertices.size(); j++)
                {
                    sf::Vector2f currentPoint = currentVertices[j];
                    currentPoint.x = currentPoint.x + currentHitbox->g_x + unit->global_x + unit->local_x;
                    currentPoint.y = currentPoint.y + currentHitbox->g_y + unit->global_y + unit->local_y;
                    //cout<<"DRAWING POINT: "<<currentVertices.size()<<" x: "<<currentPoint.x<<" y: "<<currentPoint.y<<endl;
                    sf::CircleShape shape(5);
                    shape.setFillColor(sf::Color(100, 250, 50));
                    shape.setPosition(currentPoint.x-2.5,currentPoint.y-2.5);
                    window.draw(shape);
                    convex.setPoint(j, currentPoint);
                    //cout << "convex.setPoint(" << j << ", " << currentPoint.x << " " << currentPoint.y << ");" << endl;
                }

                window.draw(convex);
            }
        }

        for(int i=0; i<tangibleLevelObjects.size(); i++)
        {
            Entity* entity = tangibleLevelObjects[i].get();

            for(int h=0; h<entity->hitboxes.size(); h++)
            {
                HitboxFrame* currentHitbox = &(entity->hitboxes[h].hitboxObject);

                sf::ConvexShape convex;
                convex.setFillColor(sf::Color(150, 50, 250));
                // resize it to 5 points
                std::vector<sf::Vector2f> currentVertices = currentHitbox->getCurrentVertices();
                convex.setPointCount(currentVertices.size());

                for (int j=0; j<currentVertices.size(); j++)
                {

                    sf::Vector2f currentPoint = currentVertices[j];
                    currentPoint.x = currentPoint.x + currentHitbox->g_x + entity->global_x + entity->local_x;
                    currentPoint.y = currentPoint.y + currentHitbox->g_y + entity->global_y + entity->local_y;
                    //cout<<"DRAWING POINT: "<<currentVertices.size()<<" x: "<<currentPoint.x<<" y: "<<currentPoint.y<<endl;
                    sf::CircleShape shape(5);
                    shape.setFillColor(sf::Color(100, 250, 50));
                    shape.setPosition(currentPoint.x-2.5,currentPoint.y-2.5);
                    window.draw(shape);
                    convex.setPoint(j, currentPoint);
                    //cout << "convex.setPoint(" << j << ", " << currentPoint.x << " " << currentPoint.y << ");" << endl;

                }

                window.draw(convex);
            }
        }
}

std::vector<int> MissionController::DrawDamageCounters(sf::RenderWindow& window)
{
    vector<int> dmg_rm;

    for(int i=0; i<dmgCounters.size(); i++)
    {
        float a=0;

        for(int d=0; d<dmgCounters[i].spr.size(); d++)
        {
            if(dmgCounters[i].display_timer.getElapsedTime().asMilliseconds() > 70*d)
            {
                float curScale = dmgCounters[i].scale[d];
                float destScale = dmgCounters[i].scale_goal[d];

                if(dmgCounters[i].mode[d])
                {
                    curScale -= float(14) / fps;
                    dmgCounters[i].alpha[d] += float(1800) / fps;

                    if(curScale <= destScale)
                    {
                        dmgCounters[i].mode[d] = false;
                        destScale = 1;
                    }

                    if(dmgCounters[i].alpha[d] >= 255)
                    dmgCounters[i].alpha[d] = 255;
                }

                if(!dmgCounters[i].mode[d])
                {
                    if(!dmgCounters[i].mode[d])
                    {
                        curScale += float(8) / fps;

                        if(curScale >= destScale)
                        {
                            curScale = destScale;
                        }
                    }
                }

                if(dmgCounters[i].display_timer.getElapsedTime().asMilliseconds() > 70*d + 1000)
                {
                    if(!dmgCounters[i].mode[d])
                    {
                        dmgCounters[i].pos[d].y += float(60) / fps;
                        dmgCounters[i].alpha[d] -= float(500) / fps;

                        if(dmgCounters[i].alpha[d] <= 0)
                        dmgCounters[i].alpha[d] = 0;
                    }
                }

                dmgCounters[i].scale[d] = curScale;
                dmgCounters[i].scale_goal[d] = destScale;

                dmgCounters[i].spr[d].setPosition(dmgCounters[i].pos[d].x, dmgCounters[i].pos[d].y-((curScale-1)*10));
                dmgCounters[i].spr[d].setScale(curScale, curScale);
                dmgCounters[i].spr[d].setColor(sf::Color(255,255,255,dmgCounters[i].alpha[d]));

                dmgCounters[i].spr[d].draw(window);

                a += dmgCounters[i].alpha[d];
            }
        }

        if(a <= 1)
        dmg_rm.push_back(i);
    }

    return dmg_rm;
}

std::vector<int> MissionController::DrawEntities(sf::RenderWindow& window)
{
    vector<int> tlo_rm;

    /** Find the farthest unit in your army (for calculations) **/
    int farthest_id = -1;
    float temp_pos = -9999;

    for(int i=0; i<units.size(); i++)
    {
        PlayableUnit* unit = units[i].get();

        if(temp_pos <= unit->getGlobalPosition().x)
        {
            temp_pos = unit->getGlobalPosition().x;
            farthest_id = i;
        }
    }

    for (int i=0; i<tangibleLevelObjects.size(); i++)
    {
        Entity* entity = tangibleLevelObjects[i].get();

        entity->fps = fps;

        if(entity->getEntityID() == 1)
        {
            entity->doRhythm(rhythm.current_song, rhythm.rhythmController.current_drum, rhythm.GetCombo(), rhythm.GetRealCombo(), rhythm.advanced_prefever, rhythm.r_gui.beatBounce, rhythm.GetSatisfaction());

            if(!missionEnd)
            entity->Draw(window);
        }
        else
        {
            ///Check if entity is off bounds, if yes, don't render it.
            entity->offbounds = false;

            if(entity->getGlobalPosition().x > (camera.followobject_x)/(window.getSize().x / float(1280))+2400)
            entity->offbounds = true;

            if(entity->getGlobalPosition().x < (camera.followobject_x)/(window.getSize().x / float(1280))-1000)
            entity->offbounds = true;

            entity->distance_to_unit = abs(temp_pos - entity->getGlobalPosition().x);

            ///Check for entity attack measures
            if(entity->doAttack())
            {
                ///ID 6,16 = Kirajin Yari
                if((entity->getEntityID() == 6) || (entity->getEntityID() == 16))
                {
                    cout << "Entity " << i << " threw a spear!" << endl;

                    float rand_hs = (rand() % 1000) / float(10);
                    float rand_vs = (rand() % 1000) / float(10);

                    float rand_rad = (rand() % 200000000) / float(1000000000);

                    float mindmg = 1;
                    float maxdmg = 10;

                    float xpos = entity->getGlobalPosition().x+entity->hitBox.left+entity->hitBox.width/2;
                    float ypos = entity->getGlobalPosition().y+entity->hitBox.top+entity->hitBox.height/2;

                    spawnProjectile(xpos, ypos, 800, -450-rand_hs, -450+rand_vs, -2.58 + rand_rad, maxdmg, mindmg, 0, true);
                }
            }

            ///Check if entity's parent is still alive, if not, kill the entity
            int parent = entity->parent;

            ///Check if parent is defined
            if(parent != -1)
            {
                ///Look for parent
                auto it = find_if(tangibleLevelObjects.begin(), tangibleLevelObjects.end(), [&parent](const std::unique_ptr<Entity>& obj) {return obj.get()->spawnOrderID == parent;});

                if(it != tangibleLevelObjects.end())
                {
                    ///Parent has been found!

                    auto index = std::distance(tangibleLevelObjects.begin(), it);
                    //cout << "My parent is currently residing at " << index << endl;

                    ///Check if it's dead
                    if(tangibleLevelObjects[index].get()->dead)
                    {
                        ///Kill the entity
                        entity->die();
                    }
                }
                else ///Parent can't be found
                {
                    ///Kill the entity
                    entity->die();
                }
            }

            entity->Draw(window);
        }

        if(entity->ready_to_erase)
        tlo_rm.push_back(i);
    }

    return tlo_rm;
}

std::vector<int> MissionController::DrawUnits(sf::RenderWindow& window)
{
    vector<int> units_rm;

    int farthest_id = -1;
    int closest_entity_id = -1;
    int closest_entity_pos = 9999;

    bool hatapon = false;

    auto max_distance = std::max_element( units.begin(), units.end(),
                             []( unique_ptr<PlayableUnit> &a, unique_ptr<PlayableUnit> &b )
                             {
                                 return a->global_x < b->global_x;
                             } );

    farthest_id = distance(units.begin(), max_distance);

    /** Units draw loop and entity range detection **/

    for(int i=0; i<tangibleLevelObjects.size(); i++)
    {
        if(tangibleLevelObjects[i].get()->entityType == Entity::EntityTypes::HOSTILE)
        {
            if(tangibleLevelObjects[i].get()->getGlobalPosition().x < closest_entity_pos)
            {
                closest_entity_pos = tangibleLevelObjects[i].get()->getGlobalPosition().x;
                closest_entity_id = i;
            }
        }
    }

    if(farthest_id != -1)
    {
        bool inRange = true;

        if(closest_entity_id == -1)
        {
            inRange = false;
        }
        else
        {
            Entity* closest_entity = tangibleLevelObjects[closest_entity_id].get();

            //cout << "Check if entity is in range" << endl;

            for(int i=0; i<units.size(); i++)
            {
                if(units[i].get()->getUnitID() != 0)
                {
                    if(closest_entity->entityType == Entity::EntityTypes::HOSTILE)
                    {
                        //cout << "Range of unit " << i << ": " << abs((units[i].get()->getGlobalPosition().x) - closest_entity->getGlobalPosition().x) - 110 << endl;
                        //cout << "Dest local x: " << units[i].get()->dest_local_x << endl;

                        if(abs((units[i].get()->getGlobalPosition().x) - closest_entity->getGlobalPosition().x) - 110 > 800)
                        inRange = false;
                    }
                }
            }

            //cout << "In range: " << inRange << endl;
        }

        /** Draw the units **/
        for(int i=0; i<units.size(); i++)
        {
            PlayableUnit* unit = units[i].get();

            if(closest_entity_id != -1)
            {
                Entity* closest_entity = tangibleLevelObjects[closest_entity_id].get();

                unit->entity_distance = abs((unit->getGlobalPosition().x) - closest_entity->getGlobalPosition().x - closest_entity->hitboxes[0].getGlobalPosition().x) - 110;

                if((closest_entity->entityType == Entity::EntityTypes::HOSTILE) && (inRange))
                {
                    unit->enemy_in_range = true;
                    //cout << "Unit " << i << " distance to closest enemy is " << unit->entity_distance << " pixels" << endl;
                }
                else
                {
                    unit->enemy_in_range = false;
                    unit->dest_local_x = 0;
                }
            }
            else
            {
                unit->enemy_in_range = false;
            }

            if(unit->getUnitID() != 0)
            {
                float unit_distance = 9999;

                for(int a=0; a<units.size(); a++)
                {
                    if(a != i)
                    {
                        if(units[a].get()->getUnitID() == 1)
                        {
                            float gx = units[a].get()->getGlobalPosition().x;
                            float my_gx = unit->getGlobalPosition().x;

                            float dis = abs(gx-my_gx);

                            if(dis < unit_distance)
                            unit_distance = dis;
                        }
                    }
                }

                unit->unit_distance = unit_distance;

                //cout << "Unit " << i << " distance to another unit is " << unit_distance << " pixels" << endl;
            }

            /** Verify if Hatapon exists **/
            if(unit->getUnitID() == 0)
            {
                if(unit->getUnitHP() > 0)
                hatapon = true;
            }

            /** Execute unit features when mission is not over **/
            if(!missionEnd)
            {
                unit->doRhythm(rhythm.current_song, rhythm.rhythmController.current_drum, rhythm.GetCombo());

                if(unit->getUnitID() == 1) /// Yaripon
                {
                    if(unit->doAttack())
                    {
                        cout << "Unit " << i << " threw a spear!" << endl;

                        float rand_hs = (rand() % 1000) / float(10);
                        float rand_vs = (rand() % 1000) / float(10);

                        float rand_rad = (rand() % 200000000) / float(1000000000);

                        int rhythm_acc = rhythm.current_perfect; ///Check how many perfect measures has been done to improve the spears throwing
                        float fever_boost = 0.8;

                        if(rhythm.GetCombo() >= 11) ///Check for fever to boost the spears damage
                        fever_boost = 1.0;

                        float mindmg = unit->mindmg * (0.8 + (rhythm_acc*0.05)) * fever_boost;
                        float maxdmg = unit->maxdmg * (0.8 + (rhythm_acc*0.05)) * fever_boost;

                        ///Make the spears be thrown with worse velocity when player is drumming bad (10% punishment)
                        rand_hs *= 0.9 + (rhythm_acc*0.025);
                        rand_vs *= 0.9 + (rhythm_acc*0.025);

                        ///This way, the lowest damage is dmg * 0.64 (36% punishment) and highest damage is 100% of the values

                        float xpos = unit->getGlobalPosition().x+unit->hitBox.left+unit->hitBox.width/2;
                        float ypos = unit->getGlobalPosition().y+unit->hitBox.top+unit->hitBox.height/2;

                        spawnProjectile(xpos, ypos, 800, 450+rand_hs, -450+rand_vs, -0.58 - rand_rad, maxdmg, mindmg, 0);
                    }
                }
            }

            if(missionEnd)
            {
                if(!failure)
                unit->doMissionEnd();
            }

            unit->fps = fps;
            unit->Draw(window);

            if(unit->ready_to_erase)
            units_rm.push_back(i);
        }
    }

    /** Fail the mission if Hatapon is dead or when Hatapon is the only unit remaining **/

    if((!hatapon) || ((hatapon) && (units.size() <= 1)))
    {
        missionEnd = true;
        failure = true;
        rhythm.Stop();
    }

    return units_rm;
}

void MissionController::Update(sf::RenderWindow &window, float cfps, InputController& inputCtrl)
{
    ///Sort tangibleLevelObjects to prioritize rendering layers
    std::sort(tangibleLevelObjects.begin(), tangibleLevelObjects.end(),
              [](const std::unique_ptr<Entity>& a, const std::unique_ptr<Entity>& b)
                {
                    return a.get()->layer < b.get()->layer;
                });

    ///Globally disable the controls when Dialogbox is opened, but preserve original controller for controlling the DialogBoxes later
    InputController o_inputCtrl;
    InputController cur_inputCtrl;

    cur_inputCtrl = inputCtrl;

    if(dialogboxes.size() > 0)
    {
        o_inputCtrl = inputCtrl;

        InputController a;
        cur_inputCtrl = a;
    }

    /** Update loop, everything here happens per each frame of the game **/
    fps = cfps;

    /** Apply the keyMap from parent class **/

    /** Execute camera and background **/

    camera.Work(window,fps,cur_inputCtrl);
    test_bg.setCamera(camera);
    test_bg.Draw(window);

    /** Execute Keyboard events and Movement **/

    DoKeyboardEvents(window,fps,cur_inputCtrl);
    DoMovement(window,fps,cur_inputCtrl);

    /** Draw all Entities **/

    vector<int> tlo_rm = DrawEntities(window);

    /** Draw all Units **/

    vector<int> units_rm = DrawUnits(window);

    /** Draw projectiles **/

    vector<int> pr_rm = DrawProjectiles(window);

    /** Draw message clouds **/
    for(int e=0; e<tangibleLevelObjects.size(); e++)
    {
        Entity* entity = tangibleLevelObjects[e].get();
        entity->doMessages(window, fps, inputCtrl);
    }

    /** Draw hitboxes **/

    if(showHitboxes)
    {
        DrawHitboxes(window);
    }

    /** Draw damage counters **/
    vector<int> dmg_rm = DrawDamageCounters(window);

    /**  Draw static UI elements **/

    auto lastView = window.getView();
    window.setView(window.getDefaultView());

    /**

    if(cutscenesLeft && !inCutscene && isMoreCutscenes())
    {
        StartCutscene(cutscene_text_identifiers[currentCutsceneId],cutscene_blackscreens[currentCutsceneId],cutscene_lengths[currentCutsceneId]);
    }

    sf::Time currentTime = timer.getElapsedTime();
    int currentAlpha = startAlpha;
    if (currentTime >= targetTime && inCutscene)
    {
        // oops: currentAlpha = endAlpha; // make certain that the alpha is at its final destination
        //you are done
        if(!isMoreCutscenes())
        {
            currentAlpha = startAlpha;
            inCutscene = false;
            if(isBlackScreenCutscene)
            {
                inFadeTransition = true;
                timer.restart();
                targetTime = sf::seconds(2);
            }
            else
            {
                FinishLastCutscene();
            }
            cutscenesLeft=false;
        }
        else
        {
            /// next cutscene
            currentCutsceneId++;
            StartCutscene(cutscene_text_identifiers[currentCutsceneId],cutscene_blackscreens[currentCutsceneId],cutscene_lengths[currentCutsceneId]);
        }
    }
    else if (currentTime >= targetTime && !inCutscene && inFadeTransition)
    {
        currentAlpha = endAlpha;
        inFadeTransition = false;
        FinishLastCutscene();
    }
    else if (!inCutscene && inFadeTransition)
    {
        currentAlpha = startAlpha + (endAlpha - startAlpha) * (currentTime.asMilliseconds() / (targetTime.asMilliseconds()+0.0));
    }
    else if (inCutscene && isBlackScreenCutscene)
    {
        currentAlpha = startAlpha;
    }
    else if (inCutscene)
    {
        currentAlpha = startAlpha;
    }
    // apply alpha to whatever colour is previously set
    if((inFadeTransition || inCutscene) && isBlackScreenCutscene)
    {
        sf::Color fadeColor = fade.getFillColor();
        fadeColor.a = currentAlpha;
        fade.setFillColor(fadeColor);
        fade.setSize(sf::Vector2f(window.getSize().x,window.getSize().y));

        fade.setPosition(0,0);
        window.draw(fade);
    }
    if (inCutscene)
    {
        for (int i=0; i<t_cutscene_text.size(); i++)
        {
            sf::Text currentLine = t_cutscene_text[i];

            currentLine.setPosition(window.getSize().x/2,300 + 39*i);
            sf::Time currentTime = timer.getElapsedTime();

            window.draw(currentLine);
        }
    }*/

    /** Draw the timer (Static UI element) **/

    if(showTimer)
    {
        t_timerMenu.setString(Func::ConvertToUtf8String(std::to_string(missionTimer.getElapsedTime().asSeconds())+" Seconds"));
        t_timerMenu.setOrigin(t_timerMenu.getGlobalBounds().width/2,t_timerMenu.getGlobalBounds().height/2);
        t_timerMenu.setPosition(window.getSize().x/2,100);
        window.draw(t_timerMenu);
    }

    /** Draw floor **/

    float resRatioX = window.getSize().x / float(1280);
    float resRatioY = window.getSize().y / float(720);
    r_floor.setSize(sf::Vector2f(1280*resRatioX, 110*resRatioY));
    r_floor.setFillColor(sf::Color::Black);
    r_floor.setPosition(0,610*resRatioY);
    window.draw(r_floor);

    DrawUnitThumbs(window);
    DrawPickedItems(window);

    /** If mission isn't finished, execute and draw Rhythm **/

    if(!missionEnd)
    {
        ctrlTips.x = 0;
        ctrlTips.y = (720-ctrlTips.ySize);
        ctrlTips.draw(window);

        rhythm.fps = fps;
        DoRhythm(cur_inputCtrl);
        rhythm.Draw(window);
    }

    /** Execute all mission end related things **/
    DoMissionEnd(window, fps);

    if(dialogboxes.size() > 0)
    {
        if(o_inputCtrl.isKeyPressed(InputController::Keys::CROSS))
        {
            switch(dialogboxes[dialogboxes.size()-1].CheckSelectedOption())
            {
                case 0:
                {
                    if(dialogboxes[dialogboxes.size()-1].id == 0)
                    {
                        cout << "Return to Patapolis" << endl;
                        dialogboxes[dialogboxes.size()-1].Close();

                        missionEnd = true;
                        failure = true;
                        rhythm.Stop();
                    }
                    else if(dialogboxes[dialogboxes.size()-1].id == 999)
                    {
                        cout << "Enable hitboxes" << endl;
                        showHitboxes = true;
                        dialogboxes[dialogboxes.size()-1].Close();
                    }

                    break;
                }

                case 1:
                {
                    if(dialogboxes[dialogboxes.size()-1].id == 0)
                    {
                        cout << "Back to Mission" << endl;
                        dialogboxes[dialogboxes.size()-1].Close();
                    }
                    else if(dialogboxes[dialogboxes.size()-1].id == 999)
                    {
                        cout << "Disable hitboxes" << endl;
                        showHitboxes = false;
                        dialogboxes[dialogboxes.size()-1].Close();
                    }

                    break;
                }

                case 2:
                {
                    if(dialogboxes[dialogboxes.size()-1].id == 999)
                    {
                        cout << "Heal all units" << endl;
                        for(int u=0; u<units.size(); u++)
                        {
                            units[u].get()->current_hp = units[u].get()->max_hp;
                        }

                        dialogboxes[dialogboxes.size()-1].Close();
                    }

                    break;
                }
            }
        }
    }

    vector<int> db_e; ///dialog box erase

    for(int i=0; i<dialogboxes.size(); i++)
    {
        dialogboxes[i].x = 640;
        dialogboxes[i].y = 360;
        dialogboxes[i].Draw(window, fps, o_inputCtrl);

        if(dialogboxes[i].closed)
        db_e.push_back(i);
    }

    for(int i=0; i<db_e.size(); i++)
    {
        dialogboxes.erase(dialogboxes.begin()+db_e[i]-i);
    }

    window.setView(lastView);

    /** Remove vector objects that are no longer in use **/

    DoVectorCleanup(units_rm, dmg_rm, tlo_rm, pr_rm);
}
void MissionController::FinishLastCutscene()
{
    /// runs when the last cutscene of a sequence is done
}
bool MissionController::isMoreCutscenes()
{
    /// returns true if there are more cutscenes
    return currentCutsceneId<cutscene_text_identifiers.size()-1;
}
void MissionController::StartCutscene(const std::wstring& text,bool isBlackScreen, int TimeToShow)
{
    /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
    t_cutscene_text.clear();
    std::vector<std::wstring> wordsinDesc = Func::Split(missionConfig->strRepo.GetUnicodeString(text),' ');
    sf::String oldTotalString;
    sf::String currentTotalString;
    int maxWidth = missionConfig->GetInt("resX") * 0.4;
    timer.restart();
    inCutscene = true;
    isBlackScreenCutscene = isBlackScreen;
    targetTime = sf::seconds(TimeToShow);
    /// we split it into words, then go word by word testing the width of the string
    for (int i=0; i<wordsinDesc.size(); i++)
    {
        std::wstring currentWord = wordsinDesc[i];
        currentTotalString = currentTotalString + Func::ConvertToUtf8String(currentWord) + L" ";
        sf::Text t_newLine;
        t_newLine.setFont(f_font);
        t_newLine.setCharacterSize(35);
        t_newLine.setFillColor(sf::Color::White);
        t_newLine.setString(currentTotalString);
        if (t_newLine.getGlobalBounds().width>maxWidth)
        {
            /// when the string is too long, we go back to the last string and lock it in, then start a new line
            currentTotalString = oldTotalString;
            t_newLine.setString(currentTotalString);
            t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
            t_cutscene_text.push_back(t_newLine);
            oldTotalString = currentWord+L" ";
            currentTotalString = currentWord+L" ";
        }
        oldTotalString = currentTotalString;
        /// if there are no more words, finish up the current line
        if (i+1==wordsinDesc.size())
        {
            currentTotalString = oldTotalString;
            t_newLine.setString(currentTotalString);
            t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
            t_cutscene_text.push_back(t_newLine);
            oldTotalString = "";
            currentTotalString = "";
        }
    }
}
MissionController::~MissionController()
{
    //dtor

}
