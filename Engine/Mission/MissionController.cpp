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

    wall.LoadConfig(&config);
    tangibleLevelObjects.push_back(&wall);
    int quality = config.GetInt("textureQuality");

    float ratioX, ratioY;
    switch(quality)
    {
        case 0: ///low
        {
            ratioX = config.GetInt("resX") / float(640);
            ratioY = config.GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = config.GetInt("resX") / float(1280);
            ratioY = config.GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = config.GetInt("resX") / float(1920);
            ratioY = config.GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = config.GetInt("resX") / float(3840);
            ratioY = config.GetInt("resY") / float(2160);
            break;
        }
    }

    pataponY = config.GetInt("resY") - (200 * ratioY);
    patapon.scaleX = ratioX;
    patapon.scaleY = ratioY;

    wallY = config.GetInt("resY") - (278 * ratioY);
    wall.scaleX = ratioX;
    wall.scaleY = ratioY;
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


        wall.x = 1000;
        wall.y = wallY;
        wall.fps = fps;

        /// here we show the hitbox
        bool showHitboxes = false;
        if(showHitboxes){
            sf::RectangleShape hitboxRect(sf::Vector2f(patapon.hitBox.width, patapon.hitBox.height));
            hitboxRect.setPosition(patapon.x+patapon.hitBox.left,patapon.y+patapon.hitBox.top);
            window.draw(hitboxRect);

            sf::RectangleShape wallHitboxRect(sf::Vector2f(wall.hitBox.width, wall.hitBox.height));
            wallHitboxRect.setPosition(wall.x+wall.hitBox.left,wall.y+wall.hitBox.top);
            window.draw(wallHitboxRect);
        }

        /** Make Patapon walk (temporary) **/
        if(camera.walk)
        {
            float proposedXPos = camera.followobject_x + (2 * 60) / fps;
            /// use the right hand side of the patapon sprite to check for collisions. This should be changed if the patapon walks to the left
            float proposedXPosRight = proposedXPos + patapon.hitBox.left + patapon.hitBox.width;
            /// need to have it check for collision and stop if blocked by wall here.

            /// right now it is very basic checking only in X axis. Jumping over a
            /// wall will not be possible.

            bool foundCollision = false;

            for(int i=0;i<tangibleLevelObjects.size();i++){
                Wall currentCollisionRect = *tangibleLevelObjects[i];
                /// if the new x position after moving will be between left side of wall and right side of wall
                if (proposedXPosRight>currentCollisionRect.x+wall.hitBox.left && proposedXPosRight<currentCollisionRect.x+wall.hitBox.left+currentCollisionRect.width){
                    /// then we have found a collision
                    foundCollision = true;
                    std::cout << "[COLLISION_SYSTEM]: Found a collision"<<endl;
                }
            }

            /// if the new position is inside a wall, don't move. If we found anything,
            if (!foundCollision){
                camera.followobject_x = proposedXPos;
            }
        }

        patapon.x = camera.followobject_x;
        patapon.y = pataponY;
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


        wall.Draw(window);
        /// patapons (and other enemies) are drawn after level objects like wall so they are always on top
        patapon.Draw(window);


        rhythm.fps = fps;
        ///ugh this is a BAD solution i need to do it differently

        rhythm.Draw(window);

}
MissionController::~MissionController()
{
    //dtor

}
