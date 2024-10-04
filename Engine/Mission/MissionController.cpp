#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "MissionController.h"
#include "../CoreManager.h"
#include "../StateManager.h"

using json = nlohmann::json;

MissionController::MissionController()
{
    CoreManager::getInstance().reinitSongController();
    CoreManager::getInstance().getSongController()->LoadTheme("ahwoon");

    bg.Load("shidavalley");
    
    initialized = true;
}

void MissionController::Update()
{
    Rhythm* rhythm = CoreManager::getInstance().getRhythm();

    rhythm->doRhythm();

    auto view = CoreManager::getInstance().getWindow()->getView();

    cam.Work(view);
    bg.Draw(cam);

    CoreManager::getInstance().getRhythmGUI()->doVisuals(0, rhythm->GetCombo());
}

MissionController::~MissionController()
{

}
