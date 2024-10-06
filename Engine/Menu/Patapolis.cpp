#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Patapolis.h"
#include "Altar.h"

#include <sstream>
#include <spdlog/spdlog.h>

#include "../CoreManager.h"
#include "../StateManager.h"
#include "../ResourceManager.h"

PatapolisMenu::PatapolisMenu()
{
    //TO-DO: is this still needed?
    is_active = false;

    SPDLOG_INFO("Initializing Patapolis");

    try
    {
        Config* config = CoreManager::getInstance().getConfig();
        StringRepository* strRepo = CoreManager::getInstance().getStrRepo();

        std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

        updateStoryPoint(); // Update story_point before anything else

        quality = config->GetInt("textureQuality");

        float resRatioX = config->GetInt("resX") / float(3840);
        float resRatioY = config->GetInt("resY") / float(2160);

        string vx_params = "0,24,128,238;198,24,128,238;1332,184,243,202;1773,184,243,202;1774,255,255,255;2130,171,243,214;2160,171,243,214";

        vector<string> v_vxparams = Func::Split(vx_params, ';');
        std::vector<sf::Vector2f> vx_pos;
        std::vector<sf::Color> vx_color;

        for (int i = 0; i < v_vxparams.size(); i++)
        {
            vector<string> tmp = Func::Split(v_vxparams[i], ',');

            sf::Vector2f tmp_vector;
            sf::Color tmp_color;

            tmp_vector.x = 0;
            tmp_vector.y = atof(tmp[0].c_str()) * resRatioY;

            if (tmp[0] == "-1")
            {
                tmp_vector.y = 2160 * resRatioY;
            }

            tmp_color.r = atoi(tmp[1].c_str());
            tmp_color.g = atoi(tmp[2].c_str());
            tmp_color.b = atoi(tmp[3].c_str());

            sf::Vector2f tmp_vector2;

            tmp_vector2.x = 3840 * resRatioX;
            tmp_vector2.y = atof(tmp[0].c_str()) * resRatioY;

            if (tmp[0] == "-1")
            {
                tmp_vector2.y = 2160 * resRatioY;
            }

            vx_pos.push_back(tmp_vector);
            vx_color.push_back(tmp_color);

            vx_pos.push_back(tmp_vector2);
            vx_color.push_back(tmp_color);
        }

        sf::VertexArray tmp(sf::TrianglesStrip, vx_pos.size());
        v_background = tmp;

        for (int i = 0; i < vx_pos.size(); i++)
        {
            v_background[i].position = vx_pos[i];
            v_background[i].color = vx_color[i];

            SPDLOG_TRACE("patapolis background vx_pos: {} {}", vx_pos[i].x, vx_pos[i].y);
        }

        floor_height = 54*3;
        r_ground.setSize(sf::Vector2f(34500 * resRatioX, floor_height * resRatioY));
        r_ground.setFillColor(sf::Color::Black);

        std::vector<string> l2_str = {"a", "b", "c", "c_winter1", "c_winter2", "d", "e", "f"};
        std::vector<string> l6_str = {"a", "b", "c", "d", "e"};

        for (int i = 0; i < l2_str.size(); i++)
        {
            ps_layer_2[l2_str[i]] = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/2" + l2_str[i] + ".png");
        }

        for (int i = 0; i < l6_str.size(); i++)
        {
            ps_layer_6[l6_str[i]] = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/6" + l6_str[i] + ".png");
        }


        addL6("a", 0, 722*3 - floor_height);
        addL6("a", 2700*3, 722*3 - floor_height);

        addL6("d", 3840*3, 726*3 - floor_height);
        addL6("c", 3800*3, 722*3 - floor_height);

        addL6("d", 4400*3, 740*3 - floor_height);
        addL6("c", 4650*3, 740*3 - floor_height);
        addL6("b", 4900*3, 726*3 - floor_height);
        addL6("d", 5100*3, 737*3 - floor_height);
        addL6("c", 5050*3, 725*3 - floor_height);
        addL6("d", 5500*3, 745*3 - floor_height);
        addL6("c", 5700*3, 723*3 - floor_height);
        addL6("d", 5760*3, 724*3 - floor_height);
        addL6("a", 5850*3, 722*3 - floor_height);
        addL6("c", 6100*3, 722*3 - floor_height);
        addL6("d", 6150*3, 733*3 - floor_height);
        addL6("c", 6400*3, 755*3 - floor_height);
        addL6("b", 6650*3, 722*3 - floor_height);

        addL6("e", 7640*3, 722*3 - floor_height);
        addL6("d", 7700*3, 731*3 - floor_height);
        addL6("c", 7670*3, 724*3 - floor_height);
        addL6("c", 7900*3, 750*3 - floor_height);

        addL6("d", 8250*3, 727*3 - floor_height);
        addL6("c", 8480*3, 742*3 - floor_height);
        addL6("d", 8710*3, 734*3 - floor_height);
        addL6("c", 8880*3, 723*3 - floor_height);
        addL6("d", 8930*3, 733*3 - floor_height);
        addL6("c", 9040*3, 757*3 - floor_height);
        addL6("d", 9180*3, 736*3 - floor_height);
        addL6("a", 9120*3, 722*3 - floor_height);
        addL6("c", 9260*3, 743*3 - floor_height);

        addL6("c", 9700*3, 728*3 - floor_height);
        addL6("d", 9740*3, 735*3 - floor_height);
        addL6("c", 9880*3, 760*3 - floor_height);
        addL6("d", 9930*3, 750*3 - floor_height);
        addL6("d", 10040*3, 737*3 - floor_height);

        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/5.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/4.png");

        addL2("a", 800*3, 766*3 - floor_height);
        addL2("b", 2900*3, 740*3 - floor_height);
        addL2("c", 3900*3, 720*3 - floor_height);
        addL2("c", 4300*3, 770*3 - floor_height);

        addL2("c", 5080*3, 750*3 - floor_height);
        addL2("c", 5220*3, 720*3 - floor_height);

        addL2("c", 5600*3, 770*3 - floor_height);
        addL2("c", 5740*3, 720*3 - floor_height);

        addL2("c", 6150*3, 760*3 - floor_height);
        addL2("c", 6320*3, 720*3 - floor_height);

        addL2("c", 6540*3, 760*3 - floor_height);

        addL2("c", 7540*3, 760*3 - floor_height);

        addL2("c", 7800*3, 720*3 - floor_height);
        addL2("c", 8000*3, 750*3 - floor_height);

        addL2("c", 8900*3, 770*3 - floor_height);
        addL2("c", 9060*3, 740*3 - floor_height);

        addL2("c", 9760*3, 740*3 - floor_height);

        addL2("c", 10300*3, 770*3 - floor_height);
        addL2("c", 10470*3, 740*3 - floor_height);

        addL2("c", 11000*3, 750*3 - floor_height);

        addL2("d", 300*3, 728*3 - floor_height);
        addL2("d", 940*3, 728*3 - floor_height);
        addL2("e", 1400*3, 728*3 - floor_height);
        addL2("f", 1800*3, 728*3 - floor_height);
        addL2("d", 4200*3, 728*3 - floor_height);
        addL2("d", 5000*3, 728*3 - floor_height);
        addL2("d", 5580*3, 728*3 - floor_height);
        addL2("d", 6400*3, 728*3 - floor_height);
        addL2("d", 7700*3, 728*3 - floor_height);
        addL2("d", 9000*3, 728*3 - floor_height);
        addL2("d", 10500*3, 728*3 - floor_height);

        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/edge.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/bridge.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/rainbow.png");

        for (int i = 0; i < 4; i++)
        {
            back_layer[i] = "resources/graphics/bg/patapolis/back_" + to_string(i + 1) + ".png";

            ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/back_" + to_string(i + 1) + ".png");
        }

        // to be replaced to store only data in vectors and draw the same sprite in draw call
        addSparkle(34860 + 25*3, 400*3);
        addSparkle(34860 + 170*3, 380*3);
        addSparkle(34860 + 83*3, 270*3);
        addSparkle(34860 + 180*3, 265*3);
        addSparkle(34860 + 283*3, 295*3);
        addSparkle(34860 + 242*3, 159*3);
        addSparkle(34860 + 333*3, 235*3);
        addSparkle(34860 + 455*3, 261*3);
        addSparkle(34860 + 416*3, 100*3);
        addSparkle(34860 + 598*3, 127*3);
        addSparkle(34860 + 506*3, 186*3);
        addSparkle(34860 + 605*3, 257*3);
        addSparkle(34860 + 722*3, 203*3);
        addSparkle(34860 + 711*3, 332*3);
        addSparkle(34860 + 831*3, 341*3);
        addSparkle(34860 + 937*3, 401*3);
        addSparkle(34860 + 860*3, 244*3);

        //TODO: to be replaced with respective .zip models
        a_wakapon.LoadConfig("resources/units/unit/wakapon.zip");
        a_wakapon.setAnimation("idle");
        a_wakapon.setGlobalPosition(sf::Vector2f(a_wakapon.getGlobalPosition().x, 395*3));

        a_sen.LoadConfig("resources/units/unit/ranpurupon.zip");
        a_sen.setAnimation("idle");
        a_sen.setGlobalPosition(sf::Vector2f(a_sen.getGlobalPosition().x, 629*3));


        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/egg.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/light.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/egg_light.png");

        addRay(10*3, 331*3, 18*3, 321*3);
        addRay(21*3, 321*3, 25*3, 316*3);
        addRay(28*3, 313*3, 32*3, 308*3);
        addRay(34*3, 306*3, 37*3, 300*3);
        addRay(39*3, 298*3, 41*3, 291*3);
        addRay(41*3, 278*3, 39*3, 271*3);
        addRay(36*3, 254*3, 33*3, 246*3);
        addRay(31*3, 242*3, 27*3, 235*3);
        addRay(24*3, 232*3, 19*3, 226*3);
        addRay(16*3, 223*3, 9*3, 217*3);
        addRay(7*3, 214*3, 0*3, 211*3);
        addRay(2*3, 210*3, -2*3, 210*3);

        for (int i = 11; i >= 0; i--)
        {
            float x1 = coords[i].ox2 * (-1);
            float x2 = coords[i].ox1 * (-1);

            addRay(x1, coords[i].y2, x2, coords[i].y1);
        }

        for (int i = 0; i < coords.size(); i++)
        {
            LightRay tmp;
            tmp.angle = 30 + (i * (float(300) / float(coords.size())));
            tmp.max_distance = 110*3 + rand() % 20*3;
            tmp.min_distance = 10*3 + rand() % 20*3;
            tmp.cur_distance = rand() % 110*3 + 20*3;


            sf::VertexArray vtmp(sf::TrianglesStrip, 3);
            vtmp[0].position = sf::Vector2f(coords[i].x1, coords[i].y1);
            vtmp[0].color = sf::Color(215, 246, 255, 255);
            vtmp[1].position = sf::Vector2f(coords[i].x2, coords[i].y2);
            vtmp[1].color = sf::Color(215, 246, 255, 255);
            vtmp[2].position = sf::Vector2f(((coords[i].x1 + coords[i].x2) / 2) + (tmp.cur_distance * sin(tmp.angle * 3.14159265 / 180)), ((coords[i].y1 + coords[i].y2) / 2) + (tmp.cur_distance * cos(tmp.angle * 3.14159265 / 180)));
            vtmp[2].color = sf::Color(215, 246, 255, 255);
            tmp.triangle = vtmp;

            SPDLOG_TRACE("[coord #{}] {} {} {} {} {} {} {} {} {} {}", i, vtmp[0].position.x, vtmp[0].position.y, vtmp[1].position.x, vtmp[1].position.y, vtmp[2].position.x, vtmp[2].position.y, tmp.angle, tmp.cur_distance, tmp.min_distance, tmp.max_distance);

            lightrays.push_back(tmp);
        }

        //preload patapolis locations
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/barracks.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/forge_main.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/blacksmith_forge.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/blacksmith_forge_glow.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/blacksmith_fence.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/slab.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/slab_glow.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/market.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/mater.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/festival_main.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/altar.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/obelisk.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/paraget_main.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/paraget_crystal_glow.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/paraget_crystal.png");

        locations.push_back(640*3 - 640*3); //Barracks
        locations.push_back(2300*3 - 640*3); //Blacksmith
        locations.push_back(3960*3 - 640*3); //Market
        locations.push_back(5620*3 - 640*3); //Festival
        locations.push_back(6450*3 - 640*3); ///Sen
        locations.push_back(7280*3 - 640*3); // Altar
        locations.push_back(8300*3 - 640*3); // Obelisk
        locations.push_back(9340*3 - 530*3); // Mater (needs to be offset differently because sprite is not centered)
        locations.push_back(10600*3 - 640*3); //Paraget
        locations.push_back(11290*3); // Wakapon
        locations.push_back(11569*3); // Egg

        int range = rand() % 30*3 + 50*3;
        for (int i = 0; i < range; i++)
        {
            /// to be replaced so it uses a single texture drawn many times only
            addParagetSparkle(10549*3 + (rand() % 100*3), 330*3 + (rand() % 200*3));
        }

        /// same as above
        addFire(4, 3903*3, 583*3, true); // market
        addFire(4, 4017*3, 583*3, true);

        addFire(2, 5816*3, 574*3, true); // festival square
        addFire(2, 5431*3, 574*3, true);
        addFire(1, 5207*3, 368*3, true);
        addFire(1, 6039*3, 368*3, true);

        addFire(0, 8135*3, 479*3, true); // we should probably switch these to use something like obelisk.position.x so it's easier to move things
        addFire(0, 8465*3, 479*3, true);

        forge_big = addFire(2, 2050*3, 542*3, false); // blacksmith
        forge_purple = addFire(3, 2370*3, 404*3, false);

        addCloud("A", 9500*3, 140*3, 0, 0);
        addCloud("A", 8800*3, 240*3, 0, 0);
        addCloud("A", 8000*3, 170*3, 0, 0);

        // preload all the tiny effect png's beforehand
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/smoke.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/sparkle.png");

        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/1a.png");

        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/paraget_sparkle_1.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/paraget_sparkle_2.png");
        ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/paraget_sparkle_3.png");

        ctrlTips.create(54*3, font, 20, sf::String("L/R: Move      X: Interact      Select: Save      Start: Title screen"), quality);

        //TO-DO: these things are not yet supported by new system. add compatibility
        /*
        mater_menu.save_loaded = save_loaded; // TEMPORARY REMOVE DIS

        credits.Initialise(config, v4Core);
        */
        screenFade.Create(ScreenFade::FADEIN, 1024);

        float volume = (float(config->GetInt("masterVolume")) * (float(config->GetInt("bgmVolume")) / 100.f));

        sb_city_loop.loadFromFile("resources/sfx/bgm/patapolis.ogg");
        city_loop.setBuffer(sb_city_loop);
        city_loop.setLoop(true);
        city_loop.setVolume(volume);


        SetTitle(location);
        camPos = locations[location];

        // camera settings
        camera.debug_x_dest = 340;
        camera.debug_x = 340;
        camera.followobject_x = 0;
        camera.lockMovement = true;

        sf::RenderWindow* window = CoreManager::getInstance().getWindow();

        camera.zoom_point_x = window->getSize().x/2;
        camera.zoom_point_y = window->getSize().y;

        patapolisView = window->getDefaultView();
    }
    catch(AnimatedObjectException& exception)
    {
        SPDLOG_ERROR("Error when loading AnimationObject: {}", exception.what());
        throw PatapolisException(exception.what());
    }
    catch(std::exception& exception)
    {
        SPDLOG_ERROR("Generic exception happened. {}", exception.what());
        throw PatapolisException(exception.what());
    }
    catch( ... )
    {
        SPDLOG_ERROR("Unknown exception happened.");
        throw PatapolisException("Unknown exception happened.");
    }

    // what???
    initialised = true;
    initialized = true;
}

