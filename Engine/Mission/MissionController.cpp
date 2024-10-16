#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "MissionController.h"
#include "../CoreManager.h"
#include "../StateManager.h"

#include <fstream>

using json = nlohmann::json;
using namespace std::chrono;

MissionController::MissionController()
{
    int q = CoreManager::getInstance().getConfig()->GetInt("textureQuality");
    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    sb_win_jingle.loadFromFile("resources/sfx/level/victory.ogg");
    sb_lose_jingle.loadFromFile("resources/sfx/level/failure.ogg");

    sb_cheer1.loadFromFile("resources/sfx/level/cheer1.ogg");
    sb_cheer2.loadFromFile("resources/sfx/level/cheer2.ogg");
    sb_cheer3.loadFromFile("resources/sfx/level/cheer1.ogg");



    t_win.setFont(font);
    t_win.setCharacterSize(56);
    t_win.setStringKey("mission_complete");

    t_lose.setFont(font);
    t_lose.setCharacterSize(56);
    t_lose.setStringKey("mission_failed");


    t_win.setOrigin(t_win.getLocalBounds().width / 2, t_win.getLocalBounds().height / 2);
    t_lose.setOrigin(t_lose.getLocalBounds().width / 2, t_lose.getLocalBounds().height / 2);

    bar_win.loadFromFile("resources/graphics/mission/bar_win.png", q);
    bar_lose.loadFromFile("resources/graphics/mission/bar_lose.png", q);

    bar_win.setOrigin(bar_win.getLocalBounds().width / 2, bar_win.getLocalBounds().height / 2);
    bar_lose.setOrigin(bar_lose.getLocalBounds().width / 2, bar_lose.getLocalBounds().height / 2);

}

void MissionController::LoadMission(const std::string& path)
{
    SPDLOG_INFO("Attempting to load {}", path);
    std::uniform_real_distribution<double> roll(0.0, 1.0);

    json mission;

    std::ifstream mfile(path);
    if(mfile.good())
    {
        mission << mfile;
    }
    else
    {
        SPDLOG_ERROR("Mission file not found.");
        throw std::exception();
    }

    std::string background, bgm;
    if(mission.contains("params"))
    {
        if(mission["params"].contains("background"))
            background = mission["params"]["background"];
        if(mission["params"].contains("bgm"))
            bgm = mission["params"]["bgm"];
    }

    hatapons.push_back(std::make_unique<Hatapon>());

    int pons = CoreManager::getInstance().getConfig()->GetInt("SUPER_DEBUG_SCARY_YARIPON_COUNT");

    if(pons == 0)
    {
        for(int i=1; i<=6; i++)
        {
            yaripons.push_back(std::make_unique<Yaripon>(i, 6));

            Pon* currentPon = CoreManager::getInstance().getSaveReader()->ponReg.GetPonByID(i-1);
            InventoryData::InventoryItem eq = CoreManager::getInstance().getSaveReader()->invData.items[currentPon->slots[0]];

            std::string wpn = eq.item->spritesheet+"/"+Func::num_padding(eq.item->spritesheet_id, 4);
            yaripons.back().get()->wpn = wpn;

            eq = CoreManager::getInstance().getSaveReader()->invData.items[currentPon->slots[1]];

            std::string hlm = eq.item->spritesheet+"/"+Func::num_padding(eq.item->spritesheet_id, 4);
            yaripons.back().get()->hlm = hlm;

            yaripons.back().get()->main.loadExtra(wpn, "weapon");
            yaripons.back().get()->main.loadExtra(hlm, "helm");
        }
    }
    else
    {
        for(int i=1; i<=pons; i++)
        {
            yaripons.push_back(std::make_unique<Yaripon>(i, pons));
            yaripons.back().get()->main.loadExtra("main/0025", "weapon");
            yaripons.back().get()->main.loadExtra("main/0030", "helm");
        }
    }

    feverworms.push_back(std::make_unique<FeverWorm>());

    if(mission.contains("entities"))
    {
        int en_c = 0;

        for(auto entity : mission["entities"])
        {
            std::string p = entity["path"];

            // handle end flag exclusively
            if(p == "resources/units/entity/end_flag.zip")
            {
                endflags.push_back(std::make_unique<EndFlag>());
                auto e = endflags.back().get();
                e->global_y = 1485;

                for(auto param : entity["params"].items())
                {
                    if(param.key() == "xpos")
                        e->global_x = param.value();
                }
            }
            else
            {
                if(std::filesystem::exists(p))
                {
                    entities.push_back(std::make_unique<Entity>());
                    auto e = entities.back().get();
                    e->LoadEntity(p);

                    if (entity.contains("params"))
                    {
                        auto& params = entity["params"];
                        SPDLOG_INFO("mission params: {}", params.dump());

                        // Check xpos
                        if (params.contains("xpos") && !params["xpos"].is_null())
                            e->global_x = params["xpos"];

                        // Check ypos
                        if (params.contains("ypos") && !params["ypos"].is_null())
                            e->global_y = params["ypos"];

                        // Check color
                        if (params.contains("color") && !params["color"].is_null())
                            e->setColor(Func::hexToColor(params["color"]));

                        // Check hp
                        if (params.contains("hp") && !params["hp"].is_null())
                        {
                            e->maxHP = params["hp"];
                            e->curHP = e->maxHP;
                        }

                        // Check extra parameters
                        if (params.contains("extra") && !params["extra"].is_null())
                        {
                            SPDLOG_INFO("mission extras: {}", params["extra"].dump());
                            for (auto& j : params["extra"].items())
                            {
                                e->loadExtra(j.value(), j.key());
                            }
                        }
                    }

                    SPDLOG_INFO("Custom loot table: {}", entity["loot"].dump());
                    if(entity.contains("loot") && !entity["loot"].is_null())
                    {
                        vector<Entity::Loot> new_loot;
                        Func::parseEntityLoot(CoreManager::getInstance().getCore()->gen, roll, entity["loot"], new_loot);

                        e->loot_table = new_loot;
                    }

                    if(entity.contains("behavior") && !entity["behavior"].is_null())
                    {
                        SPDLOG_INFO("custom mission behavior!!! {}", entity["behavior"].dump());

                        if(entity["behavior"].contains("hit") && !entity["behavior"]["hit"].is_null())
                            e->bh_hit = behavior.convStringToHitEnum(entity["behavior"]["hit"]);
                        if(entity["behavior"].contains("death") && !entity["behavior"]["death"].is_null())
                            e->bh_death = behavior.convStringToDeathEnum(entity["behavior"]["death"]);
                        if(entity["behavior"].contains("spawn") && !entity["behavior"]["spawn"].is_null())
                            e->bh_spawn = behavior.convStringToSpawnEnum(entity["behavior"]["spawn"]);
                        if(entity["behavior"].contains("loot") && !entity["behavior"]["loot"].is_null())
                            e->bh_loot = behavior.convStringToLootEnum(entity["behavior"]["loot"]);
                        if(entity["behavior"].contains("decision") && !entity["behavior"]["decision"].is_null())
                            e->bh_decision = behavior.convStringToDecisionEnum(entity["behavior"]["decision"]);
                        if(entity["behavior"].contains("attack") && !entity["behavior"]["attack"].is_null())
                            e->bh_attack = behavior.convStringToAttackEnum(entity["behavior"]["attack"]);
                        if(entity["behavior"].contains("approach") && !entity["behavior"]["approach"].is_null())
                            e->bh_approach = behavior.convStringToApproachEnum(entity["behavior"]["approach"]);
                    }

                    e->orderID = en_c;
                }
                else
                {
                    SPDLOG_ERROR("Specified entity file does not exist: {}", p);
                }
            }

            en_c++;
        }
    }

    bg.Load(background);

    CoreManager::getInstance().reinitSongController();
    CoreManager::getInstance().getSongController()->LoadTheme(bgm);
    CoreManager::getInstance().getRhythm()->LoadTheme(bgm);

    feverworms.back().get()->main.animation.animationSpeed = 60.6 * (CoreManager::getInstance().getSongController()->getBPM() / 120.f);

    lastRhythmCheck = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    //ExecuteZoom(0.999, 1000);
    mission_view.setSize(sf::Vector2f(CoreManager::getInstance().getWindow()->getSize().x, CoreManager::getInstance().getWindow()->getSize().y));

    initialized = true;
}

