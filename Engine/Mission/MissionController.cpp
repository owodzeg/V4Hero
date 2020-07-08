#include "MissionController.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include "Units/Projectile.h"
#include "../Math/PVector.h"
#include "Units/HitboxFrame.h"
#include "../V4Core.h"

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

void MissionController::spawnEntity(string entityName, int entityID, int baseX, int randX, int baseY, int spr_goal, int spr_range, int statLevel, sf::Color color, bool collidable, bool attackable)
{
    cout << "Spawning entity " << entityName << " (ID: " << entityID << ") " << baseX << " " << randX << " " << baseY << " " << spr_goal << " " << spr_range << " " << statLevel << endl;

    switch(entityID)
    {
        case 0:
        {
            unique_ptr<EndFlag> entity = make_unique<EndFlag>();
            entity.get()->LoadConfig(missionConfig);
            entity.get()->setEntityID(entityID);

            if(randX > 0)
            entity.get()->setGlobalPosition(sf::Vector2f(baseX + (rand() % randX),baseY));
            else
            entity.get()->setGlobalPosition(sf::Vector2f(baseX,baseY));

            entity.get()->setColor(color);

            entity.get()->isCollidable = collidable;
            entity.get()->isAttackable = attackable;

            if(spr_range != 0)
            {
                if(rand() % spr_range == spr_goal)
                {
                    tangibleLevelObjects.push_back(std::move(entity));
                }
            }
            else
            {
                tangibleLevelObjects.push_back(std::move(entity));
            }

            break;
        }
        case 1:
        {
            unique_ptr<FeverWorm> entity = make_unique<FeverWorm>();
            entity.get()->LoadConfig(missionConfig);
            entity.get()->setEntityID(entityID);

            if(randX > 0)
            entity.get()->setGlobalPosition(sf::Vector2f(baseX + (rand() % randX),baseY));
            else
            entity.get()->setGlobalPosition(sf::Vector2f(baseX,baseY));

            entity.get()->setColor(color);

            entity.get()->isCollidable = collidable;
            entity.get()->isAttackable = attackable;

            if(spr_range != 0)
            {
                if(rand() % spr_range == spr_goal)
                {
                    tangibleLevelObjects.push_back(std::move(entity));
                }
            }
            else
            {
                tangibleLevelObjects.push_back(std::move(entity));
            }

            break;
        }
        case 2:
        {
            unique_ptr<Kacheek> entity = make_unique<Kacheek>();
            entity.get()->LoadConfig(missionConfig);
            entity.get()->setEntityID(entityID);

            if(randX > 0)
            entity.get()->setGlobalPosition(sf::Vector2f(baseX + (rand() % randX),baseY));
            else
            entity.get()->setGlobalPosition(sf::Vector2f(baseX,baseY));

            entity.get()->setColor(color);

            entity.get()->isCollidable = collidable;
            entity.get()->isAttackable = attackable;

            if(spr_range != 0)
            {
                if(rand() % spr_range == spr_goal)
                {
                    tangibleLevelObjects.push_back(std::move(entity));
                }
            }
            else
            {
                tangibleLevelObjects.push_back(std::move(entity));
            }

            break;
        }
        case 3:
        {
            unique_ptr<Grass1> entity = make_unique<Grass1>();
            entity.get()->LoadConfig(missionConfig);
            entity.get()->setEntityID(entityID);

            if(randX > 0)
            entity.get()->setGlobalPosition(sf::Vector2f(baseX + (rand() % randX),baseY));
            else
            entity.get()->setGlobalPosition(sf::Vector2f(baseX,baseY));

            entity.get()->setColor(color);

            entity.get()->isCollidable = collidable;
            entity.get()->isAttackable = attackable;

            if(spr_range != 0)
            {
                if(rand() % spr_range == spr_goal)
                {
                    tangibleLevelObjects.push_back(std::move(entity));
                }
            }
            else
            {
                tangibleLevelObjects.push_back(std::move(entity));
            }

            break;
        }
        case 4:
        {
            unique_ptr<Grass2> entity = make_unique<Grass2>();
            entity.get()->LoadConfig(missionConfig);
            entity.get()->setEntityID(entityID);

            if(randX > 0)
            entity.get()->setGlobalPosition(sf::Vector2f(baseX + (rand() % randX),baseY));
            else
            entity.get()->setGlobalPosition(sf::Vector2f(baseX,baseY));

            entity.get()->setColor(color);

            entity.get()->isCollidable = collidable;
            entity.get()->isAttackable = attackable;

            if(spr_range != 0)
            {
                if(rand() % spr_range == spr_goal)
                {
                    tangibleLevelObjects.push_back(std::move(entity));
                }
            }
            else
            {
                tangibleLevelObjects.push_back(std::move(entity));
            }

            break;
        }
    }

    cout << "Loading finished" << endl;
}