void PatapolisMenu::updateStoryPoint()
{
    //SPDLOG_TRACE("Updating story point");
    SaveReader* saveReader = CoreManager::getInstance().getSaveReader();

    // what the hell happened here?
    // story_point needs to be updated manually after mission passes,
    // it is not the amount of unlocked missions, it can be a variable number
    // that currently points to a specific branch of the story
    // TO-DO: make up my mind about this /owocek

    // While currently just doing
    saveReader->story_point = *std::max_element(saveReader->missions_unlocked.begin(), saveReader->missions_unlocked.end());
    // would suffice, we do this in case at some point we want a story point to change without unlocking a mission
    // for example, after a mission you're supposed to fail
    int last_mission = *std::max_element(saveReader->missions_unlocked.begin(), saveReader->missions_unlocked.end());
    if (last_mission < 6) // story_point 0-5 can just do that
    {
        saveReader->story_point = last_mission;
    }
}

void PatapolisMenu::addL6(std::string variant, float x, float y)
{
    SPDLOG_TRACE("Adding L6 layer: variant {} x {} y {}", variant, x, y);
    //PSprite tmp;
    //tmp.loadFromFile("resources/graphics/bg/patapolis/6" + variant + ".png", q, r);
    //tmp.setPosition(x, y);

    L tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.type = variant;
    layer_6.push_back(tmp);
}

