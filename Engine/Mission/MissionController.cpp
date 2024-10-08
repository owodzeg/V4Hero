#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "MissionController.h"
#include "../CoreManager.h"
#include "../StateManager.h"

using json = nlohmann::json;
using namespace std::chrono;

MissionController::MissionController()
{
    hatapons.push_back(std::make_unique<Hatapon>());

    int pons = CoreManager::getInstance().getConfig()->GetInt("yaripons");
    kirajin_hp = pons*100;

    for(int i=1; i<=pons; i++)
    {
        yaripons.push_back(std::make_unique<Yaripon>(i, pons));
    }

    obstacles.push_back(std::make_unique<AnimatedObject>());
    obstacles.back().get()->LoadConfig("resources/units/entity/kirajin.zip");
    obstacles.back().get()->setAnimation("idle");

    bg.Load("shidavalley");
    std::string theme = "ahwoon";

    CoreManager::getInstance().reinitSongController();
    CoreManager::getInstance().getSongController()->LoadTheme(theme);
    CoreManager::getInstance().getRhythm()->LoadTheme(theme);

    lastRhythmCheck = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    
    initialized = true;
}

void MissionController::SendProjectile(float x, float y, float hspeed, float vspeed)
{
    projectiles.push_back(std::make_unique<Projectile>("resources/graphics/item/textures/main/0014.png", x, y, hspeed, vspeed));
    SPDLOG_DEBUG("SENDING PROJECTILE AT {} {} {} {}", x, y, hspeed, vspeed);
}

void MissionController::Update()
{
    Rhythm* rhythm = CoreManager::getInstance().getRhythm();
    RhythmGUI* rhythmGUI = CoreManager::getInstance().getRhythmGUI();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();

    float fps = CoreManager::getInstance().getCore()->getFPS();

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
            if(yaripon->action == 1) // attack
                yaripon->StopAttack();
        }

        advance = false;
    }

    followPoint += pataSpeed / fps;

    //SPDLOG_DEBUG("PataSpeed: {}, PataMaxSpeed: {}, followPoint: {}", pataSpeed, pataMaxSpeed, followPoint);

    auto view = CoreManager::getInstance().getWindow()->getView();

    float resRatioX = CoreManager::getInstance().getWindow()->getSize().x / float(3840);
    float resRatioY = CoreManager::getInstance().getWindow()->getSize().y / float(2160);

    cam.followobject_x = (followPoint*3 - 500) * resRatioX;
    cam.Work(view);
    bg.pataSpeed = pataMaxSpeed;
    bg.Draw(cam);

    // Update positions
    hatapons.back().get()->global_x = followPoint*3 - 400;
    hatapons.back().get()->global_y = 1490;
    hatapons.back().get()->Draw();

    float closest = 9999999;

    if(obstacles.size() > 0)
    {
        closest = obstacles.back().get()->getGlobalPosition().x;
    }

    // if farthest yaripon sees enemy, whole army shall be angry
    float yari_distance = yaripons.back().get()->closestEnemyX - yaripons.back().get()->global_x - yaripons.back().get()->local_x - yaripons.back().get()->attack_x - yaripons.back().get()->gap_x;
    bool yari_inSight = false;

    if(yari_distance < 4000)
    {
        yari_inSight = true;
    }

    for(auto& pon : yaripons)
    {
        pon->global_x = followPoint*3 - 240;
        pon->global_y = 1740;
        pon->closestEnemyX = closest;
        pon->enemyInSight = yari_inSight;
        pon->Draw();
    }

    if(obstacles.size() > 0)
    {
        obstacles.back().get()->setGlobalPosition(sf::Vector2f(4000, 1735));
        obstacles.back().get()->Draw();
    }

    sf::RectangleShape hb;
    hb.setSize(sf::Vector2f(10,10));
    hb.setFillColor(sf::Color::Green);

    projectiles.erase(
        std::remove_if(
            projectiles.begin(),
            projectiles.end(),
            [](const std::unique_ptr<Projectile>& projectile) {
                return projectile->finished; // Check if the projectile hits
            }
        ),
        projectiles.end() // This removes the "removed" projectiles
    );

    for(auto& projectile : projectiles)
    {
        projectile->Update();
        projectile->Draw();

        if(debug)
        {
            hb.setPosition(projectile->tipX * resRatioX, projectile->tipY * resRatioY);
            SPDLOG_DEBUG("projectile at {} {}, tip at {} {}, hitbox at {} {}", projectile->xPos, projectile->yPos, projectile->tipX, projectile->tipY, hb.getPosition().x, hb.getPosition().y);
            CoreManager::getInstance().getWindow()->draw(hb);
        }

        if(obstacles.size() > 0)
        {
            if(projectile->tipX > obstacles.back().get()->getGlobalPosition().x-60 && projectile->tipX < obstacles.back().get()->getGlobalPosition().x+60)
            {
                if(projectile->tipY > obstacles.back().get()->getGlobalPosition().y-60 && projectile->tipY < obstacles.back().get()->getGlobalPosition().y+60)
                {
                    projectile->finished = true;
                    kirajin_hp -= rand() % 20 + 10;
                    SPDLOG_DEBUG("kirajin got hit! {} HP left", kirajin_hp);
                    obstacles.back().get()->setAnimation("stagger");
                    obstacles.back().get()->restartAnimation();
                }
            }
        }
    }

    if(debug && obstacles.size() > 0)
    {
        sf::RectangleShape hbx;
        hbx.setSize(sf::Vector2f(120*resRatioX,120*resRatioY));
        hbx.setFillColor(sf::Color(128,0,128,64));
        hbx.setPosition((obstacles.back().get()->getGlobalPosition().x-60) * resRatioX, (obstacles.back().get()->getGlobalPosition().y-60) * resRatioY);
        CoreManager::getInstance().getWindow()->draw(hbx);
    }

    if(kirajin_hp <= 0 && obstacles.size() > 0)
    {
        obstacles.clear();
    }

    if(inputCtrl->isKeyPressed(Input::Keys::UP))
    {
        rhythmGUI->toggleDebugUI();
        for(auto& pon : yaripons)
        {
            pon->toggleDebug = !pon->toggleDebug;
        }
        debug = !debug;
    }

    rhythmGUI->doVisuals(0, rhythm->GetCombo());
}

MissionController::~MissionController()
{

}
