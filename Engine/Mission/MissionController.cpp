#include "MissionController.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
MissionController::MissionController()
{
    //ctor

}
void MissionController::Initialise(Config &config, std::map<int,bool> &keyMap,std::string backgroundString){
    test_bg.Load(backgroundString, config);//config.GetString("debugBackground"));

    missionKeyMap = &keyMap;
    missionConfig = &config;
    patapon.LoadConfig(&config);
}
void MissionController::StartMission(std::string songName){
    rhythm.LoadTheme(songName); // missionConfig->GetString("debugTheme")
}
void MissionController::StopMission(){
    rhythm.Stop();
}
void MissionController::Update(sf::RenderWindow &window, float fps){
        if(rhythm.current_song == "patapata")
        {
            camera.walk = true;
        }
        else
        {
            camera.walk = false;
        }

        camera.Work(window,fps);
        test_bg.setCamera(camera);
        test_bg.Draw(window);
        patapon.x = camera.followobject_x;
        patapon.fps = fps;

        // TODO: at some point some pointer shenanigans is required to make these be a reference to v4core's ones too.
        rhythm.rhythmController.keyMap = *missionKeyMap;
        rhythm.rhythmController.config = *missionConfig;
        rhythm.config = *missionConfig;

        if(rhythm.current_song == "patapata")
        {
            patapon.current_animation = "walk";
        }

        if((rhythm.rhythmController.current_drum == "pata") or (rhythm.rhythmController.current_drum == "pon") or (rhythm.rhythmController.current_drum == "chaka") or (rhythm.rhythmController.current_drum == "don"))
        {
            patapon.current_animation = rhythm.rhythmController.current_drum;
            patapon.current_frame = 0;
            rhythm.rhythmController.current_drum = "";
            rhythm.current_song = "";
        }

        patapon.Draw(window);
        rhythm.fps = fps;
        ///ugh this is a BAD solution i need to do it differently

        rhythm.Draw(window);

}
MissionController::~MissionController()
{
    //dtor

}