void PatapolisMenu::addL2(std::string variant, float x, float y)
{
    SPDLOG_TRACE("Adding L2 layer: variant {} x {} y {}", variant, x, y);
    //PSprite tmp;
    //tmp.loadFromFile("resources/graphics/bg/patapolis/2" + variant + ".png", q, r);
    //tmp.setPosition(x, y);

    L tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.type = variant;
    layer_2.push_back(tmp);
}

void PatapolisMenu::addSparkle(float x, float y)
{
    SPDLOG_TRACE("Adding Sparkle: x {} y {}", x, y);

    float scale = ((rand() % 400) + 750) / float(1000);

    Sparkle tmp;

    tmp.initScale = scale;
    tmp.curScale = scale;
    tmp.x = x;
    tmp.y = y;
    tmp.baseX = x;

    sparkles.push_back(tmp);
}

void PatapolisMenu::addParagetSparkle(float x, float y)
{
    SPDLOG_TRACE("Adding Paraget Sparkle: x {} y {}", x, y);

    float scale = ((rand() % 400) + 750) / float(1000);
    float angle = (rand() % 36000) / float(100);

    ParagetSparkle tmp;

    int choice = rand() % 3 + 1;
    string nm = "paraget_sparkle_" + to_string(choice) + ".png";

    tmp.tx_name = nm;
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
    SPDLOG_TRACE("Adding Light Ray: x1 {} y1 {} x2 {} y2 {}", x1, y1, x2, y2);

    RayStart rs;
    rs.x1 = x1;
    rs.y1 = 15 + y1;
    rs.x2 = x2;
    rs.y2 = 15 + y2;

    rs.ox1 = x1;
    rs.ox2 = x2;

    coords.push_back(rs);
}

PatapolisMenu::Fire PatapolisMenu::addFire(int type, float x, float y, bool add)
{
    SPDLOG_TRACE("Adding Fire: type {} x {} y {} add {}", type, x, y, add);
    Fire tmp;

    string str_type = "";

    switch (type)
    {
        case 0: {
            str_type = "small";
            break;
        }

        case 1: {
            str_type = "med";
            break;
        }

        case 2: {
            str_type = "large";
            break;
        }

        case 3: {
            str_type = "purple";
            break;
        }

        case 4: {
            str_type = "market";
            break;
        }
    }

    //preload assets
    ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/" + str_type + "_fire1.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/" + str_type + "_fire2.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/" + str_type + "_fire3.png");
    ResourceManager::getInstance().loadSprite("resources/graphics/bg/patapolis/" + str_type + "_glow.png");

    tmp.fire[0] = "resources/graphics/bg/patapolis/" + str_type + "_fire1.png";
    tmp.fire[1] = "resources/graphics/bg/patapolis/" + str_type + "_fire2.png";
    tmp.fire[2] = "resources/graphics/bg/patapolis/" + str_type + "_fire3.png";

    tmp.glow = "resources/graphics/bg/patapolis/" + str_type + "_glow.png";

    tmp.baseX = x;
    tmp.baseY = y;
    tmp.x = x;
    tmp.y = y;

    if (add)
        fires.push_back(tmp);

    return tmp;
}

void PatapolisMenu::addSmokeParticle(float x, float y)
{
    SPDLOG_TRACE("Adding Smoke Particle: x {} y {}", x, y);

    SmokeParticle tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.baseX = x;
    tmp.baseY = y;
    tmp.curScale = 0.1;
    smoke.push_back(tmp);
}

void PatapolisMenu::addCloud(std::string type, float x, float y, float xsize, float ysize)
{
    SPDLOG_TRACE("Adding cloud type {} x {} y {} xsize {} ysize {}", type, x, y, xsize, ysize);

    if (type == "A")
    {
        CloudA cloud;
        cloud.x = x;
        cloud.y = y;

        clouds_A.push_back(cloud);
    } else if (type == "B")
    {
        float resRatio = float(CoreManager::getInstance().getConfig()->GetInt("resX")) / float(3840);

        CloudB cloud;
        cloud.cloud.setFillColor(sf::Color::White);
        cloud.cloud.setRadius(640*3 * resRatio);

        float scale_x = xsize / 640.f*3;
        float scale_y = ysize / 640.f*3;

        cloud.cloud.setScale(scale_x * resRatio, scale_y * resRatio);

        cloud.base_x = x;
        cloud.base_y = y;

        clouds_B.push_back(cloud);
    }
}