void MissionController::SendProjectile(float x, float y, float hspeed, float vspeed, std::string prj_tex, bool evil)
{
    projectiles.push_back(std::make_unique<Projectile>("resources/graphics/item/textures/"+prj_tex+".png", x, y, hspeed, vspeed, evil));
    SPDLOG_DEBUG("SENDING PROJECTILE AT {} {} {} {}", x, y, hspeed, vspeed);
}

void MissionController::SendItemDrop(std::vector<int> order_id, float x, float y)
{
    droppeditems.push_back(std::make_unique<DroppedItem>(order_id));
    droppeditems.back().get()->global_x = x;
    droppeditems.back().get()->global_y = y;
    SPDLOG_DEBUG("SENDING DROPPED ITEM: [{}, {}, {}]", order_id[0], order_id[1], order_id[2]);
}

void MissionController::ExecuteZoom(float speed, float zoomUntil)
{
    if(speed != cam.zoomSpeed)
    {
        SPDLOG_DEBUG("Executing zoom: {} {}", speed, zoomUntil);

        cam.zoomClock.restart();
        cam.zoomSpeed = speed;
        cam.zoomUntil = zoomUntil;
        cam.activateZoom = true;
    }
}

void MissionController::ProcessDroppedItems()
{
    for(auto iid : dropped_ids)
    {
        CoreManager::getInstance().getSaveReader()->invData.addItem(iid, CoreManager::getInstance().getSaveReader()->itemReg, 1);
    }
}

