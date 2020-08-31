#include "Patapolis.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
#include <sstream>
#include "Altar.h"
PatapolisMenu::PatapolisMenu()
{
    //ctor
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");

    isActive=false;
}

void PatapolisMenu::addL6(std::string variant, float x, float y, int q, int r)
{
    PSprite tmp;
    tmp.loadFromFile("resources/graphics/bg/patapolis/6"+variant+".png", q, r);
    tmp.setPosition(x, y);
    layer_6.push_back(tmp);
}

void PatapolisMenu::addL2(std::string variant, float x, float y, int q, int r)
{
    PSprite tmp;
    tmp.loadFromFile("resources/graphics/bg/patapolis/2"+variant+".png", q, r);
    tmp.setPosition(x, y);
    layer_2.push_back(tmp);
}

void PatapolisMenu::addSparkle(float x, float y)
{
    float scale = ((rand() % 400) + 750) / float(1000);

    Sparkle tmp;

    PSprite sprk;
    sprk.loadFromFile("resources/graphics/bg/patapolis/sparkle.png", quality, 1);
    sprk.setPosition(x, y);
    sprk.setScale(scale);
    //sparkles.push_back(sprk);

    tmp.sprk = sprk;
    tmp.initScale = scale;
    tmp.curScale = scale;
    tmp.x = x;
    tmp.y = y;
    tmp.baseX = x;

    sparkles.push_back(tmp);
}

void PatapolisMenu::addParagetSparkle(float x, float y)
{
    float scale = ((rand() % 400) + 750) / float(1000);
    float angle = (rand() % 36000) / float(100);

    ParagetSparkle tmp;

    PSprite sprk;

    int choice = rand() % 3 + 1;
    string nm = "paraget_sparkle_"+to_string(choice)+".png";

    sprk.loadFromFile("resources/graphics/bg/patapolis/"+nm, quality, 1);

    sprk.setPosition(x, y);
    sprk.setOrigin(sprk.getLocalBounds().width/2, sprk.getLocalBounds().height/2);
    sprk.setRotation(angle);
    sprk.setScale(scale);
    //sparkles.push_back(sprk);

    tmp.sprk = sprk;
    tmp.initScale = scale;
    tmp.curScale = scale;
    tmp.x = x;
    tmp.y = y;
    tmp.angle = angle;
    tmp.baseX = x;
    tmp.baseY = y;

    paraget_sparkles.push_back(tmp);
}

void PatapolisMenu::addRay(float x1, float y1, float x2, float y2)
{
    RayStart rs;
    rs.x1 = x1;
    rs.y1 = 5 + y1;
    rs.x2 = x2;
    rs.y2 = 5 + y2;

    rs.ox1 = x1;
    rs.ox2 = x2;

    coords.push_back(rs);
}

PatapolisMenu::Fire PatapolisMenu::addFire(int type, float x, float y, bool add)
{
    Fire tmp;

    string str_type = "";

    switch(type)
    {
        case 0:
        {
            str_type = "small";
            break;
        }

        case 1:
        {
            str_type = "med";
            break;
        }

        case 2:
        {
            str_type = "large";
            break;
        }

        case 3:
        {
            str_type = "purple";
            break;
        }

        case 4:
        {
            str_type = "market";
            break;
        }
    }

    tmp.fire[0].loadFromFile("resources/graphics/bg/patapolis/"+str_type+"_fire1.png", quality, 1);
    tmp.fire[1].loadFromFile("resources/graphics/bg/patapolis/"+str_type+"_fire2.png", quality, 1);
    tmp.fire[2].loadFromFile("resources/graphics/bg/patapolis/"+str_type+"_fire3.png", quality, 1);

    tmp.glow.loadFromFile("resources/graphics/bg/patapolis/"+str_type+"_glow.png", quality, 1);

    tmp.baseX = x;
    tmp.baseY = y;
    tmp.x = x;
    tmp.y = y;

    if(add)
    fires.push_back(tmp);

    return tmp;
}

void PatapolisMenu::addSmokeParticle(float x, float y, PSprite& refer)
{
    SmokeParticle tmp;
    tmp.smk = refer;
    tmp.x = x;
    tmp.y = y;
    tmp.baseX = x;
    tmp.baseY = y;
    tmp.curScale = 0.1;
    smoke.push_back(tmp);
}

