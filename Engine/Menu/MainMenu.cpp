#include "MainMenu.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
MainMenu::MainMenu()
{
    //ctor
    isActive=true;
}
void MainMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent)
{
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    config = thisConfigs;
    int q = thisConfigs->GetInt("textureQuality");

    float ratioX, ratioY;
    switch(q)
    {
        case 0: ///low
        {
            ratioX = thisConfigs->GetInt("resX") / float(640);
            ratioY = thisConfigs->GetInt("resY") / float(360);

            maxQX = float(640);
            maxQY = float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = thisConfigs->GetInt("resX") / float(1280);
            ratioY = thisConfigs->GetInt("resY") / float(720);

            maxQX = float(1280);
            maxQY = float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = thisConfigs->GetInt("resX") / float(1920);
            ratioY = thisConfigs->GetInt("resY") / float(1080);

            maxQX = float(1920);
            maxQY = float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = thisConfigs->GetInt("resX") / float(3840);
            ratioY = thisConfigs->GetInt("resY") / float(2160);

            maxQX = float(3840);
            maxQY = float(2160);
            break;
        }
    }

    grass_1.loadFromFile("resources/graphics/ui/menu/grass_1.png", q);
    grass_2.loadFromFile("resources/graphics/ui/menu/grass_2.png", q);
    grass_3.loadFromFile("resources/graphics/ui/menu/grass_3.png", q);
    grass_4.loadFromFile("resources/graphics/ui/menu/grass_4.png", q);

    grass_1.setScale(1.05,1.05);
    grass_2.setScale(1.05,1.05);
    grass_3.setScale(1.05,1.05);
    grass_4.setScale(1.05,1.05);

    grass_1.setOrigin(grass_1.getGlobalBounds().width/float(100),grass_1.getGlobalBounds().height);
    grass_2.setOrigin(grass_2.getGlobalBounds().width/float(100),grass_2.getGlobalBounds().height);
    grass_3.setOrigin(grass_3.getGlobalBounds().width/float(100),grass_3.getGlobalBounds().height);
    grass_4.setOrigin(grass_4.getGlobalBounds().width/float(100),grass_4.getGlobalBounds().height);

    logo.loadFromFile("resources/graphics/ui/menu/logo.png", q);
    logo.setOrigin(logo.getGlobalBounds().width/2, logo.getGlobalBounds().height/2);

    logo_shadow.loadFromFile("resources/graphics/ui/menu/logo_shadow.png", q);
    logo_shadow.setOrigin(logo_shadow.getGlobalBounds().width/2, logo_shadow.getGlobalBounds().height/2);

    totem[0].loadFromFile("resources/graphics/ui/menu/totem_1.png", q);
    totem[1].loadFromFile("resources/graphics/ui/menu/totem_2.png", q);
    totem[2].loadFromFile("resources/graphics/ui/menu/totem_3.png", q);
    totem[3].loadFromFile("resources/graphics/ui/menu/totem_4.png", q);

    totem[0].setOrigin(0,totem[0].getGlobalBounds().height);
    totem[1].setOrigin(0,totem[1].getGlobalBounds().height);
    totem[2].setOrigin(0,totem[2].getGlobalBounds().height);
    totem[3].setOrigin(0,totem[3].getGlobalBounds().height);

    fire_1.loadFromFile("resources/graphics/ui/menu/fire_1.png", q);
    fire_2.loadFromFile("resources/graphics/ui/menu/fire_2.png", q);
    fire_3.loadFromFile("resources/graphics/ui/menu/fire_3.png", q);

    fire_1.setOrigin(fire_1.getGlobalBounds().width/2, fire_1.getGlobalBounds().height);
    fire_2.setOrigin(fire_2.getGlobalBounds().width/2, fire_2.getGlobalBounds().height);
    fire_3.setOrigin(fire_3.getGlobalBounds().width/2, fire_3.getGlobalBounds().height);

    aura.loadFromFile("resources/graphics/ui/menu/aura.png", q);
    aura.setOrigin(aura.getGlobalBounds().width/2, aura.getGlobalBounds().height/2);

    sword_1.loadFromFile("resources/graphics/ui/menu/sword.png", q);
    sword_2.loadFromFile("resources/graphics/ui/menu/sword.png", q);

    sword_1.setOrigin(sword_1.getGlobalBounds().width/2, sword_1.getGlobalBounds().height/2);
    sword_2.setOrigin(sword_2.getGlobalBounds().width/2, sword_2.getGlobalBounds().height/2);

    sword_1.setScale(1, 1);
    sword_2.setScale(-1, 1);

    for(int i=0; i<=3; i++)
    {
        t_option[i].setFont(f_font);
        t_option[i].setCharacterSize(24);
        t_option[i].setColor(sf::Color::White);
    }

    string vx_params = "0,135,38,23;80,135,38,23;680,205,107,132;-1,205,107,132";

    vector<string> v_vxparams = Func::Split(vx_params,';');
    std::vector<sf::Vector2f> vx_pos;
    std::vector<sf::Color> vx_color;

    for(int i=0; i<v_vxparams.size(); i++)
    {
        vector<string> tmp = Func::Split(v_vxparams[i],',');

        sf::Vector2f tmp_vector;
        sf::Color tmp_color;

        tmp_vector.x = 0;
        tmp_vector.y = ((atof(tmp[0].c_str()) / 720) * maxQY) * ratioY;

        if(tmp[0] == "-1")
        {
            tmp_vector.y = maxQY * ratioY;
        }

        tmp_color.r = atoi(tmp[1].c_str());
        tmp_color.g = atoi(tmp[2].c_str());
        tmp_color.b = atoi(tmp[3].c_str());

        sf::Vector2f tmp_vector2;

        tmp_vector2.x = maxQX * ratioX;
        tmp_vector2.y = ((atof(tmp[0].c_str()) / 720) * maxQY) * ratioY;

        if(tmp[0] == "-1")
        {
            tmp_vector2.y = maxQY * ratioY;
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
    }

    g_x[0] = 0;
    g_x[1] = 0;
    g_x[2] = 0;
    g_x[3] = 0;

    Scene::Initialise(thisConfigs,keymap,parent);
    keyMapping=keymap;
    //
}
void MainMenu::EventFired(sf::Event event){
    if (patapolisMenu.isActive)
    {
        patapolisMenu.EventFired(event);
    }
    else if (v4core->currentController.isInitialized)
    {
        if(event.type == sf::Event::KeyPressed)
        {

        }
    }
    else if(isActive)
    {
        if(event.type == sf::Event::KeyPressed)
        {
            UsingMouseSelection=false;
            if (event.key.code == config->GetInt("keybindPata") || event.key.code == config->GetInt("keybindChaka") || event.key.code == config->GetInt("secondaryKeybindPata") || event.key.code == config->GetInt("secondaryKeybindChaka")){
                totem_sel-=1;
                if (totem_sel<0)
                    totem_sel=3;
                old_sel = totem_sel;
            }
            if (event.key.code == config->GetInt("keybindPon") || event.key.code == config->GetInt("keybindDon") || event.key.code == config->GetInt("secondaryKeybindPon") || event.key.code == config->GetInt("secondaryKeybindDon")){
                totem_sel+=1;
                if (totem_sel>3)
                    totem_sel=0;
                old_sel = totem_sel;
            }
            if (event.key.code == config->GetInt("keybindMenuEnter"))
            {
                SelectMenuOption();
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
        if (event.mouseButton.button == sf::Mouse::Left){
                SelectMenuOption();
            }
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            mouseX = event.mouseMove.x;
            mouseY = event.mouseMove.y;
            UsingMouseSelection=true;
        }
    }
}
void MainMenu::SelectMenuOption()
{
    switch (totem_sel){
    case 0:
        // load the start game cutscenes and menu
        {

        Hide();
        sf::Thread loadingThreadInstance(v4core->LoadingThread,v4core);
        v4core->continueLoading=true;
        v4core->window.setActive(false);
        loadingThreadInstance.launch();

        nameEntryMenu.Show();
        nameEntryMenu.isActive = true;
        nameEntryMenu.Initialise(config,keyMapping,v4core,this);

        v4core->continueLoading=false;
        }
        break;
    case 1:
        // load save and patapolis
        {

        Hide();
        sf::Thread loadingThreadInstance(v4core->LoadingThread,v4core);
        v4core->continueLoading=true;
        v4core->window.setActive(false);
        loadingThreadInstance.launch();

        patapolisMenu.Show();
        patapolisMenu.isActive = true;
        patapolisMenu.Initialise(config,keyMapping,v4core,this);

        v4core->continueLoading=false;
        }
        break;
    case 2:
        // load the options menu
        Hide();
        //optionsMenu->Show();
        break;
    case 3:
        // quit the game probably
        v4core->closeWindow=true;
        break;
    default:
        cout<<"WTF happened? you probably added more menu buttons but messed it up"<<endl;
        break;
    }
}
void MainMenu::Update(sf::RenderWindow &window, float fps, std::map<int,bool> *keyMap, std::map<int,bool> *keyMapHeld)
{
    if (v4core->currentController.isInitialized){
            v4core->currentController.Update(window, fps, keyMap,keyMapHeld);
    } else if(isActive){

        window.draw(v_background);

        if(fade == 0)
        alpha -= float(15)/fps;

        if(fade == 1)
        alpha += float(15)/fps;

        if(alpha <= 220)
        fade = 1;

        if(alpha >= 250)
        fade = 0;

        float scale = 1 + ((alpha - 220) / 500);
        float aurascale = 1 + ((alpha - 220)) / 250;

        g_dest[0] = ((mouseX/320.f - 2.f) * (-1) / 1280) * maxQX;
        g_dest[1] = ((mouseX/106.f - 6.f) * (-1) / 1280) * maxQX;
        g_dest[2] = ((mouseX/45.f - 14.f) * (-1) / 1280) * maxQX;
        g_dest[3] = ((mouseX/21.f - 30.f) * (-1) / 1280) * maxQX;

        for(int i=0; i<=3; i++)
        {
            if(g_dest[i] < g_x[i])
            g_x[i] -= abs(g_dest[i] - g_x[i]) / 100;
            if(g_dest[i] > g_x[i])
            g_x[i] += abs(g_dest[i] - g_x[i]) / 100;
        }

        grass_1.setPosition(g_x[0], maxQY-((float(100) / 720) * maxQY));
        grass_2.setPosition(g_x[1], maxQY-((float(30) / 720) * maxQY));
        grass_3.setPosition(g_x[2], maxQY+((float(10) / 720) * maxQY));
        grass_4.setPosition(g_x[3], maxQY+((float(4) / 720) * maxQY));

        grass_1.draw(window);
        grass_2.draw(window);
        grass_3.draw(window);

        logo_shadow.setPosition(maxQX/2,((float(140) / 720) * maxQY));
        logo.setPosition(maxQX/2,((float(140) / 720) * maxQY));

        logo_shadow.setColor(sf::Color(255,255,255,alpha));
        logo_shadow.setScale(scale,scale);

        logo_shadow.draw(window);
        logo.draw(window);

        float fire_shift = 0;

        for(int i=0; i<=3; i++)
        {
            totem[i].setPosition((((float(120) + float(306) * i) / 1280) * maxQX) + g_x[3]/1.4, maxQY);
            if (UsingMouseSelection)
            {
                if((mouseX / float(1280) * maxQX) - g_x[3] > totem[i].getPosition().x)
                {
                    if((mouseX / float(1280) * maxQX) - g_x[3] < totem[i].getPosition().x + ((totem[totem_sel].getGlobalBounds().width / 1280) * maxQX))
                    {
                        totem_sel = i;
                    }
                }
            }
        }

        switch(totem_sel)
        {
            case 0:
            {
                fire_shift = float(34) / float(720) * maxQY;
                break;
            }
            case 1:
            {
                fire_shift = float(40) / float(720) * maxQY;
                break;
            }
            case 2:
            {
                fire_shift = float(6) / float(720) * maxQY;
                break;
            }
            case 3:
            {
                fire_shift = float(10) / float(720) * maxQY;
                break;
            }
        }

        fire_1.setPosition(totem[totem_sel].getPosition().x + ((totem[totem_sel].getGlobalBounds().width/2) / 1280) * maxQX, maxQY - totem[totem_sel].getGlobalBounds().height / 720 * maxQY + fire_shift);
        fire_2.setPosition(totem[totem_sel].getPosition().x + ((totem[totem_sel].getGlobalBounds().width/2) / 1280) * maxQX, maxQY - totem[totem_sel].getGlobalBounds().height / 720 * maxQY + fire_shift);
        fire_3.setPosition(totem[totem_sel].getPosition().x + ((totem[totem_sel].getGlobalBounds().width/2) / 1280) * maxQX, maxQY - totem[totem_sel].getGlobalBounds().height / 720 * maxQY + fire_shift);

        //cout << int(floor(fire)) << " " << totem[totem_sel].getGlobalBounds().width << " " << totem[totem_sel].getGlobalBounds().height << " vs " << fire_1.getPosition().x << " " << fire_1.getPosition().y << endl;

        switch(int(floor(fire)))
        {
            case 0:
            fire_1.draw(window);
            break;

            case 1:
            fire_2.draw(window);
            break;

            case 2:
            fire_3.draw(window);
            break;
        }

        fire += float(14) / fps;
        if(fire >= 3)
        fire = 0;

        aura.setPosition(fire_1.getPosition().x, fire_1.getPosition().y - (fire_1.getGlobalBounds().height/2) / 720 * maxQY);
        aura.setScale(aurascale,aurascale);
        aura.draw(window);

        for(int i=0; i<=3; i++)
        totem[i].draw(window);

        grass_4.draw(window);

        if(old_sel != totem_sel)
        {
            cout << "Totem changed to " << totem_sel << endl;
            t_option[totem_sel].setScale(1.2,1.2);
        }

        for(int i=0; i<=3; i++)
        {
            if(t_option[i].getScale().x > 1)
            {
                float new_scale = t_option[i].getScale().x - float(1)/fps;
                t_option[i].setScale(new_scale, new_scale);
            }
            else
            {
                t_option[i].setScale(1,1);
            }

            t_option[i].setString(temp_menu[i]);
            t_option[i].setOrigin(t_option[i].getGlobalBounds().width/2, t_option[i].getGlobalBounds().height/2);

            if(i == totem_sel)
            t_option[i].setPosition((totem[i].getPosition().x + ((totem[i].getGlobalBounds().width/2) / 1280) * maxQX) / maxQX * window.getSize().x, ((maxQY - totem[i].getGlobalBounds().height / 720 * maxQY) - ((fire_1.getGlobalBounds().height + 35) / 720) * maxQY) / maxQY * window.getSize().y);
            else
            t_option[i].setPosition((totem[i].getPosition().x + ((totem[i].getGlobalBounds().width/2) / 1280) * maxQX) / maxQX * window.getSize().x, ((maxQY - totem[i].getGlobalBounds().height / 720 * maxQY) - ((fire_1.getGlobalBounds().height/2) / 720) * maxQY) / maxQY * window.getSize().y);

            t_option[i].setColor(sf::Color(255,255,255,96));
            t_option[totem_sel].setColor(sf::Color::White);

            window.draw(t_option[i]);
        }

        sword_1.setPosition(fire_1.getPosition().x - ((fire_1.getGlobalBounds().width/2 + t_option[totem_sel].getGlobalBounds().width/2 + sword_1.getGlobalBounds().width/2) / 1280) * maxQX, (maxQY - totem[totem_sel].getGlobalBounds().height / 720 * maxQY) - ((fire_1.getGlobalBounds().height + 30) / 720) * maxQY);
        sword_2.setPosition(fire_1.getPosition().x + ((fire_1.getGlobalBounds().width/2 + t_option[totem_sel].getGlobalBounds().width/2 + sword_2.getGlobalBounds().width/2) / 1280) * maxQX, (maxQY - totem[totem_sel].getGlobalBounds().height / 720 * maxQY) - ((fire_1.getGlobalBounds().height + 30) / 720) * maxQY);

        sword_1.draw(window);
        sword_2.draw(window);

        old_sel = totem_sel;

        window.setView(window.getDefaultView());
    } else {
        if (patapolisMenu.isActive){
            patapolisMenu.Update(window,fps);
        } else if (nameEntryMenu.isActive){
            nameEntryMenu.Update(window,fps);
        } else {
        }
    }

}
void MainMenu::UpdateButtons(){
    /// this should update the text on all the buttons
}
void MainMenu::OnExit(){
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
MainMenu::~MainMenu()
{
    //dtor
}