void MissionController::Initialise(Config &config, std::map<int,bool> &keyMap,std::string backgroundString,V4Core &v4core_)
{
    v4core = &v4core_;
    sf::Context context;
    test_bg.Load(backgroundString, config);//config.GetString("debugBackground"));

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

    missionKeyMap = &keyMap;
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
    //sb_lose_jingle;

    sb_cheer1.loadFromFile("resources/sfx/level/cheer1.ogg");
    sb_cheer2.loadFromFile("resources/sfx/level/cheer2.ogg");
    sb_cheer3.loadFromFile("resources/sfx/level/cheer1.ogg");

    t_win.createText(f_moji, 56, sf::Color(222, 83, 0, 255), "MISSION COMPLETE!", q, 1);
    t_win_outline.createText(f_moji, 56, sf::Color(255, 171, 0, 255), "MISSION COMPLETE!", q, 1);
    t_win_outline.setOutlineColor(sf::Color(255, 171, 0, 255));
    t_win_outline.setOutlineThickness(10);
    t_lose.createText(f_moji, 56, sf::Color(138, 15, 26, 255), "MISSION FAILED!", q, 1);
    t_lose_outline.createText(f_moji, 56, sf::Color(254, 48, 55, 255), "MISSION FAILED!", q, 1);
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

    cout << "initialization finished" << endl;
}
void MissionController::StartMission(std::string missionFile, bool showCutscene)
{
    missionConfig->thisCore->SaveToDebugLog("Starting mission");

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

    pataponY = 720 - 174;
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
                            string sp = buff.substr(buff.find_first_of("=")+1);
                            vector<string> spawn = Func::Split(sp, ',');

                            int entityID = atoi(spawn[0].c_str());
                            int baseY = 0;
                            bool collidable = false;
                            bool attackable = false;

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
                                    }
                                }
                            }

                            entityParam.close();

                            cout << "Spawning an entity: " << entity_list[entityID] << endl;
                            spawnEntity(entity_list[entityID],entityID,atoi(spawn[1].c_str()),atoi(spawn[2].c_str()),baseY,atoi(spawn[3].c_str()),atoi(spawn[4].c_str()),atoi(spawn[5].c_str()),sf::Color(atoi(spawn[6].c_str()),atoi(spawn[7].c_str()),atoi(spawn[8].c_str()),atoi(spawn[9].c_str())), collidable, attackable);
                        }
                    }
                }
            }
        }
    }

    mf.close();

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

    units.push_back(std::move(p1));
    units.push_back(std::move(p2));
    units.push_back(std::move(p3));
    units.push_back(std::move(h));

    isFinishedLoading=true;
    v4core->LoadingWaitForKeyPress();

    string fm = "Playing mission: "+missionName;
    v4core->ChangeRichPresence(fm.c_str(), missionImg.c_str(), "logo");
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
void MissionController::DoKeyboardEvents(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap, std::map<int,bool> *keyMapHeld)
{
    /// do the keyboard things
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
    {
        missionEnd = true;
    }
}