void MissionController::DoMissionEnd()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    /** Make the missionEndTimer unusable until the mission is not finished **/
    if (!missionEnd)
        missionEndTimer.restart();

    /** Mission end cheering **/

    if (missionEnd)
    {
        if (!failure)
        {
            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 2500)
            {
                if (!playCheer[0])
                {
                    s_cheer.stop();
                    s_cheer.setBuffer(sb_cheer1);
                    s_cheer.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                    s_cheer.play();
                    playCheer[0] = true;
                }
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 4500)
            {
                if (!playCheer[1])
                {
                    s_cheer.stop();
                    s_cheer.setBuffer(sb_cheer2);
                    s_cheer.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                    s_cheer.play();
                    playCheer[1] = true;
                }
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 6500)
            {
                if (!playCheer[2])
                {
                    s_cheer.stop();
                    s_cheer.setBuffer(sb_cheer3);
                    s_cheer.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                    s_cheer.play();
                    playCheer[2] = true;
                }
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 8000)
            {
                if (!playJingle)
                {
                    s_jingle.setBuffer(sb_win_jingle);
                    s_jingle.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                    s_jingle.play();
                    playJingle = true;
                }
            }
        } else
        {
            if (!playJingle)
            {
                s_jingle.setBuffer(sb_lose_jingle);
                s_jingle.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                s_jingle.play();
                playJingle = true;
            }
        }
    }

    if(!failure)
    {
        hatapons.back().get()->Wave();

        for(auto& yaripon : yaripons)
        {
            yaripon->Advance();
        }

        advance = true;
    }

    /** Make the camera follow Patapons until the jingle is played **/

    if (missionEndTimer.getElapsedTime().asMilliseconds() < 7700)
    {
        float leftmostPataX = 99999999;
        float rightmostPataX = -99999999;

        for(auto& p : hatapons)
        {
            float pos = p->global_x+p->local_x;
            if(pos < leftmostPataX)
                leftmostPataX = pos;
            if(pos > rightmostPataX)
                rightmostPataX = pos;
        }

        for(auto& p : yaripons)
        {
            float pos = p->global_x+p->local_x+p->attack_x+p->gap_x;
            if(pos < leftmostPataX)
                leftmostPataX = pos;
            if(pos > rightmostPataX)
                rightmostPataX = pos;
        }

        cam.followobject_x = ((leftmostPataX + rightmostPataX - 300) / 2) * (window->getSize().x / float(3840));

    }

    /** Mission fade in and fade out **/

    if (!missionEnd)
    {
        if (fade_alpha > 0)
        {
            fade_alpha -= float(500) / fps;
        }

        if (fade_alpha <= 0)
        {
            fade_alpha = 0;
        }
    } else
    {
        if (!failure)
        {
            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 11000)
            {
                if (fade_alpha < 255)
                {
                    fade_alpha += float(250) / fps;
                }

                if (fade_alpha >= 255)
                {
                    fade_alpha = 255;
                }
            }
        } else
        {
            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 10000)
            {
                if (fade_alpha < 255)
                {
                    fade_alpha += float(250) / fps;
                }

                if (fade_alpha >= 255)
                {
                    fade_alpha = 255;
                }
            }
        }
    }

    auto v = window->getView();
    window->setView(window->getDefaultView());

    fade_box.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
    fade_box.setFillColor(sf::Color(0, 0, 0, fade_alpha));
    window->draw(fade_box);

    /** Mission end event (Mission complete/Mission failed screen + transition to Patapolis **/

    if (!failure) ///Victory
    {
        if (missionEndTimer.getElapsedTime().asMilliseconds() >= 11500)
        {
            if (!textBounce)
            {
                if (missionEndTimer.getElapsedTime().asMilliseconds() >= 13050)
                {
                    textCurScale = 1.4;
                    textBounce = true;
                }
            }

            t_win.setOrigin(t_win.getLocalBounds().width / 2, t_win.getLocalBounds().height / 2);

            if (barCurX > barDestX)
            {
                barCurX -= (abs(barCurX - barDestX) * 5) / fps;
            } else
            {
                barCurX = barDestX;
            }

            if (textCurX < textDestX)
            {
                textCurX += (abs(textCurX - textDestX) * 5) / fps;
            } else
            {
                textCurX = textDestX;
            }
            if (textCurScale > textDestScale)
            {
                textCurScale -= (abs(textCurScale - textDestScale) * 5) / fps;
            } else
            {
                textCurScale = textDestScale;
            }

            t_win.setScale(textCurScale);

            bar_win.setPosition(barCurX, 1080);
            t_win.setPosition(textCurX, 1080);
            //t_win_outline.setPosition(textCurX + 6, 1080 - 12);

            bar_win.draw();
            //t_win_outline.draw();
            t_win.draw();

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 17000)
            {
                if (fadeout_alpha < 255)
                {
                    fadeout_alpha += float(250) / fps;
                }

                if (fadeout_alpha >= 255)
                {
                    fadeout_alpha = 255;
                }

                fadeout_box.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
                fadeout_box.setFillColor(sf::Color(0, 0, 0, fadeout_alpha));
                window->draw(fadeout_box);
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() > 19000)
            {
                SPDLOG_INFO("End mission");

                //StopMission();

                SPDLOG_INFO("Add the picked up items to item repository");
                ProcessDroppedItems();
                //submitPickedItems();
                //updateMissions();
                //ClearMissionMemory();

                SPDLOG_INFO("Go to Patapolis");
                returnToPatapolis = true;
            }
        }
    } else ///Failure
    {
        if (missionEndTimer.getElapsedTime().asMilliseconds() >= 2500)
        {
            if (fade_alpha < 255)
            {
                fade_alpha += float(250) / fps;
            }

            if (fade_alpha >= 255)
            {
                fade_alpha = 255;
            }

            t_lose.setOrigin(t_lose.getLocalBounds().width / 2, t_lose.getLocalBounds().height / 2);

            if (barCurX > barDestX)
            {
                barCurX -= (abs(barCurX - barDestX) * 5) / fps;
            } else
            {
                barCurX = barDestX;
            }

            if (textCurX < textDestX)
            {
                textCurX += (abs(textCurX - textDestX) * 5) / fps;
            } else
            {
                textCurX = textDestX;
            }

            t_lose.setScale(textCurScale);

            bar_lose.setPosition(barCurX, 1080);
            t_lose.setPosition(textCurX - 21, 1080 - 42);

            bar_lose.draw();
            t_lose.draw();

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 6000)
            {
                if (fadeout_alpha < 255)
                {
                    fadeout_alpha += float(250) / fps;
                }

                if (fadeout_alpha >= 255)
                {
                    fadeout_alpha = 255;
                }

                fadeout_box.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
                fadeout_box.setFillColor(sf::Color(0, 0, 0, fadeout_alpha));
                window->draw(fadeout_box);
            }

            if (missionEndTimer.getElapsedTime().asMilliseconds() >= 8000)
            {
                /** End flag executes the mission victory code, so mission failed code needs to be executed separately here. **/

                SPDLOG_INFO("End mission");

                //StopMission();
                //ClearMissionMemory();

                SPDLOG_INFO("Go to Patapolis");
                returnToPatapolis = true;
            }
        }
    }

    window->setView(v);
}

