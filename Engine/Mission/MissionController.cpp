#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "MissionController.h"
#include "../CoreManager.h"
#include "../StateManager.h"

using json = nlohmann::json;
using namespace std::chrono;

MissionController::MissionController()
{
    hatapon.LoadConfig("resources/units/unit/hatapon.zip");
    hatapon.setAnimation("idle");

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
            if(message.message == "65109") // PATA PATA PATA PON
            {
                hatapon.setAnimation("wave");

                advance = true;
                pataMaxSpeed = 400;
                advanceClock.restart();
            }
            else if(message.message == "6") // PATA PATA PATA PON
            {
                hatapon.setAnimation("wave");

                advance = true;
                pataMaxSpeed = 4000;
                advanceClock.restart();
            }
        }
    }

    if(advance)
    {
        pataSpeed += pataMaxSpeed*accelerationFactor / fps;
        if(pataSpeed > pataMaxSpeed)
            pataSpeed = pataMaxSpeed;

        if(advanceClock.getElapsedTime().asMilliseconds() > 2000)
        {
            hatapon.setAnimation("idle");
            advance = false;
        }
    }
    else
    {
        pataSpeed -= pataMaxSpeed*decelerationFactor / fps;
        if(pataSpeed < 0)
            pataSpeed = 0;
    }

    followPoint += pataSpeed / fps;

    //SPDLOG_DEBUG("PataSpeed: {}, PataMaxSpeed: {}, followPoint: {}", pataSpeed, pataMaxSpeed, followPoint);

    auto view = CoreManager::getInstance().getWindow()->getView();

    cam.followobject_x = followPoint;
    cam.Work(view);
    bg.pataSpeed = pataMaxSpeed / 300;
    bg.Draw(cam);

    hatapon.setGlobalPosition(sf::Vector2f(followPoint*3 - 400, 1490));
    hatapon.Draw();

    if(inputCtrl->isKeyPressed(Input::Keys::UP))
        rhythmGUI->toggleDebugUI();

    rhythmGUI->doVisuals(0, rhythm->GetCombo());
}

MissionController::~MissionController()
{

}
