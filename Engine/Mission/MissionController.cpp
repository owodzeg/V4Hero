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

    for(int i=1; i<=pons; i++)
    {
        yaripons.push_back(std::make_unique<Yaripon>(i, pons));
    }

    obstacle.LoadConfig("resources/units/entity/kirajin.zip");
    obstacle.setAnimation("idle");

    bg.Load("shidavalley");

    CoreManager::getInstance().reinitSongController();
    CoreManager::getInstance().getSongController()->LoadTheme("ahwoon");
    CoreManager::getInstance().getRhythm()->LoadTheme("ahwoon");

    lastRhythmCheck = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    
    initialized = true;
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
            if(message.message == "65109" || message.message == "6") // PATA PATA PATA PON
            {
                hatapons.back().get()->Advance();

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
                    yaripon->Attack(1);
                    advanceClock.restart();
                }
            }
        }

        if(action == Rhythm::RhythmAction::COMBO_BREAK)
        {
            hatapons.back().get()->StopAll();

            for(auto& yaripon : yaripons)
            {
                yaripon->StopAll();
            }

            advance = false;
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

    cam.followobject_x = (followPoint*3 - 500) * resRatioX;
    cam.Work(view);
    bg.pataSpeed = pataMaxSpeed / 900 * resRatioX;
    bg.Draw(cam);

    // Update positions
    hatapons.back().get()->global_x = followPoint*3 - 400;
    hatapons.back().get()->global_y = 1490;
    hatapons.back().get()->Draw();

    // if farthest yaripon sees enemy, whole army shall be angry
    float yari_distance = yaripons.back().get()->closestEnemyX - yaripons.back().get()->global_x - yaripons.back().get()->local_x - yaripons.back().get()->attack_x;
    bool yari_inSight = false;

    if(yari_distance < 4000)
    {
        yari_inSight = true;
    }

    for(auto& pon : yaripons)
    {
        pon->global_x = followPoint*3 - 240;
        pon->global_y = 1730;
        pon->closestEnemyX = 4000;
        pon->enemyInSight = yari_inSight;
        pon->Draw();
    }

    obstacle.setGlobalPosition(sf::Vector2f(4000, 1600));
    obstacle.Draw();

    if(inputCtrl->isKeyPressed(Input::Keys::UP))
        rhythmGUI->toggleDebugUI();

    rhythmGUI->doVisuals(0, rhythm->GetCombo());
}

MissionController::~MissionController()
{

}