MissionController::CollisionEvent MissionController::DoCollisionForObject(HitboxFrame* currentObjectHitBoxFrame,float currentObjectX,float currentObjectY,int collisionObjectID,vector<string> collisionData)
{
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
                target->OnCollide(target, collisionObjectID, collisionData);

                CollisionEvent cevent;
                cevent.collided = true;
                //cevent.collidedEntityID = -1;
                cevent.isAttackable = target->isAttackable;
                cevent.isCollidable = target->isCollidable;

                return cevent;
            }
            else
            {
                cout<<"Something is very wrong"<<endl;
            }
        }
    }
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

    float maxProjectionObj2 = currentHitboxFrame->maxProjection(currentAxisAngle, targetObject->global_x,targetObject->global_y);
    float minProjectionObj2 = currentHitboxFrame->minProjection(currentAxisAngle, targetObject->global_x,targetObject->global_y);
    if(maxProjectionObj1>minProjectionObj2 && minProjectionObj1<maxProjectionObj2)
    {
        return true;
    }
    else
    {
        return false;
    }


}
void MissionController::DoMovement(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap, std::map<int,bool> *keyMapHeld)
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
    //cout << "camera.walk: " << camera.walk << endl;

    if((camera.walk) || (missionEnd))
    {
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

        PlayableUnit* farthest_unit = units[farthest_id].get();

        float pataDistance = 240 * booster;

        float diff = (Smoothstep(walkClock.getElapsedTime().asSeconds()/2)*pataDistance)-(Smoothstep(prevTime/2)*pataDistance);
        prevTime = walkClock.getElapsedTime().asSeconds();

        float proposedXPos = farthest_unit->global_x + diff;

        camera.pataSpeed = (2 * 60 * booster);

        //cout << "global_x: " << farthest_unit->global_x << endl;
        //cout << "proposedXPos = " << proposedXPos << endl;
        /// use the right hand side of the patapon sprite to check for collisions. This should be changed if the patapon walks to the left
        //float proposedXPosRight = proposedXPos + farthest_unit->hitBox.left + farthest_unit->hitBox.width;
        /// need to have it check for collision and stop if blocked by kacheek here.

        /// right now it is very basic checking only in X axis. Jumping over a
        /// kacheek will not be possible.

        bool foundCollision = false;
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

        /// if the new position is inside a kacheek, don't move. If we found anything,
        if (!foundCollision)
        {
            if(!missionEnd)
            army_X += diff;
            else
            army_X += 120.0 / fps;
        }
    }

    for(int i=0; i<units.size(); i++)
    {
        PlayableUnit* unit = units[i].get();

        switch(unit->getUnitID())
        {
            case 0: ///Hatapon
            {
                unit->setGlobalPosition(sf::Vector2f(army_X,494));
                break;
            }

            case 1: ///Yaripon
            {
                unit->setGlobalPosition(sf::Vector2f(army_X+100+(50*i),pataponY));
                break;
            }
        }
    }

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
    /// step 2: any projectiles that hit the floor are destroyed
    for(int i=0; i<levelProjectiles.size(); i++)
    {
        Projectile* p = levelProjectiles[i].get();
        float ypos = p->yPos;
        if (ypos>floorY)
        {
            levelProjectiles.erase(levelProjectiles.begin()+i);
        }
    }
    /// step 3: any projectiles that hit any collidableobject are informed
    for(int i=0; i<levelProjectiles.size(); i++)
    {
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
        CollisionEvent cevent = DoCollisionForObject(&tmp,xpos,ypos,0,collisionData); //0 - spear collision ID

        if(cevent.collided)
        {
            if(cevent.isCollidable)
            levelProjectiles.erase(levelProjectiles.begin()+i);

            ///add damage counter
            if(cevent.isAttackable)
            addDmgCounter(0, total, xpos, ypos, qualitySetting, resSetting);
        };
    }

}
void MissionController::Update(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap,std::map<int,bool> *keyMapHeld)
{
    if(!missionEnd)
    missionEndTimer.restart();

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

    missionKeyMap = keyMap;

    int quality = 1;
    float ratioX=1,ratioY=1;

    switch(quality)
    {
        case 0: ///low
        {
            ratioX = window.getSize().x / float(640);
            ratioY = window.getSize().y / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = window.getSize().x / float(1280);
            ratioY = window.getSize().y / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = window.getSize().x / float(1920);
            ratioY = window.getSize().y / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = window.getSize().x / float(3840);
            ratioY = window.getSize().y / float(2160);
            break;
        }
    }

    if(missionEnd)
    {
        if(missionEndTimer.getElapsedTime().asMilliseconds() >= 2500)
        {
            if(!playCheer[0])
            {
                s_cheer.stop();
                s_cheer.setBuffer(sb_cheer1);
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
                s_cheer.play();
                playCheer[2] = true;
            }
        }

        if(missionEndTimer.getElapsedTime().asMilliseconds() >= 8000)
        {
            if(!playJingle)
            {
                s_jingle.setBuffer(sb_win_jingle);
                s_jingle.play();
                playJingle = true;
            }
        }
    }

    if(missionEndTimer.getElapsedTime().asMilliseconds() < 7700)
    {
        camera.followobject_x = army_X * ratioX;
    }

    camera.Work(window,fps,keyMapHeld);
    test_bg.setCamera(camera);
    test_bg.Draw(window);

    DoKeyboardEvents(window,fps,keyMap,keyMapHeld);
    DoMovement(window,fps,keyMap,keyMapHeld);

    // TODO: at some point some pointer shenanigans is required to make these be a reference to v4core's ones too.
    rhythm.rhythmController.keyMap = *missionKeyMap;
    rhythm.rhythmController.config = *missionConfig;
    rhythm.config = *missionConfig;

    vector<int> tlo_rm;

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
            entity->Draw(window);
        }

        if(entity->ready_to_erase)
        tlo_rm.push_back(i);
    }

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

    PlayableUnit* farthest_unit = units[farthest_id].get();

    for(int i=0; i<units.size(); i++)
    {
        PlayableUnit* unit = units[i].get();

        if(!missionEnd)
        {
            unit->doRhythm(rhythm.current_song, rhythm.rhythmController.current_drum, rhythm.GetCombo());

            vector<float> gdistances;
            vector<float> ldistances;

            for (int e=0; e<tangibleLevelObjects.size(); e++)
            {
                //cout << "tangibleLevelObjects[" << i << "]->Draw(window);" << endl;
                ///some temporary code for entity in range detection
                if(tangibleLevelObjects[e]->type == tangibleLevelObjects[e]->HOSTILE)
                {
                    float global_distance = tangibleLevelObjects[e]->getGlobalPosition().x - (farthest_unit->getGlobalPosition().x - farthest_unit->local_x);
                    float local_distance = tangibleLevelObjects[e]->getGlobalPosition().x - farthest_unit->getGlobalPosition().x;
                    gdistances.push_back(global_distance);
                    ldistances.push_back(local_distance);
                }
            }

            /// patapons (and other enemies) are drawn after level objects like kacheek so they are always on top
            if(unit->getUnitID() == 1)
            {
                if(unit->doAttack())
                {
                    cout << "Unit " << i << " threw a spear!" << endl;

                    float rand_hs = (rand() % 1000) / float(10);
                    float rand_vs = (rand() % 1000) / float(10);

                    float rand_rad = (rand() % 200000000) / float(1000000000);

                    //cout << "===============================================" << endl;
                    //cout << "rand_hs: " << rand_hs << " rand_vs: " << rand_vs << endl;
                    //cout << "===============================================" << endl;

                    unique_ptr<Spear> p = make_unique<Spear>(s_proj);
                    p.get()->xPos = unit->getGlobalPosition().x+unit->hitBox.left+unit->hitBox.width/2;
                    p.get()->yPos = unit->getGlobalPosition().y+unit->hitBox.top+unit->hitBox.height/2;
                    p.get()->speed = 800;
                    p.get()->hspeed = 450+rand_hs;
                    p.get()->vspeed = -450+rand_vs;
                    ///-0.523598776 - 30 deg
                    //p.get()->angle = -0.698131701; /// 40 degrees from the floor - pi*4/12
                    p.get()->angle = -0.58 - rand_rad;
                    ///-0.872664626 - 50 deg
                    p.get()->maxdmg = 150;
                    p.get()->mindmg = 50;
                    p.get()->crit = 0;
                    p.get()->crit = 0;

                    levelProjectiles.push_back(std::move(p));
                }
            }
        }

        /*if(gdistances.size() > 0)
        {
            unit->gclosest = *std::min_element(std::begin(gdistances), std::end(gdistances));
            unit->lclosest = *std::min_element(std::begin(ldistances), std::end(ldistances));
        }*/

        if(missionEnd)
        {
            unit->doMissionEnd();
        }

        unit->fps = fps;
        unit->Draw(window);
    }

    if((rhythm.rhythmController.current_drum == "pata") or (rhythm.rhythmController.current_drum == "pon") or (rhythm.rhythmController.current_drum == "chaka") or (rhythm.rhythmController.current_drum == "don"))
    {
        rhythm.rhythmController.current_drum = "";
        rhythm.current_song = "";
    }

    for(int i=0; i<levelProjectiles.size(); i++)
    {
        levelProjectiles[i].get()->Draw(window,fps);
    }

    /// draw static UI elements
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
    window.setView(lastView);

    /// here we show the hitbox
    bool showHitboxes = false;
    if(showHitboxes)
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
                    currentPoint.x = currentPoint.x + currentHitbox->g_x + unit->global_x;
                    currentPoint.y = currentPoint.y + currentHitbox->g_y + unit->global_y;
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
                    currentPoint.x = currentPoint.x + currentHitbox->g_x + entity->global_x;
                    currentPoint.y = currentPoint.y + currentHitbox->g_y + entity->global_y;
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

    for(int i=0; i<dmg_rm.size(); i++)
    {
        cout << "Erased dmgCounter " << dmg_rm[i] << endl;
        dmgCounters.erase(dmgCounters.begin()+(dmg_rm[i] - i));
    }

    rhythm.fps = fps;

    if(showTimer)
    {
        auto lastView2 = window.getView();

        window.setView(window.getDefaultView());
        t_timerMenu.setString(Func::ConvertToUtf8String(std::to_string(missionTimer.getElapsedTime().asSeconds())+" Seconds"));
        t_timerMenu.setOrigin(t_timerMenu.getGlobalBounds().width/2,t_timerMenu.getGlobalBounds().height/2);
        t_timerMenu.setPosition(window.getSize().x/2,100);
        window.draw(t_timerMenu);
        window.setView(lastView2);
    }

    if(!missionEnd)
    {
        rhythm.Draw(window);
    }

    for(int i=0; i<tlo_rm.size(); i++)
    {
        tangibleLevelObjects.erase(tangibleLevelObjects.begin()+(tlo_rm[i]-i));//remove it from vector

        cout << "Erased tangibleLevelObject " << tlo_rm[i] << endl;
        //delete tangibleLevelObjects[tlo_rm[i]];
        //tangibleLevelObjects.erase(tangibleLevelObjects.begin()+(tlo_rm[i] - i));
    }

    lastView = window.getView();
    window.setView(window.getDefaultView());

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

    fade_box.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    fade_box.setFillColor(sf::Color(0,0,0,fade_alpha));
    window.draw(fade_box);

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
    }

    window.setView(lastView);
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