void PatapolisMenu::EventFired(sf::Event event)
{
    //TO-DO: fire events for barracks menu? probably through CoreManager
    //if (barracks_menu.is_active)
    //{
    //    barracks_menu.EventFired(event);
    //} else 
    //if (obelisk_menu.is_active)
    //{
    //    obelisk_menu.EventFired(event);
    //} else 
    //if (mater_menu.is_active)
    //{
    //    mater_menu.EventFired(event);
    //} else 
    if (is_active)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            

        } else if (event.type == sf::Event::MouseButtonReleased)
        {
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
        }
    }
}
void PatapolisMenu::SetTitle(int menuPosition)
{
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    Config* config = CoreManager::getInstance().getConfig();
    SaveReader* saveReader = CoreManager::getInstance().getSaveReader();

    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    sf::String a = "L/R: Move      ";

    if (draw_ID.size() <= 1)
    {
        draw_ID.push_back(menuPosition);
    } else
    {
        draw_ID.erase(draw_ID.begin());
        draw_ID.push_back(menuPosition);
    }

    std::string draw_ID_log = "";

    for (auto i : draw_ID)
        draw_ID_log += to_string(i);

    SPDLOG_TRACE("draw_ID: {}", draw_ID_log);

    switch (menuPosition)
    {
        case Buildings::MARKET:
            t_title.setStringKey("patapolis_trader");
            break;
        case Buildings::FORGE:
            t_title.setStringKey("patapolis_blacksmith");
            break;
        case Buildings::BARRACKS:
            t_title.setStringKey("patapolis_barracks");
            a += "X: Interact      ";
            break;
        case Buildings::FESTIVAL:
            t_title.setStringKey("patapolis_festival");
            break;
        case Buildings::SEN: {
            t_title.setStringKey("patapolis_sen");
            a += "X: Interact      ";

            ///activate her dialogue
            messageclouds.clear();

            MessageCloud tmp;
            tmp.Create(20, sf::Vector2f(a_sen.getGlobalPosition().x - 15, a_sen.getGlobalPosition().y - 75), sf::Color(170, 182, 250, 255), false, config->GetInt("textureQuality"));
            tmp.x_start_offset = 15;
            tmp.msgcloud_ID = 0;

            vector<int> missions = saveReader->missions_unlocked;

            switch (saveReader->story_point)
            {
                case 0:
                default: {
                    // Start of game, shouldn't be accessible
                    tmp.AddDialog("dialogue_error", true);
                    tmp.AdditionalData("story_point is either 0 or above currently regulated bounds");
                    break;
                }
                case 1: {
                    // Shida Valley
                    tmp.AddDialog("npc_sen_1", true);
                    tmp.AddDialog("npc_sen_4", true);
                    tmp.AddDialog("npc_sen_5", true);
                    break;
                }
                case 2: {
                    // Patapine Grove Unlocked
                    tmp.AddDialog("npc_sen_3", true);
                    tmp.AddDialog("npc_sen_6", true);
                    tmp.AddDialog("npc_sen_7", true);
                    break;
                }
                case 3: {
                    // Patapine Grove Beaten
                    tmp.AddDialog("npc_sen_2", true);
                    tmp.AddDialog("npc_sen_8", true);
                    tmp.AddDialog("npc_sen_9", true);
                    break;
                }
                case 4: {
                    // Ejiji Cliffs Unlocked
                    tmp.AddDialog("npc_sen_3", true);
                    tmp.AddDialog("npc_sen_10", true);
                    tmp.AddDialog("npc_sen_11", true);
                    break;
                }
                case 5: {
                    // Ejiji Cliffs Beaten
                    tmp.AddDialog("npc_sen_1", true);
                    tmp.AddDialog("npc_sen_12", true);
                    tmp.AddDialog("npc_sen_13", true);

                    tmp.msgcloud_ID = 2;
                    break;
                }
            }

            messageclouds.push_back(tmp);

            SPDLOG_DEBUG("Creating message cloud at {} {}", a_sen.getGlobalPosition().x - 15, a_sen.getGlobalPosition().y - 75);

            break;
        }
        case Buildings::ALTAR:
            t_title.setStringKey("patapolis_altar");
            a += "X: Interact      ";
            break;
        case Buildings::OBELISK:
            t_title.setStringKey("patapolis_obelisk");
            a += "X: Interact      ";
            break;
        case Buildings::MATER:
            t_title.setStringKey("patapolis_tree");
            break;
        case Buildings::PARAGET:
            t_title.setStringKey("patapolis_paraget");
            break;
        case Buildings::WAKAPON: {
            t_title.setStringKey("patapolis_wakapon");

            a += "X: Interact      ";

            ///activate her dialogue
            messageclouds.clear();

            MessageCloud tmp;
            tmp.Create(20, sf::Vector2f(a_wakapon.getGlobalPosition().x - 15, a_wakapon.getGlobalPosition().y - 75), sf::Color(255, 255, 255, 255), false, config->GetInt("textureQuality"));
            tmp.x_start_offset = 20;

            vector<int> missions = saveReader->missions_unlocked;

            if (std::find(missions.begin(), missions.end(), 1) != missions.end())
            {
                ///shida valley is unlocked

                if (missions.size() == 1) ///if it's the only mission
                {
                    ///shida valley dialogue
                    tmp.AddDialog("npc_wakapon_1", true);
                    tmp.AddDialog("npc_wakapon_2", true);
                    tmp.AddDialog("npc_wakapon_3", true);
                } else
                {
                    if (std::find(missions.begin(), missions.end(), 2) != missions.end()) ///patapine fortress STORY mission (non-repeatable)
                    {
                        ///patapine unlocked dialogue
                        tmp.AddDialog("npc_wakapon_1", true);
                        tmp.AddDialog("npc_wakapon_4", true);
                        tmp.AddDialog("npc_wakapon_5", true);
                    } else if (std::find(missions.begin(), missions.end(), 3) != missions.end()) ///patapine fortress REPEATABLE mission
                    {
                        if (missions.size() == 2) ///ejiji cliffs not unlocked yet (only shida and patapine)
                        {
                            ///ejiji locked dialogue
                            tmp.AddDialog("npc_wakapon_1", true);
                            tmp.AddDialog("npc_wakapon_6", true);
                            tmp.AddDialog("npc_wakapon_7", true);
                        } else
                        {
                            if (std::find(missions.begin(), missions.end(), 4) != missions.end()) ///ejiji cliff STORY mission
                            {
                                ///ejiji unlocked dialogue
                                tmp.AddDialog("npc_wakapon_1", true);
                                tmp.AddDialog("npc_wakapon_8", true);
                                tmp.AddDialog("npc_wakapon_9", true);
                            } else ///ejiji cliff REPEATABLE mission
                            {
                                tmp.AddDialog("npc_wakapon_1", true);
                                tmp.AddDialog("npc_wakapon_10", true);
                                tmp.AddDialog("npc_wakapon_11", true);
                            }
                        }
                    }
                }
            }

            tmp.msgcloud_ID = 1;
            messageclouds.push_back(tmp);

            SPDLOG_DEBUG("Creating message cloud at {} {}", a_wakapon.getGlobalPosition().x - 15, a_wakapon.getGlobalPosition().y - 75);

            break;
        }
        case Buildings::EGG:
            t_title.setStringKey("patapolis_egg");
            break;
        default:
            t_title.setStringKey("patapolis");
            break;
    }

    a += "Select: Save      Start: Title screen";

    ctrlTips.create(54, font, 20, a, quality);
}
float EaseIn(float time, float startValue, float change, float duration)
{
    time /= duration / 2;
    if (time < 1)
    {
        return change / 2 * time * time + startValue;
    }

    time--;
    return -change / 2 * (time * (time - 2) - 1) + startValue;
};
void PatapolisMenu::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    Config* config = CoreManager::getInstance().getConfig();
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    //if (is_active) TO-DO: is this still needed?
    if (true)
    {
        auto lastView = window->getView();
        window->setView(patapolisView);

        camera.zoom_y = 0;
        camera.Work(patapolisView, dest_zoom);

        window->draw(v_background);

        camDest = locations[location];

        
        if (StateManager::getInstance().getState() == StateManager::MATER_OUTER)
        {
            camDest += materoffset; // when mater menu is open, offset slightly so we still see the tree
        }

        float camDistance = abs(camDest - camPos); // why abs? we literally make it negative again a couple lines down
        float camSpeed = camDistance * 3;

        if (camPos != camDest)
        {
            if (left)
            {
                if (camPos < camDest)
                    camPos += camSpeed / fps;
                else
                    camPos -= camSpeed / fps;
            } else
            {
                if (camPos > camDest)
                    camPos -= camSpeed / fps;
                else
                    camPos += camSpeed / fps;
            }
        }

        for (int i = 0; i < clouds_A.size(); i++)
        {
            PSprite& cloud = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/1a.png");
            cloud.setPosition(clouds_A[i].x, clouds_A[i].y);
            cloud.lx = clouds_A[i].x - (camPos / 1.3);
            cloud.ly = clouds_A[i].y;
            cloud.draw(window);
        }

        for (int i = 0; i < clouds_B.size(); i++)
        {
            float resRatio = window->getSize().x / float(3840);

            clouds_B[i].x = clouds_B[i].base_x - (camPos / 1.3);
            clouds_B[i].cloud.setPosition(clouds_B[i].x * resRatio, clouds_B[i].y * resRatio);
            window->draw(clouds_B[i].cloud);
        }

        PSprite& rainbow = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/rainbow.png");
        rainbow.setOrigin(0, rainbow.getLocalBounds().height);
        rainbow.setPosition(9000*3 - (camPos / 1.3), 592*3);
        rainbow.draw(window);

        for (int i = 0; i < 4; i++)
        {
            PSprite& b_layer = ResourceManager::getInstance().getSprite(back_layer[i]);

            b_layer.setOrigin(0, b_layer.getLocalBounds().height);
            b_layer.setPosition(back_pos[i], 715*3);

            b_layer.lx = b_layer.baseX - (camPos / 1.3);
            b_layer.draw(window);
        }

        for (int i = 0; i < layer_6.size(); i++)
        {
            ps_layer_6[layer_6[i].type].setPosition(layer_6[i].x, layer_6[i].y);
            ps_layer_6[layer_6[i].type].lx = layer_6[i].x - (camPos / 1.1428571428571428571428571428571);
            ps_layer_6[layer_6[i].type].setOrigin(ps_layer_6[layer_6[i].type].getLocalBounds().width / 2, ps_layer_6[layer_6[i].type].getLocalBounds().height);
            ps_layer_6[layer_6[i].type].draw(window);
        }

        PSprite& L5 = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/5.png");
        PSprite& L4 = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/4.png");

        L5.setPosition(3060*3, 740*3 - floor_height);
        L4.setPosition(2530*3, 750*3 - floor_height);

        L5.lx = L5.baseX - (camPos / 1.1111111111111111111);
        L4.lx = L4.baseX - (camPos / 1.0526315789473684210526315789474);

        a_wakapon.setGlobalPosition(sf::Vector2f(11920.0*3 - camPos, a_wakapon.getGlobalPosition().y));
        a_sen.setGlobalPosition(sf::Vector2f(6450.0*3 - camPos, a_sen.getGlobalPosition().y));

        rayX = rayXbase - camPos;

        floor_x = floor_x_base - camPos;

        L5.setOrigin(L5.getLocalBounds().width / 2, L5.getLocalBounds().height);
        L4.setOrigin(L4.getLocalBounds().width / 2, L4.getLocalBounds().height);

        L5.draw(window);
        L4.draw(window);


        for (int i = 0; i < layer_2.size(); i++)
        {
            //cout << "layer_6[" << i << "]: " << layer_6[i].getGlobalBounds().width/2 << " " << layer_6[i].getGlobalBounds().height << " " << layer_6[i].getPosition().x << " " << layer_6[i].getPosition().y << endl;
            
            //layer_2[i].lx = layer_2[i].baseX - camPos;

            //layer_2[i].setOrigin(layer_2[i].getLocalBounds().width / 2, layer_2[i].getLocalBounds().height);
            //layer_2[i].draw(window);

            ps_layer_2[layer_2[i].type].setPosition(layer_2[i].x, layer_2[i].y);
            ps_layer_2[layer_2[i].type].lx = layer_2[i].x - camPos;
            ps_layer_2[layer_2[i].type].setOrigin(ps_layer_2[layer_2[i].type].getLocalBounds().width / 2, ps_layer_2[layer_2[i].type].getLocalBounds().height);
            ps_layer_2[layer_2[i].type].draw(window);
        }

        float resRatioX = window->getSize().x / float(3840);
        float resRatioY = window->getSize().y / float(2160);

        /// Fires
        for (int i = 0; i < fires.size(); i++)
        {
            fires[i].id += float(10) / fps;

            if (round(fires[i].id) >= 3)
                fires[i].id = 0;

            int id = round(fires[i].id);

            fires[i].glowsize += float(90) / fps;

            if (fires[i].glowsize >= 360)
                fires[i].glowsize -= 360;

            PSprite& glow = ResourceManager::getInstance().getSprite(fires[i].glow);

            glow.setOrigin(glow.getLocalBounds().width / 2, glow.getLocalBounds().height / 2);
            glow.setScale(1.05 + (cos(fires[i].glowsize * 3.141592 / 180) / 10));
            glow.setPosition(fires[i].baseX - camPos, fires[i].baseY);

            glow.draw(window);

            PSprite& fire = ResourceManager::getInstance().getSprite(fires[i].fire[id]);

            fire.setOrigin(fire.getLocalBounds().width / 2, fire.getLocalBounds().height / 2);
            fire.setPosition(fires[i].baseX - camPos, fires[i].baseY);

            fire.draw(window);
        }

        //cout << "for: " << endl;

        for (auto i : draw_ID)
        {
            //cout << i << endl;

            switch (i)
            {
                case Buildings::MARKET: ///Marketplace
                {
                    //for some reason, barracks are marketplace. what the fuck?

                    PSprite& barracks = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/barracks.png");
                    barracks.setPosition(640*3, 765*3 - floor_height);
                    barracks.setOrigin(barracks.getLocalBounds().width / 2, barracks.getLocalBounds().height);
                    barracks.lx = barracks.baseX - camPos;
                    barracks.draw(window);
                    break;
                }

                case Buildings::FORGE: ///Blacksmith
                {
                    PSprite& forge_main = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/forge_main.png");
                    forge_main.setPosition(2300*3, 721*3 - floor_height);
                    forge_main.setOrigin(forge_main.getLocalBounds().width / 2, forge_main.getLocalBounds().height);
                    forge_main.lx = forge_main.baseX - camPos;

                    PSprite& forge_back = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/blacksmith_forge.png");
                    forge_back.setPosition(2300*3, 721*3 - floor_height - 29*3);
                    forge_back.setOrigin(forge_back.getLocalBounds().width / 2, forge_back.getLocalBounds().height);
                    forge_back.lx = forge_back.baseX - camPos;

                    PSprite& forge_glow = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/blacksmith_forge_glow.png");
                    forge_glow.setPosition(2300*3, 720*3 - floor_height - 178*3);
                    forge_glow.setOrigin(forge_glow.getLocalBounds().width / 2, forge_glow.getLocalBounds().height);
                    forge_glow.lx = forge_glow.baseX - camPos;

                    PSprite& forge_fence = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/blacksmith_fence.png");
                    forge_fence.setPosition(2043*3, 720*3 - floor_height - 306*3);
                    forge_fence.setOrigin(forge_fence.getLocalBounds().width / 2, 0);
                    forge_fence.lx = forge_fence.baseX - camPos;

                    PSprite& forge_slab = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/slab.png");
                    forge_slab.setPosition(2300*3 - 65*3, 720*3 - floor_height - 84*3);
                    forge_slab.setOrigin(forge_slab.getLocalBounds().width / 2, forge_slab.getLocalBounds().height);
                    forge_slab.lx = forge_slab.baseX - camPos;

                    PSprite& forge_slab_glow = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/slab_glow.png");
                    forge_slab_glow.setPosition(2300*3 - 72*3, 720*3 - floor_height - 92*3);
                    forge_slab_glow.setOrigin(forge_slab_glow.getLocalBounds().width / 2, forge_slab_glow.getLocalBounds().height);
                    forge_slab_glow.lx = forge_slab_glow.baseX - camPos;

                    forge_big.id += float(12) / fps;

                    if (round(forge_big.id) >= 3)
                        forge_big.id = 0;

                    int id = round(forge_big.id);

                    forge_big.glowsize += float(90) / fps;

                    if (forge_big.glowsize >= 360)
                        forge_big.glowsize -= 360;

                    PSprite& glow = ResourceManager::getInstance().getSprite(forge_big.glow);

                    glow.setOrigin(glow.getLocalBounds().width / 2, glow.getLocalBounds().height / 2);
                    glow.setScale(1.05 + (cos(forge_big.glowsize * 3.141592 / 180) / 10));
                    glow.setPosition(forge_big.baseX - camPos, forge_big.baseY);

                    glow.draw(window);

                    PSprite& fire = ResourceManager::getInstance().getSprite(forge_big.fire[id]);

                    fire.setOrigin(fire.getLocalBounds().width / 2, fire.getLocalBounds().height / 2);
                    fire.setPosition(forge_big.baseX - camPos, forge_big.baseY);

                    fire.draw(window);

                    forge_back.draw(window);

                    forge_glowsize += float(90) / fps;

                    if (forge_glowsize >= 360)
                        forge_glowsize = 0;

                    forge_glow.setOrigin(forge_glow.getLocalBounds().width / 2, forge_glow.getLocalBounds().height / 2);
                    forge_glow.setScale(1.05 + (cos(forge_glowsize * 3.141592 / 180) / 10));
                    forge_glow.draw(window);

                    forge_purple.id += float(12) / fps;

                    if (round(forge_purple.id) >= 3)
                        forge_purple.id = 0;

                    id = round(forge_purple.id);

                    forge_purple.glowsize += float(90) / fps;

                    if (forge_purple.glowsize >= 360)
                        forge_purple.glowsize -= 360;

                    PSprite& glow2 = ResourceManager::getInstance().getSprite(forge_purple.glow);

                    glow2.setOrigin(glow2.getLocalBounds().width / 2, glow2.getLocalBounds().height / 2);
                    glow2.setScale(1.05 + (cos(forge_purple.glowsize * 3.141592 / 180) / 10));
                    glow2.setPosition(forge_purple.baseX - camPos, forge_purple.baseY);

                    glow2.draw(window);

                    PSprite& fire2 = ResourceManager::getInstance().getSprite(forge_purple.fire[id]);

                    fire2.setOrigin(fire2.getLocalBounds().width / 2, fire2.getLocalBounds().height / 2);
                    fire2.setPosition(forge_purple.baseX - camPos, forge_purple.baseY);

                    fire2.draw(window);

                    forge_slab_glow.setOrigin(forge_slab_glow.getLocalBounds().width / 2, forge_slab_glow.getLocalBounds().height / 2);
                    forge_slab_glow.setScale(1.05 + (cos(forge_glowsize * 3.141592 / 180) / 10));
                    forge_slab_glow.draw(window);

                    forge_main.draw(window);

                    forge_slab.draw(window);

                    forge_fence.draw(window);
                    break;
                }

                case Buildings::BARRACKS: ///Barracks
                {
                    //for some reason, marketplace is barracks. ??????
                    PSprite& market = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/market.png");
                    market.setPosition(3960*3, 728*3 - floor_height);
                    market.setOrigin(market.getLocalBounds().width / 2, market.getLocalBounds().height);
                    market.lx = market.baseX - camPos;
                    market.draw(window);
                    break;
                }

                case Buildings::MATER:
                {
                    PSprite& mater = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/mater.png");
                    mater.setPosition(9340*3, 728*3 - floor_height);
                    mater.setOrigin(mater.getLocalBounds().width / 2, mater.getLocalBounds().height);
                    mater.lx = mater.baseX - camPos;
                    mater.draw(window);
                    break;
                }

                case Buildings::PARAGET: ///Paraget
                {
                    PSprite& paraget_main = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/paraget_main.png");
                    paraget_main.setPosition(10600*3, 722*3 - floor_height);
                    paraget_main.setOrigin(paraget_main.getLocalBounds().width / 2, paraget_main.getLocalBounds().height);
                    paraget_main.lx = paraget_main.baseX - camPos;

                    PSprite& paraget_crystal_glow = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/paraget_crystal_glow.png");
                    paraget_crystal_glow.setPosition(10600*3, 430*3);
                    paraget_crystal_glow.setOrigin(paraget_crystal_glow.getLocalBounds().width / 2, paraget_crystal_glow.getLocalBounds().height / 2);
                    paraget_crystal_glow.lx = paraget_crystal_glow.baseX - camPos;

                    PSprite& paraget_crystal = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/paraget_crystal.png");
                    paraget_crystal.setPosition(10600*3, 430*3);
                    paraget_crystal.setOrigin(paraget_crystal.getLocalBounds().width / 2, paraget_crystal.getLocalBounds().height / 2);
                    paraget_crystal.lx = paraget_crystal.baseX - camPos;

                    crystal_y += float(90) / fps;

                    if (crystal_y >= 360)
                        crystal_y -= 360;

                    crystal_y2 += float(30) / fps;

                    if (crystal_y2 >= 360)
                        crystal_y2 -= 360;

                    paraget_crystal.ly = paraget_crystal.baseY + sin(crystal_y * 3.141592 / 180) * 10;
                    paraget_crystal_glow.ly = paraget_crystal.baseY + sin(crystal_y * 3.141592 / 180) * 10;

                    paraget_crystal_glow.setScale(1.05 + (cos(crystal_y2 * 3.141592 / 180) / 10));
                    paraget_crystal_glow.draw(window);
                    paraget_main.draw(window);
                    paraget_crystal.draw(window);

                    for (int i = 0; i < paraget_sparkles.size(); i++)
                    {
                        paraget_sparkles[i].x = paraget_sparkles[i].baseX - camPos;

                        if (paraget_sparkles[i].alpha > 0)
                        {
                            paraget_sparkles[i].alpha -= float(300) / fps;
                        } else
                        {
                            paraget_sparkles[i].alpha = 0;
                        }

                        if (paraget_sparkles[i].curScale > 0)
                        {
                            paraget_sparkles[i].curScale -= float(1.35) / fps;
                        } else
                        {
                            paraget_sparkles[i].curScale = 0;
                        }

                        if (paraget_sparkles[i].curScale <= 0)
                        {
                            paraget_sparkles[i].timer -= float(1000) / fps;
                        }

                        if (paraget_sparkles[i].timer <= 0)
                        {
                            paraget_sparkles[i].curScale = paraget_sparkles[i].initScale;
                            paraget_sparkles[i].alpha = 255;
                            paraget_sparkles[i].timer = 400 + (rand() % 200);
                            paraget_sparkles[i].y = paraget_sparkles[i].baseY + sin(crystal_y * 3.141592 / 180) * 10;
                        }

                        PSprite& sprk = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/" + paraget_sparkles[i].tx_name);

                        sprk.setColor(sf::Color(255, 255, 255, paraget_sparkles[i].alpha));
                        sprk.setScale(paraget_sparkles[i].curScale);
                        sprk.setOrigin(sprk.getLocalBounds().width / 2, sprk.getLocalBounds().height / 2);
                        sprk.setPosition(paraget_sparkles[i].x, paraget_sparkles[i].y);
                        sprk.draw(window);
                    }

                    break;
                }

                case Buildings::OBELISK: 
                {
                    PSprite& obelisk = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/obelisk.png");
                    obelisk.setPosition(8300*3, 722*3 - floor_height);
                    obelisk.setOrigin(obelisk.getLocalBounds().width / 2, obelisk.getLocalBounds().height);
                    obelisk.lx = obelisk.baseX - camPos;
                    obelisk.draw(window);
                    break;
                }
            }
        }

        // Festival
        PSprite& festival_main = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/festival_main.png");
        festival_main.setPosition(5620*3, 721*3 - floor_height);
        festival_main.setOrigin(festival_main.getLocalBounds().width / 2, festival_main.getLocalBounds().height);
        festival_main.lx = festival_main.baseX - camPos;
        festival_main.draw(window);

        /// Altar
        PSprite& altar = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/altar.png");
        altar.setPosition(7280*3, 720*3 - floor_height);
        altar.setOrigin(altar.getLocalBounds().width / 2, altar.getLocalBounds().height);
        altar.lx = altar.baseX - camPos;
        altar.draw(window);

        /// Smoke
        smokepath1 += 40.f / fps;

        if (smokepath1 >= 360)
            smokepath1 = 0;

        if (rand() % 100 == 1)
            addSmokeParticle(altar.baseX - 186*3 + (cos(smokepath1 * 3.141592 / 180) * 2), 410*3);
        if (rand() % 100 == 1)
            addSmokeParticle(altar.baseX + 186*3 + (sin(smokepath1 * 3.141592 / 180) * 2), 410*3);

        vector<int> e_s;

        for (int i = 0; i < smoke.size(); i++)
        {
            smoke[i].y -= float(20) / fps;
            smoke[i].x = smoke[i].baseX - camPos;

            smoke[i].alpha -= float(60) / fps;

            if (smoke[i].alpha <= 0)
                smoke[i].alpha = 0;

            smoke[i].curScale += float(0.14) / fps;

            PSprite& p_smoke = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/smoke.png");
            p_smoke.setPosition(smoke[i].x, smoke[i].y);
            p_smoke.setColor(sf::Color(255, 255, 255, smoke[i].alpha));
            p_smoke.setScale(smoke[i].curScale);
            p_smoke.draw(window);

            if (smoke[i].alpha <= 0)
            {
                e_s.push_back(i);
            }
        }

        for (int i = 0; i < e_s.size(); i++)
        {
            smoke.erase(smoke.begin() + e_s[i] - i);
        }

        r_ground.setPosition(floor_x * resRatioX, (float(720*3) - floor_height) * resRatioY);
        window->draw(r_ground);

        PSprite& edge = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/edge.png");
        edge.setOrigin(0, edge.getLocalBounds().height);
        edge.setPosition(floor_x + 11500*3, 720*3);
        edge.draw(window);

        /// Remaining sparkles
        for (int i = 0; i < sparkles.size(); i++)
        {
            sparkles[i].x = sparkles[i].baseX - camPos;

            if (sparkles[i].alpha > 0)
            {
                sparkles[i].alpha -= float(300) / fps;
            } else
            {
                sparkles[i].alpha = 0;
            }

            if (sparkles[i].curScale > 0)
            {
                sparkles[i].curScale -= float(1.35) / fps;
            } else
            {
                sparkles[i].curScale = 0;
            }

            if (sparkles[i].curScale <= 0)
            {
                sparkles[i].timer -= float(1000) / fps;
            }

            if (sparkles[i].timer <= 0)
            {
                sparkles[i].curScale = sparkles[i].initScale;
                sparkles[i].alpha = 255;
                sparkles[i].timer = 900 + (rand() % 200);
            }

            PSprite& sprk = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/sparkle.png");

            sprk.setColor(sf::Color(255, 255, 255, sparkles[i].alpha));
            sprk.setScale(sparkles[i].curScale);
            sprk.setOrigin(sprk.getLocalBounds().width / 2, sprk.getLocalBounds().height / 2);
            sprk.setPosition(sparkles[i].x, sparkles[i].y);
            sprk.draw(window);
        }

        /// Egg light and light rays
        light_value += 2.f / fps;
        float scale = cos(light_value) * 0.125;

        PSprite& light_1 = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/light.png");
        light_1.setPosition(11715*3, 161*3);
        light_1.setOrigin(light_1.getLocalBounds().width / 2, light_1.getLocalBounds().height / 2);
        light_1.lx = light_1.baseX - camPos;
        light_1.setScale(0.9 + scale);
        light_1.draw(window);

        PSprite& light_2 = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/light.png");
        light_2.setPosition(12720*3, 161*3);
        light_2.setOrigin(light_2.getLocalBounds().width / 2, light_2.getLocalBounds().height / 2);
        light_2.lx = light_2.baseX - camPos;
        light_2.setScale(0.9 + scale);
        light_2.draw(window);

        PSprite& egg_light = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/egg_light.png");
        egg_light.setPosition(12217*3, 288*3);
        egg_light.setOrigin(egg_light.getLocalBounds().width / 2, egg_light.getLocalBounds().height / 2);
        egg_light.lx = egg_light.baseX - camPos;
        egg_light.setScale(0.9 + scale);
        egg_light.draw(window);

        for (int i = 0; i < lightrays.size(); i++)
        {
            if (i != 11)
            {
                if (!lightrays[i].rise)
                {
                    lightrays[i].cur_distance -= float(80) / fps;

                    if (lightrays[i].cur_distance <= lightrays[i].min_distance)
                    {
                        lightrays[i].cur_distance = lightrays[i].min_distance;
                        lightrays[i].rise = true;
                    }
                } else
                {
                    lightrays[i].cur_distance += float(80) / fps;

                    if (lightrays[i].cur_distance >= lightrays[i].max_distance)
                    {
                        lightrays[i].cur_distance = lightrays[i].max_distance;
                        lightrays[i].rise = false;
                    }
                }

                float x1 = coords[i].x1 + rayX;
                float y1 = coords[i].y1;

                float x2 = coords[i].x2 + rayX;
                float y2 = coords[i].y2;

                lightrays[i].triangle[0].position = sf::Vector2f(x1 * resRatioX, y1 * resRatioY);
                lightrays[i].triangle[1].position = sf::Vector2f(x2 * resRatioX, y2 * resRatioY);
                lightrays[i].triangle[2].position = sf::Vector2f((((x1 + x2) / 2) + (lightrays[i].cur_distance * sin(lightrays[i].angle * 3.14159265 / 180))) * resRatioX, (((y1 + y2) / 2) + (lightrays[i].cur_distance * cos(lightrays[i].angle * 3.14159265 / 180))) * resRatioY);

                //cout << "[coord #" << i << "]" << lightrays[i].triangle[0].position.x << " " << lightrays[i].triangle[0].position.y << " " << lightrays[i].triangle[1].position.x << " " << lightrays[i].triangle[1].position.y << " " << lightrays[i].triangle[2].position.x << " " << lightrays[i].triangle[2].position.y << endl;

                window->draw(lightrays[i].triangle);
            }
        }

        PSprite& bridge = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/bridge.png");
        bridge.setOrigin(0, bridge.getLocalBounds().height); // could consider not drawing this + wakapon unless player camera is close
        bridge.setPosition(floor_x + 11200*3, 720*3);
        bridge.draw(window);

        /// Wakapon
        a_wakapon.Draw();

        /// Sen
        a_sen.Draw();

        PSprite& world_egg = ResourceManager::getInstance().getSprite("resources/graphics/bg/patapolis/egg.png");
        world_egg.setPosition(12215*3, 462*3);
        world_egg.setOrigin(world_egg.getLocalBounds().width / 2, world_egg.getLocalBounds().height);
        world_egg.lx = world_egg.baseX - camPos;
        world_egg.draw(window);

        //TO-DO: replace with something else probably core manager blabla
        /* if (barracks_menu.mission_started)
        {
            obelisk_menu.displayMissions = false;
            //obelisk_menu.sel_location = 1;
            obelisk_menu.sel_mission = 0;
            obelisk_menu.Hide();
            SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       mission_started = false;
        }*/

        /* TO-DO: whole ass section of not yet refactored classes.
        if(barracks_menu.is_active)
        {
            SPDLOG_TRACE("barracks_menu leftover"); //TO-DO: replace this       update(window, fps, inputCtrl);
        } else if (altar_menu->is_active)
        {
            altar_menu->update(window, fps, inputCtrl);
        } else if (mater_menu.is_active)
        {
            mater_menu.update(window, fps, inputCtrl);
        } else if (obelisk_menu.is_active)
        {
            obelisk_menu.Update(window, fps, inputCtrl);
        } else if (credits.is_active)
        {
            credits.draw(window, fps, inputCtrl);
        } else*/
        {
            if (city_loop.getStatus() == sf::Sound::Status::Stopped)
            {
                SPDLOG_DEBUG("Play city_loop");
                city_loop.play();
            }

            t_title.setFont(font);
            t_title.setCharacterSize(38);
            t_title.setOutlineThickness(2);
            t_title.setOutlineColor(sf::Color::Black);
            t_title.setOrigin(t_title.getLocalBounds().width / 2, t_title.getLocalBounds().height / 2);
            t_title.setPosition(640*3, 80*3);
            t_title.draw();

            vector<int> m_rm;

            if (dialogboxes.size() <= 0)
            {
                bool messageCloudActive = false;
                for (int i = 0; i < messageclouds.size(); i++)
                {
                    if (messageclouds[i].firstrender)
                        messageclouds[i].Show();

                    if ((messageclouds[i].msgcloud_ID == 0) || (messageclouds[i].msgcloud_ID == 2))
                        messageclouds[i].startpos = sf::Vector2f(a_sen.getGlobalPosition().x - 15, a_sen.getGlobalPosition().y - 75);
                    else if (messageclouds[i].msgcloud_ID == 1){
                        messageclouds[i].startpos = sf::Vector2f(a_wakapon.getGlobalPosition().x - 15, a_wakapon.getGlobalPosition().y - 75);
                    }
                    if (messageclouds[i].active && !messageclouds[i].done && messageclouds[i].cur_dialog > 0)
                    {
                        messageCloudActive = true;
                    }

                    if (messageclouds[i].done)
                    {
                        if (location == 4)
                        {
                            if (messageclouds[i].msgcloud_ID == 2)
                            {
                                ///Create ending dialogbox here
                                std::vector<sf::String> a = {"patapolis_demo_pick1", "patapolis_demo_pick2"};

                                PataDialogBox db;
                                db.Create(font, "patapolis_demofinish", a, config->GetInt("textureQuality"));
                                db.id = 4;
                                dialogboxes.push_back(db);

                                messageclouds[i].Hide();
                            }
                        }
                    }

                    messageclouds[i].Draw();

                    if ((!messageclouds[i].active) && (messageclouds[i].done))
                        m_rm.push_back(i);
                }
                if (messageCloudActive){
                    dest_zoom = 0.8f; // zoom in when message cloud is active
                    if (location == 9){ // wakapon 
                        camera.manual_y_dest = -100; // move camera up a bit when wakapon is zoomed into to adjust for bridge
                    } else {
                        camera.manual_y_dest = 0;
                    }
                } else {
                    dest_zoom = 1.0f;
                    camera.manual_y_dest = 0;
                }
            } 

            for (int i = 0; i < m_rm.size(); i++)
            {
                SPDLOG_DEBUG("Erasing MessageCloud id {}", m_rm[i]);
                messageclouds.erase(messageclouds.begin() + m_rm[i] - i);
            }
        }

        vector<int> db_e; ///dialog box erase

        for (int i = 0; i < dialogboxes.size(); i++)
        {
            dialogboxes[i].x = 640*3;
            dialogboxes[i].y = 360*3;
            dialogboxes[i].Draw();

            if (dialogboxes[i].closed)
                db_e.push_back(i);
        }

        for (int i = 0; i < db_e.size(); i++)
        {
            dialogboxes.erase(dialogboxes.begin() + db_e[i] - i);
        }

        window->setView(lastView);

        lastView = window->getView();
        window->setView(window->getDefaultView());

        //if ((!mater_menu.is_active) && (!credits.is_active))
        //{
            //ctrlTips.x = 0;
            //ctrlTips.y = (720 - ctrlTips.ySize);
            //TO-DO: control tips without window argument
            //ctrlTips.draw(window);
        //}

        /*if(fade_alpha > 0)
        {
            fade_alpha -= float(500) / fps;
        }

        if(fade_alpha <= 0)
        {
            fade_alpha = 0;
        }

        fade_box.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
        fade_box.setFillColor(sf::Color(0,0,0,fade_alpha));
        window->draw(fade_box);*/

        screenFade.draw();

        if (screenFade.checkFinished())
        {
            if (goto_id != -1)
            {
                switch (goto_id)
                {
                    case 0: ///Barracks
                    {
                        city_loop.stop();

                        screenFade.Create(ScreenFade::FADEIN, 1024);
                        StateManager::getInstance().setState(StateManager::BARRACKS);

                        break;
                    }

                    case 1: ///Obelisk
                    {
                        city_loop.stop();

                        screenFade.Create(ScreenFade::FADEIN, 1024);
                        StateManager::getInstance().setState(StateManager::OBELISK);

                        break;
                    }

                    case 2: ///Exit barracks
                    {
                        city_loop.stop();
                        is_active = true;

                        screenFade.Create(ScreenFade::FADEIN, 1024);

                        break;
                    }

                    case 6: ///credits
                    {
                        city_loop.stop();
                        credits.is_active = true;
                        credits.restart();

                        screenFade.Create(ScreenFade::FADEIN, 1024);

                        break;
                    }

                    case 7: ///go to title screen
                    {
                        city_loop.stop();
                        this->is_active = false;

                        screenFade.Create(ScreenFade::FADEIN, 1024);
                        StateManager::getInstance().setState(StateManager::MAINMENU);

                        return; //i return the function here so no more routines are executed when Patapolis is destroyed.

                        break;
                    }
                }

                goto_id = -1;
            }
        }

        window->setView(lastView);

        if (dialogboxes.size() <= 0)
        {
            if ((StateManager::getInstance().getState() != StateManager::PATAPOLIS_ALTAR) && (StateManager::getInstance().getState() != StateManager::MATER_OUTER) && (!credits.is_active) && (screenFade.checkFinished()))
            {
                if ((inputCtrl->isKeyPressed(Input::Keys::LEFT)) || (inputCtrl->isKeyPressed(Input::Keys::LTRIGGER)))
                {
                    if (location > 0)
                    {
                        location--;
                        SPDLOG_INFO("New Location {}", location);
                        left = true;

                        for (int i = 0; i < messageclouds.size(); i++)
                            messageclouds[i].End();

                        SetTitle(location);
                    }
                } else if ((inputCtrl->isKeyPressed(Input::Keys::RIGHT)) || (inputCtrl->isKeyPressed(Input::Keys::RTRIGGER)))
                {
                    if (location < locations.size() - 1)
                    {
                        location++;
                        SPDLOG_INFO("New Location {}", location);
                        left = false;

                        for (int i = 0; i < messageclouds.size(); i++)
                            messageclouds[i].End();

                        SetTitle(location);
                    }
                } else if (inputCtrl->isKeyPressed(Input::Keys::CROSS))
                {
                    // select the current menu item
                    switch (location)
                    {
                        case Buildings::MARKET:
                            /// trader/random
                            // open the world map
                            break;
                        case Buildings::BARRACKS:
                            /// armory/barracks

                            screenFade.Create(ScreenFade::FADEOUT, 1024);
                            goto_id = 0;

                            break;
                        case Buildings::FESTIVAL:
                            /// festival
                            // open barracks screen
                            break;
                        case Buildings::ALTAR:
                            /// altar
                            // open mater menu
                            
                            
                            /* altar_menu->save_loaded = save_loaded;
                            altar_menu->reloadInventory();
                            altar_menu->showAltar();
                            altar_menu->is_active = true;
                            altar_menu->showAltar(); */

                            StateManager::getInstance().setState(StateManager::PATAPOLIS_ALTAR);

                            break;
                        case Buildings::OBELISK:
                            /// obelisk

                            screenFade.Create(ScreenFade::FADEOUT, 1024);
                            goto_id = 1;

                            /*obelisk_menu.Reload();
                        obelisk_menu.Show();
                        obelisk_menu.isActive = true;*/
                            break;
                        case Buildings::MATER:
                            /// festival
                            // open barracks screen
                            StateManager::getInstance().setState(StateManager::MATER_OUTER);
                            //mater_menu.save_loaded = save_loaded;
                            //mater_menu.is_active = true;
                            //mater_menu.showMater();
                            break;
                        default:
                            /// nothing
                            break;
                    }
                }

                if (inputCtrl->isKeyPressed(Input::Keys::START))
                {
                    std::vector<sf::String> a = {"nav_yes", "nav_no"};

                    PataDialogBox db;
                    db.Create(font, "patapolis_returntomain", a, config->GetInt("textureQuality"));
                    db.id = 0;
                    dialogboxes.push_back(db);
                } else if (inputCtrl->isKeyPressed(Input::Keys::SELECT))
                {
                    std::vector<sf::String> a = {"nav_yes", "nav_no"};

                    PataDialogBox db;
                    db.Create(font, "patapolis_save", a, config->GetInt("textureQuality"));
                    db.id = 2;
                    dialogboxes.push_back(db);
                }
            }
        } else
        {
            if (inputCtrl->isKeyPressed(Input::Keys::CROSS))
            {
                switch (dialogboxes[dialogboxes.size() - 1].CheckSelectedOption())
                {
                    case 0: {
                        if (dialogboxes[dialogboxes.size() - 1].id == 0)
                        {
                            SPDLOG_DEBUG("Open second dialogbox");
                            dialogboxes[dialogboxes.size() - 1].Close();

                            std::vector<sf::String> a = {"nav_yes", "nav_no"};

                            PataDialogBox db;
                            db.Create(font, "patapolis_returntomainsave", a, config->GetInt("textureQuality"));
                            db.id = 1;
                            dialogboxes.push_back(db);

                            break;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 1)
                        {
                            SPDLOG_INFO("Left from Patapolis to Title screen.");
                            screenFade.Create(ScreenFade::FADEOUT, 1024);
                            goto_id = 7;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 2)
                        {
                            SPDLOG_DEBUG("Saving game should happen here. Game not saving yet.");
                            CoreManager::getInstance().getSaveReader()->Save();
                            SPDLOG_INFO("Saved the game.");

                            dialogboxes[dialogboxes.size() - 1].Close();

                            std::vector<sf::String> a = {"nav_understood"};

                            PataDialogBox db;
                            db.Create(font, "patapolis_saved", a, config->GetInt("textureQuality"));
                            db.id = 3;
                            dialogboxes.push_back(db);

                            break;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 3)
                        {
                            SPDLOG_DEBUG("Done.");

                            dialogboxes[dialogboxes.size() - 1].Close();
                            break;
                        } else if (dialogboxes[dialogboxes.size() - 1].id == 4)
                        {
                            SPDLOG_INFO("Going into credits!");
                            screenFade.Create(ScreenFade::FADEOUT, 1024);
                            goto_id = 6;

                            dialogboxes[dialogboxes.size() - 1].Close();
                            break;
                        }

                        break;
                    }

                    case 1: {
                        SPDLOG_DEBUG("Back to Patapolis");
                        dialogboxes[dialogboxes.size() - 1].Close();

                        break;
                    }
                }
            }
        }
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
    SPDLOG_DEBUG("Patapolis destructor");

    //TO-DO: if you want to clean the memory that is taken from Patapolis, look at AnimatedObject and optimize it first.
    //       probably won't happen before we get new animations and someone (me) is motivated enough to write it from beginning. /owocek
}