void MissionController::Update()
{
    Rhythm* rhythm = CoreManager::getInstance().getRhythm();
    RhythmGUI* rhythmGUI = CoreManager::getInstance().getRhythmGUI();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();

    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    float fps = CoreManager::getInstance().getCore()->getFPS();

    // TODO: why the fuck is this needed? without this, zoom doesn't work correctly.
    zoom_offset = (0.000709722222222 * CoreManager::getInstance().getWindow()->getSize().y);

    rhythm->doRhythm();

    std::vector<Rhythm::RhythmMessage> messages = rhythm->fetchRhythmMessages(lastRhythmCheck);

    uint64_t lastDebugTimestamp = 0;
    if(rhythm->messages.size() > 0)
    {
        lastDebugTimestamp = rhythm->messages[rhythm->messages.size()-1].timestamp;
    }

    SPDLOG_TRACE("Checked rhythm messages. Last check set to {}, size {}, last message timestamp {}", lastRhythmCheck, messages.size(), lastDebugTimestamp);

    if(messages.size() > 0)
    {
        SPDLOG_DEBUG("Received {} rhythm messages", messages.size());
    }

    for(auto message : messages)
    {
        SPDLOG_DEBUG("message-> action: {}, message: {}, timestamp {}", message.action, message.message, message.timestamp);

        Rhythm::RhythmAction action = message.action;

        if(action == Rhythm::RhythmAction::COMMAND)
        {
            SPDLOG_DEBUG("Received a command: {}. React appropriately", message.message);
            hatapons.back().get()->Wave();

            if(message.message == "65109" || message.message == "6") // PATA PATA PATA PON
            {
                for(auto& yaripon : yaripons)
                {
                    yaripon->Advance();
                }

                advance = true;
                advanceClock.restart();
            }

            if(message.message == "146359") // PON PON PATA PON
            {
                for(auto& yaripon : yaripons)
                {
                    if(inFever)
                        yaripon->Attack(1);
                    else
                        yaripon->Attack(3);

                    advanceClock.restart();
                }
            }

            if(message.message == "308859") // CHAKA CHAKA PATA PON
            {
                for(auto& yaripon : yaripons)
                {
                    if(inFever)
                        yaripon->Attack(2);
                    else
                        yaripon->Attack(4);

                    advanceClock.restart();
                }
            }
        }

        if(action == Rhythm::RhythmAction::FOUND_COMMAND)
        {
            pataCurMaxSpeed = pataMaxSpeed;
        }

        if(action == Rhythm::RhythmAction::PERFECT_COMMAND)
        {
            pataCurMaxSpeed = pataMaxSpeed * 1.2;
        }

        if(action == Rhythm::RhythmAction::COMBO_BREAK)
        {
            hatapons.back().get()->StopAll();

            for(auto& yaripon : yaripons)
            {
                yaripon->StopAll();
            }

            advance = false;
            inFever = false;

            feverworms.back().get()->Break();
        }

        if(action == Rhythm::RhythmAction::FEVER_ON)
        {
            inFever = true;
            feverworms.back().get()->Transform();
        }

        if(action == Rhythm::RhythmAction::DRUM_ANY)
        {
            std::vector<std::string> params = Func::Split(message.message, ' ');
            auto drum = params[0];

            SPDLOG_DEBUG("Current drum: {}", drum);

            for(auto& yaripon : yaripons)
            {
                yaripon->Drum(drum);
            }
        }
    }

    float actionClockLimit = CoreManager::getInstance().getRhythm()->measure_ms - CoreManager::getInstance().getRhythm()->halfbeat_ms;

    if(advance)
    {
        pataSpeed += pataMaxSpeed*accelerationFactor / fps;
        if(pataSpeed > pataMaxSpeed)
            pataSpeed = pataMaxSpeed;
    }
    else
    {
        pataSpeed -= pataMaxSpeed*decelerationFactor / fps;
        if(pataSpeed < 0)
            pataSpeed = 0;
    }

    if(advanceClock.getElapsedTime().asMilliseconds() > actionClockLimit)
    {
        hatapons.back().get()->StopAll();

        for(auto& yaripon : yaripons)
        {
            if(yaripon->action == 2) // march
                yaripon->StopAll();
            if(yaripon->action == 1 && advanceClock.getElapsedTime().asMilliseconds() > actionClockLimit*2) // attack
                yaripon->StopAttack();
        }

        advance = false;
    }

    followPoint += pataSpeed / fps;

    //SPDLOG_DEBUG("PataSpeed: {}, PataMaxSpeed: {}, followPoint: {}", pataSpeed, pataMaxSpeed, followPoint);

    float resRatioX = CoreManager::getInstance().getWindow()->getSize().x / float(3840);
    float resRatioY = CoreManager::getInstance().getWindow()->getSize().y / float(2160);

    auto lastView = CoreManager::getInstance().getWindow()->getView();
    CoreManager::getInstance().getWindow()->setView(mission_view);

    // Update positions
    hatapons.back().get()->global_x = followPoint*3 - 400;
    hatapons.back().get()->global_y = 1490 + cam.zoom_y / zoom_offset;

    float closest = 9999999;

    for(auto& entity : entities)
    {
        if(entity->getGlobalPosition().x < closest && (entity->entityType == Entity::EntityTypes::HOSTILE || entity->entityType == Entity::EntityTypes::NEUTRAL) && !entity->dead)
            closest = entity->getGlobalPosition().x + entity->getLocalPosition().x;
    }

    // if farthest yaripon sees enemy, whole army shall be angry
    bool yari_inSight = false;

    if(yaripons.size() > 0)
    {
        float yari_distance = closest - yaripons.back().get()->global_x - yaripons.back().get()->local_x - yaripons.back().get()->gap_x;

        if(yari_distance < 3000 && !entities.empty())
        {
            yari_inSight = true;
            ExecuteZoom(1.0007, 1.2);
        }
        else
        {
            ExecuteZoom(0.9993, 0.8);
        }
    }

    for(auto& pon : yaripons)
    {
        pon->global_x = followPoint*3 - 240;
        pon->global_y = 1740; // ,/0.511 dla 720p ,/ 1.533 dla 2160p
        pon->cam_offset = cam.zoom_y / zoom_offset;
        pon->closestEnemyX = closest;
        pon->enemyInSight = yari_inSight;
        pon->missionEnd = missionEnd;
        pon->failure = failure;
    }

    for(auto& entity : entities)
    {
        entity->cam_offset = cam.zoom_y / zoom_offset;
    }

    sf::RectangleShape hbb;
    hbb.setSize(sf::Vector2f(10,10));
    hbb.setFillColor(sf::Color::Green);

    projectiles.erase(
        std::remove_if(
            projectiles.begin(),
            projectiles.end(),
            [](const std::unique_ptr<Projectile>& projectile) {
                return projectile->finished;
            }
        ),
        projectiles.end()
    );

    entities.erase(
        std::remove_if(
            entities.begin(),
            entities.end(),
            [](const std::unique_ptr<Entity>& entity) {
                return entity->forRemoval;
            }
        ),
        entities.end()
        );

    yaripons.erase(
            std::remove_if(
                yaripons.begin(),
                yaripons.end(),
                [](const std::unique_ptr<Yaripon>& pon) {
                    return pon->for_removal;
                }
            ),
            yaripons.end()
        );

    for(auto& projectile : projectiles)
    {
        projectile->cam_offset = cam.zoom_y / zoom_offset;

        projectile->Update();

        for(auto& entity : entities)
        {
            if((entity->entityType == Entity::EntityTypes::HOSTILE || entity->entityType == Entity::EntityTypes::NEUTRAL) && !entity->dead)
            {
                auto pos = sf::Vector2f(entity->getGlobalPosition() + entity->getLocalPosition());

                if(!projectile->fromEnemy)
                {
                    for(auto hb : entity->getHitbox())
                    {
                        if(projectile->tipX > pos.x+hb.left && projectile->tipX < pos.x+hb.width)
                        {
                            if(projectile->tipY > pos.y+hb.top && projectile->tipY < pos.y+hb.height)
                            {
                                projectile->finished = true;
                                entity->handleHit(rand() % 20 + 10);
                            }
                        }
                    }
                }
            }
        }

        for(auto& pon : yaripons)
        {
            if(!pon->dead)
            {
                auto pos = sf::Vector2f(pon->main.getGlobalPosition() + pon->main.getLocalPosition());

                if(projectile->fromEnemy)
                {
                    for(auto hb : pon->main.animation.animations[pon->main.animation.currentAnimation].hitboxes)
                    {
                        if(projectile->tipX > pos.x+hb.left && projectile->tipX < pos.x+hb.width)
                        {
                            if(projectile->tipY > pos.y+hb.top && projectile->tipY < pos.y+hb.height)
                            {
                                projectile->finished = true;
                                pon->curHP -= 3 + rand() % 5;
                            }
                        }
                    }
                }
            }
        }

        for(auto& pon : hatapons)
        {
            if(pon->curHP > 0)
            {
                auto pos = sf::Vector2f(pon->main.getGlobalPosition() + pon->main.getLocalPosition());

                if(projectile->fromEnemy)
                {
                    for(auto hb : pon->main.animation.animations[pon->main.animation.currentAnimation].hitboxes)
                    {
                        if(projectile->tipX > pos.x+hb.left && projectile->tipX < pos.x+hb.width)
                        {
                            if(projectile->tipY > pos.y+hb.top && projectile->tipY < pos.y+hb.height)
                            {
                                projectile->finished = true;
                                pon->curHP -= 3 + rand() % 5;
                            }
                        }
                    }
                }
            }
        }
    }

    // find leftmost and rightmost patapon so a middlespot can be calculated
    // i think in patapon the camera actually follows the rightmost spot
    // can be experimented here

    float leftmostPataX = 99999999;
    float rightmostPataX = -99999999;

    for(auto& p : hatapons)
    {
        float pos = p->global_x+p->local_x;
        if(pos < leftmostPataX)
            leftmostPataX = pos;
        if(pos > rightmostPataX)
            rightmostPataX = pos;
    }

    for(auto& p : yaripons)
    {
        float pos = p->global_x+p->local_x+p->attack_x+p->gap_x;
        if(pos < leftmostPataX)
            leftmostPataX = pos;
        if(pos > rightmostPataX)
            rightmostPataX = pos;
    }

    if(rightmostPataX > endflags.back().get()->global_x && !missionEnd)
    {
        missionEnd = true;
        rhythm->Stop();
        missionEndTimer.restart();
        endflags.back().get()->main.setAnimation("triggered");
        endflags.back().get()->main.restartAnimation();
    }

    if(!missionEnd)
    {
        if(yari_inSight)
            cam.followobject_x = ((leftmostPataX + closest - 1800) / 2) * resRatioX;
        else
            cam.followobject_x = ((leftmostPataX + rightmostPataX - 300) / 2) * resRatioX;
    }

    cam.Work(mission_view);
    bg.pataSpeed = pataMaxSpeed;
    bg.Draw(cam);

    for(auto& entity : entities)
        entity->Draw();

    endflags.back().get()->global_y = 1485 + cam.zoom_y / zoom_offset;
    endflags.back().get()->Draw();

    if(debug && entities.size() > 0)
    {
        for(auto& entity : entities)
        {
            auto pos = sf::Vector2f(entity->global_x + entity->local_x + entity->hPos, entity->global_y + entity->local_y + entity->vPos + entity->cam_offset);

            for(auto hb : entity->getHitbox())
            {
                sf::RectangleShape hbx;
                hbx.setSize(sf::Vector2f(hb.width*resRatioX,hb.height*resRatioY));
                hbx.setFillColor(sf::Color(128,0,128,64));
                hbx.setPosition((pos.x+hb.left) * resRatioX, (pos.y+hb.top) * resRatioY);
                CoreManager::getInstance().getWindow()->draw(hbx);
            }
        }
    }

    for(auto& projectile : projectiles)
    {
        projectile->Draw();

        if(debug)
        {
            hbb.setPosition(projectile->tipX * resRatioX, projectile->tipY * resRatioY);
            SPDLOG_DEBUG("projectile at {} {}, tip at {} {}, hitbox at {} {}", projectile->xPos, projectile->yPos, projectile->tipX, projectile->tipY, hbb.getPosition().x, hbb.getPosition().y);
            CoreManager::getInstance().getWindow()->draw(hbb);
        }
    }

    // draw everything
    hatapons.back().get()->Draw();

    for(auto& pon : yaripons)
        pon->Draw();

    for(auto& di : droppeditems)
    {
        if(rightmostPataX > di->global_x + di->local_x + di->off_x && !di->pickedup)
        {
            di->Collect();
            dropped_ids.push_back(di->order_id);
        }

        di->off_y = cam.zoom_y / zoom_offset;
        di->Draw();
    }

    bg.DrawFloor();

    lastView = CoreManager::getInstance().getWindow()->getView();
    CoreManager::getInstance().getWindow()->setView(CoreManager::getInstance().getWindow()->getDefaultView());

    // UI elements

    // Unit thumbs
    sf::CircleShape thumb;
    thumb.setRadius(80 * resRatioX);
    thumb.setFillColor(sf::Color(224, 224, 224));
    thumb.setOrigin(thumb.getLocalBounds().width/2, thumb.getLocalBounds().height/2);

    PSprite& hpbar_out = ResourceManager::getInstance().getSprite("resources/graphics/mission/hpbar_back.png");
    hpbar_out.setOrigin(hpbar_out.getLocalBounds().width/2, hpbar_out.getLocalBounds().height/2);

    PSprite& hpbar_fill = ResourceManager::getInstance().getSprite("resources/graphics/mission/hpbar_ins.png");
    hpbar_fill.setOrigin(0, hpbar_fill.getLocalBounds().height/2);

    float thumbY = 250;

    thumb.setPosition(200*resRatioX, thumbY*resRatioY);
    CoreManager::getInstance().getWindow()->draw(thumb);

    hatapons.back().get()->main.animation.drawCopy(sf::Vector2f(221, thumbY-148), sf::Vector2f(0.6, 0.6));

    hpbar_out.setPosition(200, thumbY-120);
    hpbar_out.draw();

    hpbar_fill.setPosition(118, hpbar_out.getPosition().y);

    float hp = hatapons.back().get()->curHP / hatapons.back().get()->maxHP;

    if (hp > 0.70)
        hpbar_fill.setColor(sf::Color(0, 255, 0, 255));
    else if (hp > 0.35)
        hpbar_fill.setColor(sf::Color(245, 230, 66, 255));
    else
        hpbar_fill.setColor(sf::Color(212, 0, 0, 255));

    hpbar_fill.setScale(hp, 1);
    hpbar_fill.draw();

    if(yaripons.size() > 0)
    {
        thumb.setPosition(400*resRatioX, thumbY*resRatioY);
        CoreManager::getInstance().getWindow()->draw(thumb);

        yaripons.back().get()->main.animation.drawCopy(sf::Vector2f(400, thumbY), sf::Vector2f(0.6, 0.6));

        hpbar_out.setPosition(400, thumbY-120);
        hpbar_out.draw();

        hpbar_fill.setPosition(318, hpbar_out.getPosition().y);

        float yari_maxhp = 0;
        float yari_curhp = 0;
        for(auto& y : yaripons)
        {
            yari_maxhp += y->maxHP;
            yari_curhp += y->curHP;
        }

        hp = yari_curhp / yari_maxhp;

        if (hp > 0.70)
            hpbar_fill.setColor(sf::Color(0, 255, 0, 255));
        else if (hp > 0.35)
            hpbar_fill.setColor(sf::Color(245, 230, 66, 255));
        else
            hpbar_fill.setColor(sf::Color(212, 0, 0, 255));

        hpbar_fill.setScale(hp, 1);
        hpbar_fill.draw();

        unit_count_shadow.setFont(CoreManager::getInstance().getStrRepo()->GetFontNameForLanguage(CoreManager::getInstance().getStrRepo()->GetCurrentLanguage()));
        unit_count_shadow.disable_processing = true;
        unit_count_shadow.setCharacterSize(30);
        unit_count_shadow.setString(to_string(yaripons.size()));
        unit_count_shadow.setColor(sf::Color::Black);
        unit_count_shadow.setOrigin(0, unit_count_shadow.getLocalBounds().height/2);
        unit_count_shadow.setPosition(460, thumbY + 75);
        unit_count_shadow.draw();

        unit_count.setFont(CoreManager::getInstance().getStrRepo()->GetFontNameForLanguage(CoreManager::getInstance().getStrRepo()->GetCurrentLanguage()));
        unit_count.disable_processing = true;
        unit_count.setCharacterSize(30);
        unit_count.setString(to_string(yaripons.size()));
        unit_count.setColor(sf::Color::White);
        unit_count.setOrigin(0, unit_count.getLocalBounds().height/2);
        unit_count.setPosition(452, thumbY + 65);
        unit_count.draw();
    }

    // item thumbs
    int diid = 0;

    for(auto iid : dropped_ids)
    {
        int row = floor(diid/12);
        int col = diid%12;

        sf::CircleShape i_thumb;
        i_thumb.setRadius(80 * resRatioX);
        i_thumb.setFillColor(sf::Color(224, 224, 224));
        i_thumb.setOrigin(i_thumb.getLocalBounds().width/2, i_thumb.getLocalBounds().height/2);
        i_thumb.setPosition((3840-200-(180*col))*resRatioX, (250+(180*row))*resRatioY);
        CoreManager::getInstance().getWindow()->draw(i_thumb);

        Item* itemPtr = CoreManager::getInstance().getSaveReader()->itemReg.getItemByID(iid);

        PSprite& item = ResourceManager::getInstance().getSprite(std::format("resources/graphics/item/textures/{}/{:04}.png", itemPtr->spritesheet, itemPtr->spritesheet_id));
        item.setOrigin(item.getLocalBounds().width/2, item.getLocalBounds().height/2);
        item.setScale(0.65,0.65);
        item.setPosition(3840-200-(180*col), 250+(180*row));
        item.setRotation(0);
        item.setColor(sf::Color::White);
        item.draw();

        diid++;
    }

    // worm

    feverworms.back().get()->Draw();

    rhythmGUI->doVisuals(0, rhythm->GetCombo());

    if (!missionEnd)
    {
        if(dialogboxes.size() <= 0)
        {
            if (inputCtrl->isKeyPressed(Input::Keys::SELECT))
            {
                std::vector<sf::String> a = {"Toggle hitboxes", "Toggle debug info", "Heal units", "Kill all player units", "Kill Hatapon", "Mission failure", "Mission complete", "Toggle rhythm debug UI", "Toggle song debug"};

                PataDialogBox db;
                db.Create(font, "Debug menu", a, CoreManager::getInstance().getConfig()->GetInt("textureQuality"), 3);
                db.id = 999;
                dialogboxes.push_back(db);
            }

            if (inputCtrl->isKeyPressed(Input::Keys::START))
            {
                std::vector<sf::String> a = {"nav_yes", "nav_no"};

                PataDialogBox db;
                db.Create(font, "mission_backtopatapolis", a, CoreManager::getInstance().getConfig()->GetInt("textureQuality"));
                dialogboxes.push_back(db);
            }
        }
    }

    if(missionEnd)
        DoMissionEnd();

    inputCtrl->lockRhythm = false;

    SPDLOG_TRACE("Handle dialog boxes");
    if (dialogboxes.size() > 0)
    {
        inputCtrl->lockRhythm = true;

        if (inputCtrl->isKeyPressed(Input::Keys::CROSS))
        {
            switch (dialogboxes[dialogboxes.size() - 1].CheckSelectedOption())
            {
                case 0: {
                    if (dialogboxes[dialogboxes.size() - 1].id == 0)
                    {
                        SPDLOG_DEBUG("Return to Patapolis");
                        dialogboxes[dialogboxes.size() - 1].Close();

                        missionEnd = true;
                        failure = true;
                        rhythm->Stop();
                        missionEndTimer.restart();
                    } else if (dialogboxes[dialogboxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Toggle hitboxes");

                        for(auto& pon : yaripons)
                        {
                            pon->toggleDebug = !pon->toggleDebug;
                        }
                        debug = !debug;

                        dialogboxes[dialogboxes.size() - 1].Close();
                    }

                    break;
                }

                case 1: {
                    if (dialogboxes[dialogboxes.size() - 1].id == 0)
                    {
                        SPDLOG_DEBUG("Back to mission");
                        dialogboxes[dialogboxes.size() - 1].Close();
                    } else if (dialogboxes[dialogboxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Toggle debug info");

                        for(auto& entity : entities)
                        {
                            entity->toggleDebug = !entity->toggleDebug;
                        }

                        dialogboxes[dialogboxes.size() - 1].Close();
                    }

                    break;
                }

                case 2: {
                    if (dialogboxes[dialogboxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Heal all units");
                        for(auto& pon : yaripons)
                        {
                            pon->curHP = pon->maxHP;
                        }

                        dialogboxes[dialogboxes.size() - 1].Close();
                    }

                    break;
                }

                case 3: {
                    if (dialogboxes[dialogboxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Kill all player units");

                        for(auto& pon : yaripons)
                        {
                            pon->curHP = 0;
                        }

                        dialogboxes[dialogboxes.size() - 1].Close();
                    }

                    break;
                }

                case 4: {
                    if (dialogboxes[dialogboxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Kill Hatapon");

                        for(auto& pon : hatapons)
                        {
                            pon->curHP = 0;
                        }

                        dialogboxes[dialogboxes.size() - 1].Close();
                    }

                    break;
                }

                case 5: {
                    if (dialogboxes[dialogboxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Deprecated: verbose logging");

                        missionEnd = true;
                        failure = true;
                        rhythm->Stop();
                        missionEndTimer.restart();

                        dialogboxes[dialogboxes.size() - 1].Close();
                    }

                    break;
                }

                case 6: {
                    if (dialogboxes[dialogboxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Mission complete");

                        missionEnd = true;
                        rhythm->Stop();
                        missionEndTimer.restart();

                        dialogboxes[dialogboxes.size() - 1].Close();
                    }

                    break;
                }

                case 7: {
                    if (dialogboxes[dialogboxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Toggle rhythm debug");

                        CoreManager::getInstance().getRhythmGUI()->toggleDebugUI();
                        
                        dialogboxes[dialogboxes.size() - 1].Close();
                    }

                    break;
                }

                case 8: {
                    if (dialogboxes[dialogboxes.size() - 1].id == 999)
                    {
                        SPDLOG_DEBUG("Toggle song debug");

                        CoreManager::getInstance().getRhythm()->toggleDebug();
                        
                        dialogboxes[dialogboxes.size() - 1].Close();
                    }

                    break;
                }
            }
        }
    }

    vector<int> db_e; ///dialog box erase

    for (int i = 0; i < dialogboxes.size(); i++)
    {
        dialogboxes[i].x = 1920;
        dialogboxes[i].y = 1080;
        dialogboxes[i].Draw();

        if (dialogboxes[i].closed)
            db_e.push_back(i);
    }

    for (int i = 0; i < db_e.size(); i++)
    {
        dialogboxes.erase(dialogboxes.begin() + db_e[i] - i);
    }

    CoreManager::getInstance().getWindow()->setView(lastView);

    if((yaripons.size() == 0 || hatapons.back().get()->curHP <= 0) && !missionEnd)
    {
        missionEnd = true;
        missionEndTimer.restart();
        failure = true;
        rhythm->Stop();
    }

    if (returnToPatapolis)
    {
        StateManager::getInstance().setState(StateManager::PATAPOLIS);
    }
}

MissionController::~MissionController()
{

}
