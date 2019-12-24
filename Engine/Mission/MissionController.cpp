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

    //ctor
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    //t_cutscene_text.setFont(f_font);

    //t_cutscene_text.setCharacterSize(35);
    //t_cutscene_text.setFillColor(sf::Color::White);
    //t_cutscene_text.setString(Func::ConvertToUtf8String(config.strRepo.GetUnicodeString(L"intro_cutscene_1")));
    //t_cutscene_text.setOrigin(t_cutscene_text.getGlobalBounds().width/2,t_cutscene_text.getGlobalBounds().height/2);

    missionKeyMap = &keyMap;
    missionConfig = &config;
    patapon.LoadConfig(&config);

    kacheek.LoadConfig(&config);
    kacheek2.LoadConfig(&config);
    kacheek3.LoadConfig(&config);
    endFlag1.LoadConfig(&config);
    tangibleLevelObjects.push_back(&kacheek);
    tangibleLevelObjects.push_back(&kacheek2);
    tangibleLevelObjects.push_back(&kacheek3);
    tangibleLevelObjects.push_back(&endFlag1);
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

    kacheek.scaleX = ratioX;
    kacheek.scaleY = ratioY;
    kacheek2.scaleX = ratioX;
    kacheek2.scaleY = ratioY;
    kacheek3.scaleX = ratioX;
    kacheek3.scaleY = ratioY;
    endFlag1.scaleX = ratioX*0.2;
    endFlag1.scaleY = ratioY*0.2;

    kacheek.x = 1000;
    kacheek.y = config.GetInt("resY") - (250 * ratioY);
    kacheek2.x = 1500;
    kacheek2.y = config.GetInt("resY") - (250 * ratioY);
    kacheek3.x = 2000;
    kacheek3.y = config.GetInt("resY") - (250 * ratioY);
    endFlag1.x = 2500;
    endFlag1.y = config.GetInt("resY") - (250 * ratioY);
    isInitialized = true;
    // this is outside the loop
    startAlpha = 255;
    endAlpha = 0;
    targetTime = sf::seconds(10);


    fade.setPosition(sf::Vector2f(0,0));
    fade.setFillColor(sf::Color(0,0,0,0));
    fade.setSize(sf::Vector2f(800,600));
    currentCutsceneId=0;
}
void MissionController::StartMission(std::string songName,int missionID,bool showCutscene){
    rhythm.LoadTheme(songName); // missionConfig->GetString("debugTheme")

    int quality = missionConfig->GetInt("textureQuality");
    float ratioX, ratioY;
    switch(quality)
    {
        case 0: ///low
        {
            ratioX = missionConfig->GetInt("resX") / float(640);
            ratioY = missionConfig->GetInt("resY") / float(360);
            break;
        }

        case 1: ///med
        {
            ratioX = missionConfig->GetInt("resX") / float(1280);
            ratioY = missionConfig->GetInt("resY") / float(720);
            break;
        }

        case 2: ///high
        {
            ratioX = missionConfig->GetInt("resX") / float(1920);
            ratioY = missionConfig->GetInt("resY") / float(1080);
            break;
        }

        case 3: ///ultra
        {
            ratioX = missionConfig->GetInt("resX") / float(3840);
            ratioY = missionConfig->GetInt("resY") / float(2160);
            break;
        }
    }
    if(showCutscene){
        cutscene_text_identifiers.push_back(L"intro_cutscene_1");
        cutscene_text_identifiers.push_back(L"intro_cutscene_2");
        cutscene_text_identifiers.push_back(L"intro_cutscene_3");
        cutscene_text_identifiers.push_back(L"intro_cutscene_4");
        cutscene_text_identifiers.push_back(L"intro_cutscene_5");
        cutscene_lengths.push_back(4);
        cutscene_lengths.push_back(4);
        cutscene_lengths.push_back(3);
        cutscene_lengths.push_back(3);
        cutscene_lengths.push_back(2);
        cutscene_blackscreens.push_back(true);
        cutscene_blackscreens.push_back(true);
        cutscene_blackscreens.push_back(true);
        cutscene_blackscreens.push_back(true);
        cutscene_blackscreens.push_back(true);
        currentCutsceneId=0;
        cutscenesLeft=true;
        isFinishedLoading=true;

    } else {
        inCutscene = false;
        cutscene_blackscreens.clear();
        cutscene_lengths.clear();
        cutscene_text_identifiers.clear();
        cutscenesLeft=false;
    }
    tangibleLevelObjects.clear();
    switch(missionID){
    case 1:{

        endFlag1.LoadConfig(missionConfig);

        tangibleLevelObjects.push_back(&endFlag1);

        endFlag1.scaleX = ratioX*0.2;
        endFlag1.scaleY = ratioY*0.2;

        endFlag1.x = 2500;
        endFlag1.y = missionConfig->GetInt("resY") - (250 * ratioY);
        break;
    }
    case 2:{
        kacheek.LoadConfig(missionConfig);
        kacheek2.LoadConfig(missionConfig);
        kacheek3.LoadConfig(missionConfig);
        endFlag1.LoadConfig(missionConfig);
        tangibleLevelObjects.push_back(&kacheek);
        tangibleLevelObjects.push_back(&kacheek2);
        tangibleLevelObjects.push_back(&kacheek3);
        tangibleLevelObjects.push_back(&endFlag1);
        kacheek.scaleX = ratioX;
        kacheek.scaleY = ratioY;
        kacheek2.scaleX = ratioX;
        kacheek2.scaleY = ratioY;
        kacheek3.scaleX = ratioX;
        kacheek3.scaleY = ratioY;
        endFlag1.scaleX = ratioX*0.2;
        endFlag1.scaleY = ratioY*0.2;
        kacheek.x = 1000;
        kacheek.y = missionConfig->GetInt("resY") - (250 * ratioY);
        kacheek2.x = 1500;
        kacheek2.y = missionConfig->GetInt("resY") - (250 * ratioY);
        kacheek3.x = 2000;
        kacheek3.y = missionConfig->GetInt("resY") - (250 * ratioY);
        endFlag1.x = 2500;
        endFlag1.y = missionConfig->GetInt("resY") - (250 * ratioY);
        break;

    }
    default:

        endFlag1.LoadConfig(missionConfig);

        tangibleLevelObjects.push_back(&endFlag1);
        endFlag1.scaleX = ratioX*0.2;
        endFlag1.scaleY = ratioY*0.2;
        endFlag1.x = 2500;
        endFlag1.y = missionConfig->GetInt("resY") - (250 * ratioY);
        break;
    }
    isFinishedLoading=true;
}
void MissionController::StopMission(){
    rhythm.Stop();
    isInitialized = false;
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

        kacheek.fps = fps;
        kacheek2.fps = fps;
        kacheek3.fps = fps;
        endFlag1.fps = fps;

        /** Make Patapon walk (temporary) **/
        if(camera.walk)
        {
            float proposedXPos = camera.followobject_x + (2 * 60) / fps;
            /// use the right hand side of the patapon sprite to check for collisions. This should be changed if the patapon walks to the left
            float proposedXPosRight = proposedXPos + patapon.hitBox.left + patapon.hitBox.width;
            /// need to have it check for collision and stop if blocked by kacheek here.

            /// right now it is very basic checking only in X axis. Jumping over a
            /// kacheek will not be possible.

            bool foundCollision = false;

            for(int i=0;i<tangibleLevelObjects.size();i++){
                //kacheek currentCollisionRect = *tangibleLevelObjects[i];
                /// if the new x position after moving will be between left side of kacheek and right side of kacheek
                if (proposedXPosRight>tangibleLevelObjects[i]->x+kacheek.hitBox.left && proposedXPosRight<tangibleLevelObjects[i]->x+kacheek.hitBox.left+tangibleLevelObjects[i]->width){
                    /// then we have found a collision
                    foundCollision = true;
                    tangibleLevelObjects[i]->OnCollide(*tangibleLevelObjects[i]);
                    std::cout << "[COLLISION_SYSTEM]: Found a collision"<<endl;
                    /*///HARDCODED FOR KACHEEK SHOWCASE PURPOSES
                    tangibleLevelObjects[i]->walk_timer.restart();

                    if(tangibleLevelObjects[i]->current_animation != "walk")
                    {
                        tangibleLevelObjects[i]->current_animation = "walk";
                        tangibleLevelObjects[i]->current_frame = 0;
                    }*/
                }
            }

            /// if the new position is inside a kacheek, don't move. If we found anything,
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

        for (int i=0;i<tangibleLevelObjects.size();i++){
            tangibleLevelObjects[i]->Draw(window);
        }
        //kacheek.Draw(window);
        //kacheek2.Draw(window);
        //kacheek3.Draw(window);
        //endFlag1.Draw(window);

        /// patapons (and other enemies) are drawn after level objects like kacheek so they are always on top
        patapon.Draw(window);

        /// draw static UI elements
        auto lastView = window.getView();

        window.setView(window.getDefaultView());


        if(cutscenesLeft && !inCutscene && isMoreCutscenes()){
            StartCutscene(cutscene_text_identifiers[currentCutsceneId],cutscene_blackscreens[currentCutsceneId],cutscene_lengths[currentCutsceneId]);
        }

        sf::Time currentTime = timer.getElapsedTime();
        int currentAlpha = startAlpha;
        if (currentTime >= targetTime && inCutscene)
        {
            // oops: currentAlpha = endAlpha; // make certain that the alpha is at its final destination
            //you are done
            if(!isMoreCutscenes()){
                currentAlpha = startAlpha;
                inCutscene = false;
                if(isBlackScreenCutscene){
                    inFadeTransition = true;
                    timer.restart();
                    targetTime = sf::seconds(2);
                } else {
                    FinishLastCutscene();
                }
                cutscenesLeft=false;
            } else {
                /// next cutscene
                currentCutsceneId++;
                StartCutscene(cutscene_text_identifiers[currentCutsceneId],cutscene_blackscreens[currentCutsceneId],cutscene_lengths[currentCutsceneId]);
            }
        }
        else if (currentTime >= targetTime && !inCutscene && inFadeTransition)
        {
            currentAlpha = endAlpha;
            inFadeTransition = false;
            FinishLastCutscene();
        }
        else if (!inCutscene && inFadeTransition)
        {
            currentAlpha = startAlpha + (endAlpha - startAlpha) * (currentTime.asMilliseconds() / (targetTime.asMilliseconds()+0.0));
        } else if (inCutscene && isBlackScreenCutscene){
            currentAlpha = startAlpha;
        } else if (inCutscene){
            currentAlpha = startAlpha;
        }
        // apply alpha to whatever colour is previously set
        if((inFadeTransition || inCutscene) && isBlackScreenCutscene){
            sf::Color fadeColor = fade.getFillColor();
            fadeColor.a = currentAlpha;
            fade.setFillColor(fadeColor);
            fade.setSize(sf::Vector2f(window.getSize().x,window.getSize().y));

            fade.setPosition(0,0);
            window.draw(fade);
        }
        if (inCutscene){
            for (int i=0;i<t_cutscene_text.size();i++){
                sf::Text currentLine = t_cutscene_text[i];

                currentLine.setPosition(window.getSize().x/2,300 + 39*i);
                sf::Time currentTime = timer.getElapsedTime();

                window.draw(currentLine);
            }
        }
        window.setView(lastView);

        /// here we show the hitbox
        bool showHitboxes = false;
        if(showHitboxes){
            sf::RectangleShape hitboxRect(sf::Vector2f(patapon.hitBox.width, patapon.hitBox.height));
            hitboxRect.setPosition(patapon.x+patapon.hitBox.left,patapon.y+patapon.hitBox.top);
            window.draw(hitboxRect);

            sf::RectangleShape kacheekHitboxRect(sf::Vector2f(kacheek.hitBox.width, kacheek.hitBox.height));
            kacheekHitboxRect.setPosition(kacheek.x+kacheek.hitBox.left,kacheek.y+kacheek.hitBox.top);
            window.draw(kacheekHitboxRect);
        }


        rhythm.fps = fps;
        ///ugh this is a BAD solution i need to do it differently

        rhythm.Draw(window);

}
void MissionController::FinishLastCutscene(){
    /// runs when the last cutscene of a sequence is done
}
bool MissionController::isMoreCutscenes(){
    /// returns true if there are more cutscenes
    return currentCutsceneId<cutscene_text_identifiers.size()-1;
}
void MissionController::StartCutscene(const std::wstring& text,bool isBlackScreen, int TimeToShow){
    /// because the description needs to be able to go over multiple lines, we have to split it into a series of lines
    t_cutscene_text.clear();
    std::vector<std::wstring> wordsinDesc = Func::Split(missionConfig->strRepo.GetUnicodeString(text),' ');
    sf::String oldTotalString;
    sf::String currentTotalString;
    int maxWidth = missionConfig->GetInt("resX") * 0.4;
    timer.restart();
    inCutscene = true;
    isBlackScreenCutscene = isBlackScreen;
    targetTime = sf::seconds(TimeToShow);
    /// we split it into words, then go word by word testing the width of the string
    for (int i=0;i<wordsinDesc.size();i++){
        std::wstring currentWord = wordsinDesc[i];
        currentTotalString = currentTotalString + Func::ConvertToUtf8String(currentWord) + L" ";
        sf::Text t_newLine;
        t_newLine.setFont(f_font);
        t_newLine.setCharacterSize(35);
        t_newLine.setFillColor(sf::Color::White);
        t_newLine.setString(currentTotalString);
        if (t_newLine.getGlobalBounds().width>maxWidth){
            /// when the string is too long, we go back to the last string and lock it in, then start a new line
            currentTotalString = oldTotalString;
            t_newLine.setString(currentTotalString);
            t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
            t_cutscene_text.push_back(t_newLine);
            oldTotalString = currentWord+L" ";
            currentTotalString = currentWord+L" ";
        }
        oldTotalString = currentTotalString;
        /// if there are no more words, finish up the current line
        if (i+1==wordsinDesc.size()){
            currentTotalString = oldTotalString;
            t_newLine.setString(currentTotalString);
            t_newLine.setOrigin(t_newLine.getGlobalBounds().width/2,t_newLine.getGlobalBounds().height/2);
            t_cutscene_text.push_back(t_newLine);
            oldTotalString = "";
            currentTotalString = "";
        }
    }
}
MissionController::~MissionController()
{
    //dtor

}