void PatapolisMenu::Initialise(Config *thisConfigs,V4Core *parent, Menu *curParentMenu)
{
    parent->SaveToDebugLog("Initializing Patapolis...");

    sf::Context context;
    Scene::Initialise(thisConfigs,parent);
    altar_menu.Initialise(thisConfigs,parent,this);
    barracks_menu.Initialise(thisConfigs,parent,this);
    obelisk_menu.Initialise(thisConfigs,parent,this);
    parentMenu = curParentMenu;
    quality = thisConfig->GetInt("textureQuality");

    float resRatioX = thisConfig->GetInt("resX") / float(1280);
    float resRatioY = thisConfig->GetInt("resY") / float(720);

    t_title.createText(f_font, 38, sf::Color::White, "", quality, 1);
    t_title.t.setOutlineThickness(2);
    t_title.t.setOutlineColor(sf::Color::Black);

    string vx_params = "0,24,128,238;66,24,128,238;444,184,243,202;591,184,243,202;592,255,255,255;710,171,243,214;720,171,243,214";

    vector<string> v_vxparams = Func::Split(vx_params,';');
    std::vector<sf::Vector2f> vx_pos;
    std::vector<sf::Color> vx_color;

    for(int i=0; i<v_vxparams.size(); i++)
    {
        vector<string> tmp = Func::Split(v_vxparams[i],',');

        sf::Vector2f tmp_vector;
        sf::Color tmp_color;

        tmp_vector.x = 0;
        tmp_vector.y = atof(tmp[0].c_str()) * resRatioY;

        if(tmp[0] == "-1")
        {
            tmp_vector.y = 720 * resRatioY;
        }

        tmp_color.r = atoi(tmp[1].c_str());
        tmp_color.g = atoi(tmp[2].c_str());
        tmp_color.b = atoi(tmp[3].c_str());

        sf::Vector2f tmp_vector2;

        tmp_vector2.x = 1280 * resRatioX;
        tmp_vector2.y = atof(tmp[0].c_str()) * resRatioY;

        if(tmp[0] == "-1")
        {
            tmp_vector2.y = 720 * resRatioY;
        }

        vx_pos.push_back(tmp_vector);
        vx_color.push_back(tmp_color);

        vx_pos.push_back(tmp_vector2);
        vx_color.push_back(tmp_color);
    }

    sf::VertexArray tmp(sf::TrianglesStrip,vx_pos.size());
    v_background = tmp;

    for(int i=0; i<vx_pos.size(); i++)
    {
        v_background[i].position = vx_pos[i];
        v_background[i].color = vx_color[i];

        cout << "vx_pos: " << vx_pos[i].x << " " << vx_pos[i].y << endl;
    }

    floor_height = 54;
    r_ground.setSize(sf::Vector2f(11500*resRatioX, floor_height*resRatioY));
    r_ground.setFillColor(sf::Color::Black);

    addL6("a", 0, 722-floor_height, quality, 1);
    addL6("a", 2700, 722-floor_height, quality, 1);

    addL6("d", 3840, 726-floor_height, quality, 1);
    addL6("c", 3800, 722-floor_height, quality, 1);

    addL6("d", 4400, 740-floor_height, quality, 1);
    addL6("c", 4650, 740-floor_height, quality, 1);
    addL6("b", 4900, 726-floor_height, quality, 1);
    addL6("d", 5100, 737-floor_height, quality, 1);
    addL6("c", 5050, 725-floor_height, quality, 1);
    addL6("d", 5500, 745-floor_height, quality, 1);
    addL6("c", 5700, 723-floor_height, quality, 1);
    addL6("d", 5760, 724-floor_height, quality, 1);
    addL6("a", 5850, 722-floor_height, quality, 1);
    addL6("c", 6100, 722-floor_height, quality, 1);
    addL6("d", 6150, 733-floor_height, quality, 1);
    addL6("c", 6400, 755-floor_height, quality, 1);
    addL6("b", 6650, 722-floor_height, quality, 1);

    addL6("e", 7640, 722-floor_height, quality, 1);
    addL6("d", 7700, 731-floor_height, quality, 1);
    addL6("c", 7670, 724-floor_height, quality, 1);
    addL6("c", 7900, 750-floor_height, quality, 1);

    addL6("d", 8250, 727-floor_height, quality, 1);
    addL6("c", 8480, 742-floor_height, quality, 1);
    addL6("d", 8710, 734-floor_height, quality, 1);
    addL6("c", 8880, 723-floor_height, quality, 1);
    addL6("d", 8930, 733-floor_height, quality, 1);
    addL6("c", 9040, 757-floor_height, quality, 1);
    addL6("d", 9180, 736-floor_height, quality, 1);
    addL6("a", 9120, 722-floor_height, quality, 1);
    addL6("c", 9260, 743-floor_height, quality, 1);

    addL6("c", 9700, 728-floor_height, quality, 1);
    addL6("d", 9740, 735-floor_height, quality, 1);
    addL6("c", 9880, 760-floor_height, quality, 1);
    addL6("d", 9930, 750-floor_height, quality, 1);
    addL6("d", 10040, 737-floor_height, quality, 1);

    L5.loadFromFile("resources/graphics/bg/patapolis/5.png", quality, 1);
    L4.loadFromFile("resources/graphics/bg/patapolis/4.png", quality, 1);

    L5.setPosition(3060, 740-floor_height);
    L4.setPosition(2530, 750-floor_height);

    addL2("a", 800, 766-floor_height, quality, 1);
    addL2("b", 2900, 740-floor_height, quality, 1);
    addL2("c", 3900, 720-floor_height, quality, 1);
    addL2("c", 4300, 770-floor_height, quality, 1);

    addL2("c", 5080, 750-floor_height, quality, 1);
    addL2("c", 5220, 720-floor_height, quality, 1);

    addL2("c", 5600, 770-floor_height, quality, 1);
    addL2("c", 5740, 720-floor_height, quality, 1);

    addL2("c", 6150, 760-floor_height, quality, 1);
    addL2("c", 6320, 720-floor_height, quality, 1);

    addL2("c", 6540, 760-floor_height, quality, 1);

    addL2("c", 7540, 760-floor_height, quality, 1);

    addL2("c", 7800, 720-floor_height, quality, 1);
    addL2("c", 8000, 750-floor_height, quality, 1);

    addL2("c", 8900, 770-floor_height, quality, 1);
    addL2("c", 9060, 740-floor_height, quality, 1);

    addL2("c", 9760, 740-floor_height, quality, 1);

    addL2("c", 10300, 770-floor_height, quality, 1);
    addL2("c", 10470, 740-floor_height, quality, 1);

    addL2("c", 11000, 750-floor_height, quality, 1);

    addL2("d", 300, 728-floor_height, quality, 1);
    addL2("d", 940, 728-floor_height, quality, 1);
    addL2("e", 1400, 728-floor_height, quality, 1);
    addL2("f", 1800, 728-floor_height, quality, 1);
    addL2("d", 4200, 728-floor_height, quality, 1);
    addL2("d", 5000, 728-floor_height, quality, 1);
    addL2("d", 5580, 728-floor_height, quality, 1);
    addL2("d", 6400, 728-floor_height, quality, 1);
    addL2("d", 7700, 728-floor_height, quality, 1);
    addL2("d", 9000, 728-floor_height, quality, 1);
    addL2("d", 10500, 728-floor_height, quality, 1);

    edge.loadFromFile("resources/graphics/bg/patapolis/edge.png", quality, 1);
    bridge.loadFromFile("resources/graphics/bg/patapolis/bridge.png", quality, 1);
    rainbow.loadFromFile("resources/graphics/bg/patapolis/rainbow.png", quality, 1);

    addSparkle(11620 + 25, 400);
    addSparkle(11620 + 170, 380);
    addSparkle(11620 + 83, 270);
    addSparkle(11620 + 180, 265);
    addSparkle(11620 + 283, 295);
    addSparkle(11620 + 242, 159);
    addSparkle(11620 + 333, 235);
    addSparkle(11620 + 455, 261);
    addSparkle(11620 + 416, 100);
    addSparkle(11620 + 598, 127);
    addSparkle(11620 + 506, 186);
    addSparkle(11620 + 605, 257);
    addSparkle(11620 + 722, 203);
    addSparkle(11620 + 711, 332);
    addSparkle(11620 + 831, 341);
    addSparkle(11620 + 937, 401);
    addSparkle(11620 + 860, 244);

    wakapon.loadFromFile("resources/graphics/bg/patapolis/wakapon.png", quality, 1);
    wakapon.setPosition(11930, 462);
    wakapon.setOrigin(wakapon.getLocalBounds().width/2, wakapon.getLocalBounds().height);

    world_egg.loadFromFile("resources/graphics/bg/patapolis/egg.png", quality, 1);
    world_egg.setPosition(12215, 462);
    world_egg.setOrigin(world_egg.getLocalBounds().width/2, world_egg.getLocalBounds().height);

    light_1.loadFromFile("resources/graphics/bg/patapolis/light.png", quality, 1);
    light_1.setPosition(11715, 161);
    light_1.setOrigin(light_1.getLocalBounds().width/2, light_1.getLocalBounds().height/2);

    light_2.loadFromFile("resources/graphics/bg/patapolis/light.png", quality, 1);
    light_2.setPosition(12720, 161);
    light_2.setOrigin(light_2.getLocalBounds().width/2, light_2.getLocalBounds().height/2);

    egg_light.loadFromFile("resources/graphics/bg/patapolis/egg_light.png", quality, 1);
    egg_light.setPosition(12217, 288);
    egg_light.setOrigin(egg_light.getLocalBounds().width/2, egg_light.getLocalBounds().height/2);

    addRay(10, 331, 18, 321);
    addRay(21, 321, 25, 316);
    addRay(28, 313, 32, 308);
    addRay(34, 306, 37, 300);
    addRay(39, 298, 41, 291);
    addRay(41, 278, 39, 271);
    addRay(36, 254, 33, 246);
    addRay(31, 242, 27, 235);
    addRay(24, 232, 19, 226);
    addRay(16, 223, 9, 217);
    addRay(7, 214, 0, 211);
    addRay(2, 210, -2, 210);

    for(int i=11; i>=0; i--)
    {
        float x1 = coords[i].ox2 * (-1);
        float x2 = coords[i].ox1 * (-1);

        addRay(x1, coords[i].y2, x2, coords[i].y1);
    }

    for(int i=0; i<coords.size(); i++)
    {
        LightRay tmp;
        tmp.angle = 30 + (i * (float(300)/float(coords.size())));
        tmp.max_distance = 110 + rand() % 20;
        tmp.min_distance = 10 + rand() % 20;
        tmp.cur_distance = rand() % 110 + 20;



        sf::VertexArray vtmp(sf::TrianglesStrip, 3);
        vtmp[0].position = sf::Vector2f(coords[i].x1, coords[i].y1);
        vtmp[0].color = sf::Color(215,246,255,255);
        vtmp[1].position = sf::Vector2f(coords[i].x2, coords[i].y2);
        vtmp[1].color = sf::Color(215,246,255,255);
        vtmp[2].position = sf::Vector2f(((coords[i].x1 + coords[i].x2) / 2) + (tmp.cur_distance * sin(tmp.angle*3.14159265/180)), ((coords[i].y1 + coords[i].y2) / 2) + (tmp.cur_distance * cos(tmp.angle*3.14159265/180)));
        vtmp[2].color = sf::Color(215,246,255,255);
        tmp.triangle = vtmp;

        cout << "[coord #" << i << "]" << vtmp[0].position.x << " " << vtmp[0].position.y << " " << vtmp[1].position.x << " " << vtmp[1].position.y << " " << vtmp[2].position.x << " " << vtmp[2].position.y << " " << tmp.angle << " " << tmp.cur_distance << " " << tmp.min_distance << " " << tmp.max_distance << endl;

        lightrays.push_back(tmp);
    }

    market.loadFromFile("resources/graphics/bg/patapolis/market.png", quality, 1);
    market.setPosition(640, 728-floor_height);
    market.setOrigin(market.getLocalBounds().width/2, market.getLocalBounds().height);

    forge_main.loadFromFile("resources/graphics/bg/patapolis/forge_main.png", quality, 1);
    forge_main.setPosition(2300, 720-floor_height);
    forge_main.setOrigin(forge_main.getLocalBounds().width/2, forge_main.getLocalBounds().height);

    forge_back.loadFromFile("resources/graphics/bg/patapolis/blacksmith_forge.png", quality, 1);
    forge_back.setPosition(2300, 720-floor_height-29);
    forge_back.setOrigin(forge_back.getLocalBounds().width/2, forge_back.getLocalBounds().height);

    forge_glow.loadFromFile("resources/graphics/bg/patapolis/blacksmith_forge_glow.png", quality, 1);
    forge_glow.setPosition(2300, 720-floor_height-178);
    forge_glow.setOrigin(forge_glow.getLocalBounds().width/2, forge_glow.getLocalBounds().height);

    forge_fence.loadFromFile("resources/graphics/bg/patapolis/blacksmith_fence.png", quality, 1);
    forge_fence.setPosition(2043, 720-floor_height-306);
    forge_fence.setOrigin(forge_fence.getLocalBounds().width/2, 0);

    forge_slab.loadFromFile("resources/graphics/bg/patapolis/slab.png", quality, 1);
    forge_slab.setPosition(2300-65, 720-floor_height-84);
    forge_slab.setOrigin(forge_slab.getLocalBounds().width/2, forge_slab.getLocalBounds().height);

    forge_slab_glow.loadFromFile("resources/graphics/bg/patapolis/slab_glow.png", quality, 1);
    forge_slab_glow.setPosition(2300-72, 720-floor_height-92);
    forge_slab_glow.setOrigin(forge_slab_glow.getLocalBounds().width/2, forge_slab_glow.getLocalBounds().height);

    barracks.loadFromFile("resources/graphics/bg/patapolis/barracks.png", quality, 1);
    barracks.setPosition(3960, 765-floor_height);
    barracks.setOrigin(barracks.getLocalBounds().width/2, barracks.getLocalBounds().height);

    festival_main.loadFromFile("resources/graphics/bg/patapolis/festival_main.png", quality, 1);
    festival_main.setPosition(5620, 720-floor_height);
    festival_main.setOrigin(festival_main.getLocalBounds().width/2, festival_main.getLocalBounds().height);

    altar.loadFromFile("resources/graphics/bg/patapolis/altar.png", quality, 1);
    altar.setPosition(7280, 720-floor_height);
    altar.setOrigin(altar.getLocalBounds().width/2, altar.getLocalBounds().height);

    obelisk.loadFromFile("resources/graphics/bg/patapolis/obelisk.png", quality, 1);
    obelisk.setPosition(8940, 720-floor_height);
    obelisk.setOrigin(obelisk.getLocalBounds().width/2, obelisk.getLocalBounds().height);

    paraget_main.loadFromFile("resources/graphics/bg/patapolis/paraget_main.png", quality, 1);
    paraget_main.setPosition(10600, 722-floor_height);
    paraget_main.setOrigin(paraget_main.getLocalBounds().width/2, paraget_main.getLocalBounds().height);

    paraget_crystal_glow.loadFromFile("resources/graphics/bg/patapolis/paraget_crystal_glow.png", quality, 1);
    paraget_crystal_glow.setPosition(10600, 430);
    paraget_crystal_glow.setOrigin(paraget_crystal_glow.getLocalBounds().width/2, paraget_crystal_glow.getLocalBounds().height/2);

    paraget_crystal.loadFromFile("resources/graphics/bg/patapolis/paraget_crystal.png", quality, 1);
    paraget_crystal.setPosition(10600, 430);
    paraget_crystal.setOrigin(paraget_crystal.getLocalBounds().width/2, paraget_crystal.getLocalBounds().height/2);

    locations.push_back(market.getPosition().x - 640);
    locations.push_back(forge_main.getPosition().x - 640);
    locations.push_back(barracks.getPosition().x - 640);
    locations.push_back(festival_main.getPosition().x - 640);
    locations.push_back(altar.getPosition().x - 640);
    locations.push_back(obelisk.getPosition().x - 640);
    locations.push_back(paraget_main.getPosition().x - 640);
    locations.push_back(11290);
    locations.push_back(11569);

    int range = rand() % 30 + 50;
    for(int i=0; i<range; i++)
    {
        addParagetSparkle(10549 + (rand()%100), 330 + (rand() % 200));
    }

    addFire(4, 583, 583, true);
    addFire(4, 697, 583, true);

    addFire(2, 5816, 574, true);
    addFire(2, 5431, 574, true);
    addFire(1, 5207, 368, true);
    addFire(1, 6039, 368, true);

    addFire(0, 8775, 479, true);
    addFire(0, 9105, 479, true);

    forge_big = addFire(2, 2050, 542, false);
    forge_purple = addFire(3, 2370, 404, false);

    p_smoke.loadFromFile("resources/graphics/bg/patapolis/smoke.png", quality, 1);
    p_smoke.setOrigin(p_smoke.getLocalBounds().width/2, p_smoke.getLocalBounds().height/2);

    ctrlTips.create(54, f_font, 20, sf::String(L"L/R: Move      ×: Interact      Select: Save      Start: Title screen"), quality);

    initialised=true;

    SetTitle(location);
    camPos = locations[location];

    if (doWaitKeyPress)
    {
        v4core->LoadingWaitForKeyPress();
        v4core->ChangeRichPresence("In Patapolis", "logo", "");
    }

    parent->SaveToDebugLog("Initializing Patapolis finished.");
}
void PatapolisMenu::EventFired(sf::Event event)
{
    if (altar_menu.isActive)
    {
        altar_menu.EventFired(event);
    }
    else if (barracks_menu.isActive)
    {
        barracks_menu.EventFired(event);
    }
    else if (obelisk_menu.isActive)
    {
        obelisk_menu.EventFired(event);
    }
    else if(isActive)
    {
        if(event.type == sf::Event::KeyPressed)
        {
            /*if (event.key.code == thisConfig->GetInt("keybindBack"))
            {
                thisConfig->thisCore->SaveToDebugLog("Left from Patapolis to Title screen.");
                this->Hide();
                this->isActive = false;
                parentMenu->Show();
                parentMenu->isActive=true;
            }*/

        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        }
    }

}
void PatapolisMenu::SetTitle(int menuPosition)
{
    sf::String a = L"L/R: Move      ";

    switch(menuPosition)
    {
    case 0:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_trader")));
        break;
    case 1:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_blacksmith")));
        break;
    case 2:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_barracks")));
        a += L"×: Interact      ";
        break;
    case 3:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_festival")));
        break;
    case 4:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_altar")));
        a += L"×: Interact      ";
        break;
    case 5:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_obelisk")));
        a += L"×: Interact      ";
        break;
    case 6:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_paraget")));
        break;
    case 7:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_wakapon")));
        break;
    case 8:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_egg")));
        break;
    default:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis")));
        break;
    }

    a += L"Select: Save      Start: Title screen";

    ctrlTips.create(54, f_font, 20, a, quality);
}
float EaseIn (float time, float startValue, float change, float duration)
{
    time /= duration / 2;
    if (time < 1)
    {
        return change / 2 * time * time + startValue;
    }

    time--;
    return -change / 2 * (time * (time - 2) - 1) + startValue;
};
void PatapolisMenu::Update(sf::RenderWindow &window, float fps, InputController& inputCtrl)
{
    if(isActive)
    {
        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        window.draw(v_background);

        camDest = locations[location];

        float camDistance = abs(camDest - camPos);
        float camSpeed = camDistance * 3;

        if(camPos != camDest)
        {
            if(left)
            {
                if(camPos < locations[location])
                camPos += camSpeed / fps;
                else
                camPos -= camSpeed / fps;
            }
            else
            {
                if(camPos > locations[location])
                camPos -= camSpeed / fps;
                else
                camPos += camSpeed / fps;
            }
        }

        for(int i=0; i<layer_6.size(); i++)
        {
            //cout << "layer_6[" << i << "]: " << layer_6[i].getGlobalBounds().width/2 << " " << layer_6[i].getGlobalBounds().height << " " << layer_6[i].getPosition().x << " " << layer_6[i].getPosition().y << endl;
            layer_6[i].lx = layer_6[i].baseX - (camPos / 1.1428571428571428571428571428571);

            layer_6[i].setOrigin(layer_6[i].getLocalBounds().width/2, layer_6[i].getLocalBounds().height);
            layer_6[i].draw(window);
        }

        L5.lx = L5.baseX - (camPos / 1.1111111111111111111);
        L4.lx = L4.baseX - (camPos / 1.0526315789473684210526315789474);
        wakapon.lx = wakapon.baseX - camPos;
        world_egg.lx = world_egg.baseX - camPos;
        light_1.lx = light_1.baseX - camPos;
        light_2.lx = light_2.baseX - camPos;
        egg_light.lx = egg_light.baseX - camPos;

        market.lx = market.baseX - camPos;
        forge_main.lx = forge_main.baseX - camPos;
        forge_back.lx = forge_back.baseX - camPos;
        forge_glow.lx = forge_glow.baseX - camPos;
        forge_fence.lx = forge_fence.baseX - camPos;
        forge_slab.lx = forge_slab.baseX - camPos;
        forge_slab_glow.lx = forge_slab_glow.baseX - camPos;
        barracks.lx = barracks.baseX - camPos;
        festival_main.lx = festival_main.baseX - camPos;
        altar.lx = altar.baseX - camPos;
        obelisk.lx = obelisk.baseX - camPos;
        paraget_main.lx = paraget_main.baseX - camPos;
        paraget_crystal.lx = paraget_crystal.baseX - camPos;
        paraget_crystal_glow.lx = paraget_crystal_glow.baseX - camPos;

        rayX = rayXbase - camPos;

        floor_x = floor_x_base - camPos;

        L5.setOrigin(L5.getLocalBounds().width/2, L5.getLocalBounds().height);
        L4.setOrigin(L4.getLocalBounds().width/2, L4.getLocalBounds().height);

        L5.draw(window);
        L4.draw(window);


        for(int i=0; i<layer_2.size(); i++)
        {
            //cout << "layer_6[" << i << "]: " << layer_6[i].getGlobalBounds().width/2 << " " << layer_6[i].getGlobalBounds().height << " " << layer_6[i].getPosition().x << " " << layer_6[i].getPosition().y << endl;
            layer_2[i].lx = layer_2[i].baseX - camPos;

            layer_2[i].setOrigin(layer_2[i].getLocalBounds().width/2, layer_2[i].getLocalBounds().height);
            layer_2[i].draw(window);
        }

        float resRatioX = window.getSize().x / float(1280);
        float resRatioY = window.getSize().y / float(720);

        market.draw(window);

        forge_big.id += float(12) / fps;

        if(round(forge_big.id) >= 3)
        forge_big.id = 0;

        int id = round(forge_big.id);

        forge_big.glowsize += float(90) / fps;

        if(forge_big.glowsize >= 360)
        forge_big.glowsize -= 360;

        forge_big.glow.setOrigin(forge_big.glow.getLocalBounds().width/2, forge_big.glow.getLocalBounds().height/2);
        forge_big.glow.setScale(1.05 + (cos(forge_big.glowsize * 3.141592 / 180) / 10));
        forge_big.glow.setPosition(forge_big.baseX - camPos, forge_big.baseY);

        forge_big.glow.draw(window);

        forge_big.fire[id].setOrigin(forge_big.fire[id].getLocalBounds().width/2, forge_big.fire[id].getLocalBounds().height/2);
        forge_big.fire[id].setPosition(forge_big.baseX - camPos, forge_big.baseY);

        forge_big.fire[id].draw(window);

        forge_back.draw(window);

        forge_glowsize += float(90) / fps;

        if(forge_glowsize >= 360)
        forge_glowsize = 0;

        forge_glow.setOrigin(forge_glow.getLocalBounds().width/2, forge_glow.getLocalBounds().height/2);
        forge_glow.setScale(1.05 + (cos(forge_glowsize * 3.141592 / 180) / 10));
        forge_glow.draw(window);

        forge_purple.id += float(12) / fps;

        if(round(forge_purple.id) >= 3)
        forge_purple.id = 0;

        id = round(forge_purple.id);

        forge_purple.glowsize += float(90) / fps;

        if(forge_purple.glowsize >= 360)
        forge_purple.glowsize -= 360;

        forge_purple.glow.setOrigin(forge_purple.glow.getLocalBounds().width/2, forge_purple.glow.getLocalBounds().height/2);
        forge_purple.glow.setScale(1.05 + (cos(forge_purple.glowsize * 3.141592 / 180) / 10));
        forge_purple.glow.setPosition(forge_purple.baseX - camPos, forge_purple.baseY);

        forge_purple.glow.draw(window);

        forge_purple.fire[id].setOrigin(forge_purple.fire[id].getLocalBounds().width/2, forge_purple.fire[id].getLocalBounds().height/2);
        forge_purple.fire[id].setPosition(forge_purple.baseX - camPos, forge_purple.baseY);

        forge_purple.fire[id].draw(window);

        forge_slab_glow.setOrigin(forge_slab_glow.getLocalBounds().width/2, forge_slab_glow.getLocalBounds().height/2);
        forge_slab_glow.setScale(1.05 + (cos(forge_glowsize * 3.141592 / 180) / 10));
        forge_slab_glow.draw(window);

        forge_main.draw(window);

        forge_slab.draw(window);

        forge_fence.draw(window);

        barracks.draw(window);

        crystal_y += float(90) / fps;

        if(crystal_y >= 360)
        crystal_y -= 360;

        crystal_y2 += float(30) / fps;

        if(crystal_y2 >= 360)
        crystal_y2 -= 360;

        paraget_crystal.ly = paraget_crystal.baseY + sin(crystal_y * 3.141592 / 180) * 10;
        paraget_crystal_glow.ly = paraget_crystal.baseY + sin(crystal_y * 3.141592 / 180) * 10;

        paraget_crystal_glow.setScale(1.05 + (cos(crystal_y2 * 3.141592 / 180) / 10));
        paraget_crystal_glow.draw(window);
        paraget_main.draw(window);
        paraget_crystal.draw(window);

        for(int i=0; i<paraget_sparkles.size(); i++)
        {
            paraget_sparkles[i].x = paraget_sparkles[i].baseX - camPos;

            if(paraget_sparkles[i].alpha > 0)
            {
                paraget_sparkles[i].alpha -= float(300) / fps;
            }
            else
            {
                paraget_sparkles[i].alpha = 0;
            }

            if(paraget_sparkles[i].curScale > 0)
            {
                paraget_sparkles[i].curScale -= float(1.35) / fps;
            }
            else
            {
                paraget_sparkles[i].curScale = 0;
            }

            if(paraget_sparkles[i].curScale <= 0)
            {
                paraget_sparkles[i].timer -= float(1000) / fps;
            }

            if(paraget_sparkles[i].timer <= 0)
            {
                paraget_sparkles[i].curScale = paraget_sparkles[i].initScale;
                paraget_sparkles[i].alpha = 255;
                paraget_sparkles[i].timer = 400 + (rand() % 200);
                paraget_sparkles[i].y = paraget_sparkles[i].baseY + sin(crystal_y * 3.141592 / 180) * 10;
            }

            paraget_sparkles[i].sprk.setColor(sf::Color(255,255,255,paraget_sparkles[i].alpha));
            paraget_sparkles[i].sprk.setScale(paraget_sparkles[i].curScale);
            paraget_sparkles[i].sprk.setOrigin(paraget_sparkles[i].sprk.getLocalBounds().width/2, paraget_sparkles[i].sprk.getLocalBounds().height/2);
            paraget_sparkles[i].sprk.setPosition(paraget_sparkles[i].x, paraget_sparkles[i].y);
            paraget_sparkles[i].sprk.draw(window);
        }

        for(int i=0; i<fires.size(); i++)
        {
            fires[i].id += float(10) / fps;

            if(round(fires[i].id) >= 3)
            fires[i].id = 0;

            int id = round(fires[i].id);

            fires[i].glowsize += float(90) / fps;

            if(fires[i].glowsize >= 360)
            fires[i].glowsize -= 360;

            fires[i].glow.setOrigin(fires[i].glow.getLocalBounds().width/2, fires[i].glow.getLocalBounds().height/2);
            fires[i].glow.setScale(1.05 + (cos(fires[i].glowsize * 3.141592 / 180) / 10));
            fires[i].glow.setPosition(fires[i].baseX - camPos, fires[i].baseY);

            fires[i].glow.draw(window);

            fires[i].fire[id].setOrigin(fires[i].fire[id].getLocalBounds().width/2, fires[i].fire[id].getLocalBounds().height/2);
            fires[i].fire[id].setPosition(fires[i].baseX - camPos, fires[i].baseY);

            fires[i].fire[id].draw(window);
        }

        festival_main.draw(window);
        obelisk.draw(window);

        smokepath1 += float(40) / fps;

        if(smokepath1 >= 360)
        smokepath1 = 0;

        if(rand() % 100 == 1)
        addSmokeParticle(altar.baseX - 186 + (cos(smokepath1 * 3.141592 / 180) * 2), 410, p_smoke);
        if(rand() % 100 == 1)
        addSmokeParticle(altar.baseX + 186 + (sin(smokepath1 * 3.141592 / 180) * 2), 410, p_smoke);

        //cout << smoke.size() << endl;

        for(auto i=smoke.begin(); i!=smoke.end(); i++)
        {
            int s = std::distance(smoke.begin(), i);

            smoke[s].y -= float(20) / fps;
            smoke[s].x = smoke[s].baseX - camPos;

            smoke[s].alpha -= float(60) / fps;

            if(smoke[s].alpha <= 0)
            smoke[s].alpha = 0;

            smoke[s].curScale += float(0.14) / fps;

            smoke[s].smk.setPosition(smoke[s].x, smoke[s].y);
            smoke[s].smk.setColor(sf::Color(255,255,255,smoke[s].alpha));
            smoke[s].smk.setScale(smoke[s].curScale);

            //cout << "[SMOKE " << i << "] " << smoke[s].smk.lx << " " << smoke[s].smk.ly << endl;
            smoke[s].smk.draw(window);

            if(smoke[s].alpha <= 0)
            {
                smoke.erase(i--);
            }
        }

        altar.draw(window);

        r_ground.setPosition(floor_x * resRatioX, (float(720) - floor_height) * resRatioY);
        window.draw(r_ground);

        edge.setOrigin(0, edge.getLocalBounds().height);
        edge.setPosition(floor_x + 11500, 720);
        edge.draw(window);

        rainbow.setOrigin(0, rainbow.getLocalBounds().height);
        rainbow.setPosition(floor_x + 11620, 592);
        rainbow.draw(window);

        for(int i=0; i<sparkles.size(); i++)
        {
            sparkles[i].x = sparkles[i].baseX - camPos;

            if(sparkles[i].alpha > 0)
            {
                sparkles[i].alpha -= float(300) / fps;
            }
            else
            {
                sparkles[i].alpha = 0;
            }

            if(sparkles[i].curScale > 0)
            {
                sparkles[i].curScale -= float(1.35) / fps;
            }
            else
            {
                sparkles[i].curScale = 0;
            }

            if(sparkles[i].curScale <= 0)
            {
                sparkles[i].timer -= float(1000) / fps;
            }

            if(sparkles[i].timer <= 0)
            {
                sparkles[i].curScale = sparkles[i].initScale;
                sparkles[i].alpha = 255;
                sparkles[i].timer = 900 + (rand() % 200);
            }

            sparkles[i].sprk.setColor(sf::Color(255,255,255,sparkles[i].alpha));
            sparkles[i].sprk.setScale(sparkles[i].curScale);
            sparkles[i].sprk.setOrigin(sparkles[i].sprk.getLocalBounds().width/2, sparkles[i].sprk.getLocalBounds().height/2);
            sparkles[i].sprk.setPosition(sparkles[i].x, sparkles[i].y);
            sparkles[i].sprk.draw(window);
        }

        if(!light)
        {
            light_1.setScale(light_1.scaleX + (float(0.15) / fps));
            light_2.setScale(light_2.scaleX + (float(0.15) / fps));
            egg_light.setScale(egg_light.scaleX + (float(0.15) / fps));

            if(light_1.scaleX > 1.15)
            {
                light = true;
                light_1.setScale(1.15);
                light_2.setScale(1.15);
                egg_light.setScale(1.15);
            }
        }
        else
        {
            light_1.setScale(light_1.scaleX - (float(0.15) / fps));
            light_2.setScale(light_2.scaleX - (float(0.15) / fps));
            egg_light.setScale(egg_light.scaleX - (float(0.15) / fps));

            if(light_1.scaleX < 0.9)
            {
                light = false;
                light_1.setScale(0.9);
                light_2.setScale(0.9);
                egg_light.setScale(0.9);
            }
        }

        light_1.draw(window);
        light_2.draw(window);
        egg_light.draw(window);

        for(int i=0; i<lightrays.size(); i++)
        {
            if(i != 11)
            {
                if(!lightrays[i].rise)
                {
                    lightrays[i].cur_distance -= float(80) / fps;

                    if(lightrays[i].cur_distance <= lightrays[i].min_distance)
                    {
                        lightrays[i].cur_distance = lightrays[i].min_distance;
                        lightrays[i].rise = true;
                    }
                }
                else
                {
                    lightrays[i].cur_distance += float(80) / fps;

                    if(lightrays[i].cur_distance >= lightrays[i].max_distance)
                    {
                        lightrays[i].cur_distance = lightrays[i].max_distance;
                        lightrays[i].rise = false;
                    }
                }

                float x1 = coords[i].x1 + rayX;
                float y1 = coords[i].y1;

                float x2 = coords[i].x2 + rayX;
                float y2 = coords[i].y2;

                lightrays[i].triangle[0].position = sf::Vector2f(x1*resRatioX, y1*resRatioY);
                lightrays[i].triangle[1].position = sf::Vector2f(x2*resRatioX, y2*resRatioY);
                lightrays[i].triangle[2].position = sf::Vector2f((((x1 + x2) / 2) + (lightrays[i].cur_distance * sin(lightrays[i].angle*3.14159265/180))) * resRatioX, (((y1 + y2) / 2) + (lightrays[i].cur_distance * cos(lightrays[i].angle*3.14159265/180))) * resRatioY);

                //cout << "[coord #" << i << "]" << lightrays[i].triangle[0].position.x << " " << lightrays[i].triangle[0].position.y << " " << lightrays[i].triangle[1].position.x << " " << lightrays[i].triangle[1].position.y << " " << lightrays[i].triangle[2].position.x << " " << lightrays[i].triangle[2].position.y << endl;

                window.draw(lightrays[i].triangle);
            }
        }

        bridge.setOrigin(0, bridge.getLocalBounds().height);
        bridge.setPosition(floor_x + 11200, 720);
        bridge.draw(window);

        wakapon.draw(window);
        world_egg.draw(window);

        if(barracks_menu.missionStarted)
        {
            obelisk_menu.displayMissions = false;
            obelisk_menu.sel_location = 1;
            obelisk_menu.sel_mission = 0;
            obelisk_menu.Hide();
            barracks_menu.missionStarted = false;
        }

        if(barracks_menu.isActive)
        {
            barracks_menu.Update(window,fps,inputCtrl);
        }
        else if(altar_menu.isActive)
        {
            altar_menu.Update(window,fps, inputCtrl);
        }
        else if(obelisk_menu.isActive)
        {
            obelisk_menu.Update(window,fps,inputCtrl);
        }
        else
        {
        t_title.setOrigin(t_title.getLocalBounds().width/2,t_title.getLocalBounds().height/2);
        t_title.setPosition(640,80);
        t_title.draw(window);
        }

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

        lastView = window.getView();
        window.setView(window.getDefaultView());

        if((!barracks_menu.isActive) && (!altar_menu.isActive) && (!obelisk_menu.isActive))
        {
            ctrlTips.x = 0;
            ctrlTips.y = (720-ctrlTips.ySize);
            ctrlTips.draw(window);
        }

        if(fade_alpha > 0)
        {
            fade_alpha -= float(500) / fps;
        }

        if(fade_alpha <= 0)
        {
            fade_alpha = 0;
        }

        fade_box.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        fade_box.setFillColor(sf::Color(0,0,0,fade_alpha));
        window.draw(fade_box);

        window.setView(lastView);

        if(dialogboxes.size() <= 0)
        {
            if((inputCtrl.isKeyPressed(InputController::Keys::LEFT)) || (inputCtrl.isKeyPressed(InputController::Keys::LTRIGGER)))
            {
                if(location > 0)
                {
                    location--;
                    left = true;

                    SetTitle(location);
                    thisConfig->thisCore->SaveToDebugLog("Changing Patapolis location to "+to_string(location));
                }
            }
            else if((inputCtrl.isKeyPressed(InputController::Keys::RIGHT)) || (inputCtrl.isKeyPressed(InputController::Keys::RTRIGGER)))
            {
                if(location < locations.size()-1)
                {
                    location++;
                    left = false;

                    SetTitle(location);
                    thisConfig->thisCore->SaveToDebugLog("Changing Patapolis location to "+to_string(location));
                }
            }
            else if(inputCtrl.isKeyPressed(InputController::Keys::CROSS))
            {
                // select the current menu item
                switch (location)
                {
                case 0:
                    /// trader/random
                    // open the world map
                    break;
                case 2:
                    /// armory/barracks
                    thisConfig->thisCore->SaveToDebugLog("Entering Barracks...");
                    barracks_menu.Show();
                    barracks_menu.isActive = true;
                    barracks_menu.obelisk = false;
                    barracks_menu.OpenBarracksMenu();
                    barracks_menu.UpdateInputControls();
                    thisConfig->thisCore->SaveToDebugLog("Barracks entered.");
                    break;
                case 3:
                    /// festival
                    // open barracks screen
                    break;
                case 4:
                    /// altar
                    // open mater menu
                    thisConfig->thisCore->SaveToDebugLog("Entering Altar...");
                    altar_menu.Show();
                    altar_menu.isActive = true;
                    altar_menu.ShowAltar();
                    thisConfig->thisCore->SaveToDebugLog("Altar entered.");
                    break;
                case 5:
                    /// obelisk
                    thisConfig->thisCore->SaveToDebugLog("Entering Obelisk...");
                    obelisk_menu.Reload();
                    obelisk_menu.Show();
                    obelisk_menu.isActive = true;
                    thisConfig->thisCore->SaveToDebugLog("Obelisk entered.");
                    break;
                default:
                    /// nothing
                    break;
                }
            }
            else if(inputCtrl.isKeyPressed(InputController::Keys::START))
            {
                std::vector<std::string> a = {Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"nav_yes")),Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"nav_no"))};

                PataDialogBox db;
                db.Create(f_font, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_returntomain")), a, thisConfig->GetInt("textureQuality"));
                db.id = 0;
                dialogboxes.push_back(db);
            }
            else if(inputCtrl.isKeyPressed(InputController::Keys::SELECT))
            {
                std::vector<std::string> a = {Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"nav_yes")),Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"nav_no"))};

                PataDialogBox db;
                db.Create(f_font, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_save")), a, thisConfig->GetInt("textureQuality"));
                db.id = 2;
                dialogboxes.push_back(db);
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
                            cout << "Open second dialogbox" << endl;
                            dialogboxes[dialogboxes.size()-1].Close();

                            std::vector<std::string> a = {Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"nav_yes")),Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"nav_no"))};

                            PataDialogBox db;
                            db.Create(f_font, Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_returntomainsave")), a, thisConfig->GetInt("textureQuality"));
                            db.id = 1;
                            dialogboxes.push_back(db);

                            break;
                        }
                        else if(dialogboxes[dialogboxes.size()-1].id == 1)
                        {
                            thisConfig->thisCore->SaveToDebugLog("Left from Patapolis to Title screen.");
                            this->Hide();
                            this->isActive = false;
                            parentMenu->Show();
                            parentMenu->isActive=true;
                        }
                        else if(dialogboxes[dialogboxes.size()-1].id == 2)
                        {
                            cout << "Saving game should happen here. Game not saving yet." << endl;
                            dialogboxes[dialogboxes.size()-1].Close();
                        }
                    }

                    case 1:
                    {
                        cout << "Back to Patapolis" << endl;
                        dialogboxes[dialogboxes.size()-1].Close();

                        break;
                    }
                }
            }
        }
    }
    else
    {
        fade_alpha = 255;
    }
}

void PatapolisMenu::UpdateButtons()
{
    /// this should update the text on all the buttons
}
void PatapolisMenu::OnExit()
{
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
PatapolisMenu::~PatapolisMenu()
{
    //dtor
}
