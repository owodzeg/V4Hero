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
    t_title.setFont(f_font);

    t_title.setCharacterSize(38);
    t_title.setFillColor(sf::Color::White);

    mm_bigBox.setSize(sf::Vector2f(100,10));
    mm_bigBox.setFillColor(sf::Color(4,0,90));

    mm_titleBox.setSize(sf::Vector2f(100,10));
    mm_titleBox.setFillColor(sf::Color::Red);


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

    sparkles.push_back(tmp);
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

void PatapolisMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent, Menu *curParentMenu)
{
    sf::Context context;
    Scene::Initialise(thisConfigs,keymap,parent);
    altar_menu.Initialise(thisConfigs,keymap,parent,this);
    barracks_menu.Initialise(thisConfigs,keymap,parent,this);
    obelisk_menu.Initialise(thisConfigs,keymap,parent,this);
    parentMenu = curParentMenu;
    quality = thisConfig->GetInt("textureQuality");
    float ratioX, ratioY;
    switch(quality)
    {
        case 0: ///low
        {
            ratioX = thisConfig->GetInt("resX") / float(640);
            ratioY = thisConfig->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = thisConfig->GetInt("resX") / float(1280);
            ratioY = thisConfig->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = thisConfig->GetInt("resX") / float(1920);
            ratioY = thisConfig->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = thisConfig->GetInt("resX") / float(3840);
            ratioY = thisConfig->GetInt("resY") / float(2160);
            break;
        }
    }

    float resRatioX = thisConfig->GetInt("resX") / float(1280);
    float resRatioY = thisConfig->GetInt("resY") / float(720);

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

    initialised=true;

    if (doWaitKeyPress)
    {
        v4core->LoadingWaitForKeyPress();
        v4core->ChangeRichPresence("In Patapolis", "logo", "");
    }
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
            // do something here;
            /*if (event.key.code == sf::Keyboard::Num1)
            {

                animStartVal=p_background[0].x;
                animChangeVal=6000-p_background[0].x;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 0;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == sf::Keyboard::Num2)
            {

                animStartVal=p_background[0].x;
                animChangeVal=4000-p_background[0].x;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 1;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == sf::Keyboard::Num3)
            {

                animStartVal=p_background[0].x;
                animChangeVal=2880-p_background[0].x;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 2;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == sf::Keyboard::Num4)
            {

                animStartVal=p_background[0].x;
                animChangeVal=730-p_background[0].x;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 3;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == sf::Keyboard::Num5)
            {

                animStartVal=p_background[0].x;
                animChangeVal=-p_background[0].x-350;
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                currentMenuPosition = 4;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == thisConfig->GetInt("keybindPon") || event.key.code == thisConfig->GetInt("secondaryKeybindPon"))
            {
                currentMenuPosition += 1;
                if (currentMenuPosition>possibleMenuPositions.size()-1)
                {
                    currentMenuPosition=0;
                }
                animStartVal=p_background[0].x;
                animChangeVal=-p_background[0].x-possibleMenuPositions[currentMenuPosition];
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                SetTitle(currentMenuPosition);


            }
            else if (event.key.code == thisConfig->GetInt("keybindPata") || event.key.code == thisConfig->GetInt("secondaryKeybindPata"))
            {
                currentMenuPosition -= 1;
                if (currentMenuPosition<0)
                {
                    currentMenuPosition=4;
                }
                animStartVal=p_background[0].x;
                animChangeVal=-p_background[0].x-possibleMenuPositions[currentMenuPosition];
                anim_timer.restart();
                totalTime=1;
                isAnim = true;
                SetTitle(currentMenuPosition);

            }
            else if (event.key.code == thisConfig->GetInt("keybindDon") || event.key.code == thisConfig->GetInt("secondaryKeybindDon") || event.key.code == thisConfig->GetInt("keybindMenuEnter"))
            {
                // select the current menu item
                switch (currentMenuPosition)
                {
                case 0:
                    /// trader/random
                    // open the world map
                    break;
                case 1:
                    /// armoury/barracks
                    barracks_menu.Show();
                    barracks_menu.isActive = true;
                    barracks_menu.OpenBarracksMenu();
                    break;
                case 2:
                    /// festival
                    // open barracks screen
                    break;
                case 3:
                    /// altar
                    // open mater menu
                    altar_menu.Show();
                    altar_menu.isActive = true;
                    altar_menu.ShowAltar();
                    break;
                case 4:
                    /// obelisk
                    // idk?
                    // open inventory menu
                    obelisk_menu.Show();
                    obelisk_menu.isActive = true;
                    break;
                default:
                    /// nothing

                    break;
                }

            }
            else if (event.key.code == thisConfig->GetInt("keybindBack"))
            {
                this->Hide();
                this->isActive = false;
                parentMenu->Show();
                parentMenu->isActive=true;
            }
            */

        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        }
    }

}
void PatapolisMenu::SetTitle(int menuPosition)
{
    switch(menuPosition)
    {
    case 0:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_trader")));
        break;
    case 1:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_barracks")));
        break;
    case 2:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_festival")));
        break;
    case 3:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_altar")));
        break;
    case 4:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis_obelisk")));
        break;
    default:
        t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"patapolis")));
        break;
    }
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);
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
void PatapolisMenu::Update(sf::RenderWindow &window, float fps)
{
    if(isActive)
    {
        window.draw(v_background);

        for(int i=0; i<layer_6.size(); i++)
        {
            //cout << "layer_6[" << i << "]: " << layer_6[i].getGlobalBounds().width/2 << " " << layer_6[i].getGlobalBounds().height << " " << layer_6[i].getPosition().x << " " << layer_6[i].getPosition().y << endl;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                layer_6[i].lx += float(8.75) / fps * float(240);
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                layer_6[i].lx -= float(8.75) / fps * float(240);
            }

            layer_6[i].setOrigin(layer_6[i].getLocalBounds().width/2, layer_6[i].getLocalBounds().height);
            layer_6[i].draw(window);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            L5.lx += float(9) / fps * float(240);
            L4.lx += float(9.5) / fps * float(240);
            wakapon.lx += float(10) / fps * float(240);
            world_egg.lx += float(10) / fps * float(240);
            light_1.lx += float(10) / fps * float(240);
            light_2.lx += float(10) / fps * float(240);
            egg_light.lx += float(10) / fps * float(240);
            rayX += float(10) / fps * float(240);

            floor_x += float(10) / fps * float(240);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            L5.lx -= float(9) / fps * float(240);
            L4.lx -= float(9.5) / fps * float(240);
            wakapon.lx -= float(10) / fps * float(240);
            world_egg.lx -= float(10) / fps * float(240);
            light_1.lx -= float(10) / fps * float(240);
            light_2.lx -= float(10) / fps * float(240);
            egg_light.lx -= float(10) / fps * float(240);
            rayX -= float(10) / fps * float(240);

            floor_x -= float(10) / fps * float(240);
        }

        L5.setOrigin(L5.getLocalBounds().width/2, L5.getLocalBounds().height);
        L4.setOrigin(L4.getLocalBounds().width/2, L4.getLocalBounds().height);

        L5.draw(window);
        L4.draw(window);


        for(int i=0; i<layer_2.size(); i++)
        {
            //cout << "layer_6[" << i << "]: " << layer_6[i].getGlobalBounds().width/2 << " " << layer_6[i].getGlobalBounds().height << " " << layer_6[i].getPosition().x << " " << layer_6[i].getPosition().y << endl;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                layer_2[i].lx += float(10) / fps * float(240);
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                layer_2[i].lx -= float(10) / fps * float(240);
            }

            layer_2[i].setOrigin(layer_2[i].getLocalBounds().width/2, layer_2[i].getLocalBounds().height);
            layer_2[i].draw(window);
        }

        float resRatioX = window.getSize().x / float(1280);
        float resRatioY = window.getSize().y / float(720);

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
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                sparkles[i].x += float(10) / fps * float(240);
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                sparkles[i].x -= float(10) / fps * float(240);
            }

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

                lightrays[i].triangle[0].position = sf::Vector2f(x1, y1);
                lightrays[i].triangle[1].position = sf::Vector2f(x2, y2);
                lightrays[i].triangle[2].position = sf::Vector2f(((x1 + x2) / 2) + (lightrays[i].cur_distance * sin(lightrays[i].angle*3.14159265/180)), ((y1 + y2) / 2) + (lightrays[i].cur_distance * cos(lightrays[i].angle*3.14159265/180)));

                //cout << "[coord #" << i << "]" << lightrays[i].triangle[0].position.x << " " << lightrays[i].triangle[0].position.y << " " << lightrays[i].triangle[1].position.x << " " << lightrays[i].triangle[1].position.y << " " << lightrays[i].triangle[2].position.x << " " << lightrays[i].triangle[2].position.y << endl;

                window.draw(lightrays[i].triangle);
            }
        }

        bridge.setOrigin(0, bridge.getLocalBounds().height);
        bridge.setPosition(floor_x + 11200, 720);
        bridge.draw(window);

        wakapon.draw(window);
        world_egg.draw(window);

        /**window.setView(window.getDefaultView());
        mm_bigBox.setSize(sf::Vector2f(window.getSize().x,window.getSize().y-200));
        //mm_smallerBox.setSize(sf::Vector2f(100,10));
        //mm_titleBox.setSize(sf::Vector2f(100,10));

        mm_bigBox.setPosition(0,85);
        //mm_smallerBox.setPosition(100,10);
        //mm_titleBox.setPosition(100,10);


        window.draw(mm_bigBox);
        //window.draw(mm_smallerBox);
        //window.draw(mm_titleBox);




        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        auto lastView = window.getView();
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos,lastView);

        //t_pressToContinue.setPosition(window.getSize().x/2,300);
        //window.draw(t_pressToContinue);


        window.setView(window.getDefaultView());
        if(anim_timer.getElapsedTime().asSeconds() >= totalTime)
        {
            isAnim = false;
        }
        if (isAnim)
        {
            p_background[0].x = EaseIn(anim_timer.getElapsedTime().asSeconds(),animStartVal,animChangeVal,totalTime);
        }
        for(int i=0; i<s_background.size(); i++)
        {
            //s_background[i].setTexture(t_background[i]);

            s_background[i].setPosition(p_background[i].x,p_background[i].y);
            //cout << s_background[i].y << endl;
            s_background[i].draw(window);
        }
        if (barracks_menu.isActive)
        {
            barracks_menu.Update(window,fps);
        }
        else if(altar_menu.isActive)
        {
            altar_menu.Update(window,fps);
        }
        else if(obelisk_menu.isActive)
        {
            obelisk_menu.Update(window,fps);
        }
        else
        {
            t_title.setPosition(window.getSize().x/2,110);
            window.draw(t_title);
        }*/
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
