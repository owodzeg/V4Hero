#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "MissionController.h"
#include "../CoreManager.h"
#include "../StateManager.h"

using json = nlohmann::json;

MissionController::MissionController()
{
    hatapon.LoadConfig("resources/units/unit/hatapon.zip");

    bg.Load("shidavalley");

    CoreManager::getInstance().reinitSongController();
    CoreManager::getInstance().getSongController()->LoadTheme("ahwoon");
    CoreManager::getInstance().getRhythm()->LoadTheme("ahwoon");

    
    initialized = true;
}

void MissionController::Update()
{
    Rhythm* rhythm = CoreManager::getInstance().getRhythm();
    RhythmGUI* rhythmGUI = CoreManager::getInstance().getRhythmGUI();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();

    rhythm->doRhythm();

    auto view = CoreManager::getInstance().getWindow()->getView();

    cam.Work(view);
    bg.Draw(cam);

    hatapon.setGlobalPosition(sf::Vector2f(800, 1490));
    hatapon.Draw();

    if(inputCtrl->isKeyPressed(Input::Keys::UP))
        rhythmGUI->toggleDebugUI();

    rhythmGUI->doVisuals(0, rhythm->GetCombo());
}

MissionController::~MissionController()
{

}
