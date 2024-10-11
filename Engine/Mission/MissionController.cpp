#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "MissionController.h"
#include "../CoreManager.h"
#include "../StateManager.h"

#include <fstream>

using json = nlohmann::json;
using namespace std::chrono;

MissionController::MissionController()
{

}

void MissionController::LoadMission(const std::string& path)
{
    SPDLOG_INFO("Attempting to load {}", path);

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

    int pons = CoreManager::getInstance().getConfig()->GetInt("yaripons");

    for(int i=1; i<=pons; i++)
    {
        yaripons.push_back(std::make_unique<Yaripon>(i, pons));
    }

    if(mission.contains("entities"))
    {
        int en_c = 0;

        for(auto entity : mission["entities"])
        {
            std::string p = entity["path"];

            entities.push_back(std::make_unique<Entity>());
            auto e = entities.back().get();
            e->LoadEntity(p);

            for(auto param : entity["params"].items())
            {
                if(param.key() == "xpos")
                    e->setGlobalPosition(sf::Vector2f(param.value(),e->yPos));

                if(param.key() == "color")
                    e->setColor(Func::hexToColor(param.value()));

                if(param.key() == "hp")
                {
                    e->maxHP = param.value();
                    e->curHP = e->maxHP;
                }
            }

            // TODO: add stat overrides
            //entities.back().get()->setAnimation("idle");
            e->orderID = en_c;

            en_c++;
        }
    }

    bg.Load(background);

    CoreManager::getInstance().reinitSongController();
    CoreManager::getInstance().getSongController()->LoadTheme(bgm);
    CoreManager::getInstance().getRhythm()->LoadTheme(bgm);

    lastRhythmCheck = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    //ExecuteZoom(0.999, 1000);
    mission_view.setSize(sf::Vector2f(CoreManager::getInstance().getWindow()->getSize().x, CoreManager::getInstance().getWindow()->getSize().y));

    initialized = true;
}

void MissionController::SendProjectile(float x, float y, float hspeed, float vspeed)
{
    projectiles.push_back(std::make_unique<Projectile>("resources/graphics/item/textures/main/0014.png", x, y, hspeed, vspeed));
    SPDLOG_DEBUG("SENDING PROJECTILE AT {} {} {} {}", x, y, hspeed, vspeed);
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

void MissionController::Update()
{
    Rhythm* rhythm = CoreManager::getInstance().getRhythm();
    RhythmGUI* rhythmGUI = CoreManager::getInstance().getRhythmGUI();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();

    float fps = CoreManager::getInstance().getCore()->getFPS();

    // TODO: why the fuck is this needed? without this, zoom doesn't work correctly.
    double zoom_offset = (0.000709722222222 * CoreManager::getInstance().getWindow()->getSize().y);

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
        }

        if(action == Rhythm::RhythmAction::FEVER_ON)
        {
            inFever = true;
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
        if(entity->getGlobalPosition().x < closest && (entity->entityType == Entity::EntityTypes::HOSTILE || entity->entityType == Entity::EntityTypes::NEUTRAL))
            closest = entity->getGlobalPosition().x;
    }

    // if farthest yaripon sees enemy, whole army shall be angry
    float yari_distance = closest - yaripons.back().get()->global_x - yaripons.back().get()->local_x - yaripons.back().get()->attack_x - yaripons.back().get()->gap_x;
    bool yari_inSight = false;

    if(yari_distance < 3000 && !entities.empty())
    {
        yari_inSight = true;
        ExecuteZoom(1.0007, 1.2);
    }
    else
    {
        ExecuteZoom(0.9993, 0.8);
    }

    for(auto& pon : yaripons)
    {
        pon->global_x = followPoint*3 - 240;
        pon->global_y = 1740 + cam.zoom_y / zoom_offset; // ,/0.511 dla 720p ,/ 1.533 dla 2160p
        pon->closestEnemyX = closest;
        pon->enemyInSight = yari_inSight;
    }

    for(auto& entity : entities)
    {
        entity->setGlobalPosition(sf::Vector2f(entity->getGlobalPosition().x, entity->yPos + cam.zoom_y / zoom_offset));
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

    for(auto& projectile : projectiles)
    {
        projectile->Update();

        for(auto& entity : entities)
        {
            if(entity->entityType == Entity::EntityTypes::HOSTILE || entity->entityType == Entity::EntityTypes::NEUTRAL)
            {
                auto pos = entity->getGlobalPosition();

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

    if(yari_inSight)
        cam.followobject_x = ((leftmostPataX + closest - 1800) / 2) * resRatioX;
    else
        cam.followobject_x = ((leftmostPataX + rightmostPataX - 300) / 2) * resRatioX;

    cam.Work(mission_view);
    bg.pataSpeed = pataMaxSpeed;
    bg.Draw(cam);

    for(auto& entity : entities)
        entity->Draw();

    if(debug && entities.size() > 0)
    {
        for(auto& entity : entities)
        {
            auto pos = entity->getGlobalPosition();

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

    if(inputCtrl->isKeyPressed(Input::Keys::UP))
    {
        rhythmGUI->toggleDebugUI();
        for(auto& pon : yaripons)
        {
            pon->toggleDebug = !pon->toggleDebug;
        }
        for(auto& entity : entities)
        {
            entity->toggleDebug = !entity->toggleDebug;
        }
        debug = !debug;
    }

    bg.DrawFloor();

    lastView = CoreManager::getInstance().getWindow()->getView();
    CoreManager::getInstance().getWindow()->setView(CoreManager::getInstance().getWindow()->getDefaultView());

    // UI elements
    sf::CircleShape thumb;
    thumb.setRadius(80 * resRatioX);
    thumb.setFillColor(sf::Color::White);
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

    hpbar_fill.setPosition(hpbar_out.getPosition().x - hpbar_out.getLocalBounds().width - 22, hpbar_out.getPosition().y);

    float hp = hatapons.back().get()->curHP / hatapons.back().get()->maxHP;

    if (hp > 0.70)
        hpbar_fill.setColor(sf::Color(0, 255, 0, 255));
    else if (hp > 0.35)
        hpbar_fill.setColor(sf::Color(245, 230, 66, 255));
    else
        hpbar_fill.setColor(sf::Color(212, 0, 0, 255));

    hpbar_fill.draw();

    thumb.setPosition(400*resRatioX, thumbY*resRatioY);
    CoreManager::getInstance().getWindow()->draw(thumb);

    yaripons.back().get()->main.animation.drawCopy(sf::Vector2f(400, thumbY), sf::Vector2f(0.6, 0.6));

    hpbar_out.setPosition(400, thumbY-120);
    hpbar_out.draw();

    hpbar_fill.setPosition(hpbar_out.getPosition().x - hpbar_out.getLocalBounds().width - 22, hpbar_out.getPosition().y);

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


    CoreManager::getInstance().getWindow()->setView(lastView);

    rhythmGUI->doVisuals(0, rhythm->GetCombo());

    CoreManager::getInstance().getWindow()->setView(lastView);
}

MissionController::~MissionController()
{

}
