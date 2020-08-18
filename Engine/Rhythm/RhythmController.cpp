#include "RhythmController.h"
#include <iostream>

using namespace std;

RhythmController::RhythmController()
{
    for(int i=0; i<=2; i++)
    {
        string num = "_"+to_string(i);

        if(i==0)
        num = "";

        b_pata[i].loadFromFile("resources/sfx/drums/pata"+num+".ogg");
        b_pon[i].loadFromFile("resources/sfx/drums/pon"+num+".ogg");
        b_don[i].loadFromFile("resources/sfx/drums/don"+num+".ogg");
        b_chaka[i].loadFromFile("resources/sfx/drums/chaka"+num+".ogg");

        b_chpata[i].loadFromFile("resources/sfx/drums/ch_pata"+num+".ogg");
        b_chpon[i].loadFromFile("resources/sfx/drums/ch_pon"+num+".ogg");
        b_chdon[i].loadFromFile("resources/sfx/drums/ch_don"+num+".ogg");
        b_chchaka[i].loadFromFile("resources/sfx/drums/ch_chaka"+num+".ogg");
    }

    b_perfect.loadFromFile("resources/sfx/drums/perfect.ogg");

    patterns["pata"] = 0;
    patterns["pon"] = 0;
    patterns["don"] = 0;
    patterns["chaka"] = 0;
}

bool RhythmController::checkForInput(InputController& inputCtrl)
{
    ///Set initial values for Drum quality check
    int drum_quality = 2;
    bool add_to_commandtable = false;

    bool pata = inputCtrl.isKeyPressed(InputController::Keys::SQUARE);
    bool pon = inputCtrl.isKeyPressed(InputController::Keys::CIRCLE);
    bool don = inputCtrl.isKeyPressed(InputController::Keys::CROSS);
    bool chaka = inputCtrl.isKeyPressed(InputController::Keys::TRIANGLE);

    ///Determine the quality of given drum input
    if(pata || pon || don || chaka)
    {
        if(masterTimer < low_range) ///BAD hit
        {
            ///Apply BAD drum sound effect
            drum_quality = 2;
        }
        else
        {
            ///Apply GOOD drum sound effect
            if((masterTimer >= low_range) && (masterTimer < high_range)) ///GOOD hit
            {
                drum_quality = 1;
            }
            else if(masterTimer >= high_range)
            {
                drum_quality = 0;
            }

            ///Add drum to commandInput table
            add_to_commandtable = true;
        }
    }

    ///Keybind for PATA drum
    if(pata)
    {
        ///Add PATA drum sound effect to the buffer
        drum_nc.setBuffer(b_pata[drum_quality]);
        drum_c.setBuffer(b_chpata[drum_quality]);

        drum_nc.setVolume(float(config.GetInt("masterVolume"))*(float(config.GetInt("sfxVolume"))/100.f));
        drum_c.setVolume(float(config.GetInt("masterVolume"))*(float(config.GetInt("sfxVolume"))/100.f));

        ///Add PATA drum to user input table
        if(add_to_commandtable)
        commandInput.push_back("PATA");

        drumToLoad = "pata";
        currentPattern = patterns[drumToLoad];

        current_drum = "pata";

        patterns[drumToLoad]++;
    }

    ///Keybind for PON drum
    if(pon)
    {
        ///Add PON drum sound effect to the buffer
        drum_nc.setBuffer(b_pon[drum_quality]);
        drum_c.setBuffer(b_chpon[drum_quality]);

        drum_nc.setVolume(float(config.GetInt("masterVolume"))*(float(config.GetInt("sfxVolume"))/100.f));
        drum_c.setVolume(float(config.GetInt("masterVolume"))*(float(config.GetInt("sfxVolume"))/100.f));

        ///Add PON drum to user input table
        if(add_to_commandtable)
        commandInput.push_back("PON");

        drumToLoad = "pon";
        currentPattern = patterns[drumToLoad];

        current_drum = "pon";

        patterns[drumToLoad]++;
    }

    ///Keybind for CHAKA drum
    if(chaka)
    {
        ///Add CHAKA drum sound effect to the buffer
        drum_nc.setBuffer(b_chaka[drum_quality]);
        drum_c.setBuffer(b_chchaka[drum_quality]);

        drum_nc.setVolume(float(config.GetInt("masterVolume"))*(float(config.GetInt("sfxVolume"))/100.f));
        drum_c.setVolume(float(config.GetInt("masterVolume"))*(float(config.GetInt("sfxVolume"))/100.f));

        ///Add CHAKA drum to user input table
        if(add_to_commandtable)
        commandInput.push_back("CHAKA");

        drumToLoad = "chaka";
        currentPattern = patterns[drumToLoad];

        current_drum = "chaka";

        patterns[drumToLoad]++;
    }

    ///Keybind for DON drum
    if(don)
    {
        ///Add DON drum sound effect to the buffer
        drum_nc.setBuffer(b_don[drum_quality]);
        drum_c.setBuffer(b_chdon[drum_quality]);

        drum_nc.setVolume(float(config.GetInt("masterVolume"))*(float(config.GetInt("sfxVolume"))/100.f));
        drum_c.setVolume(float(config.GetInt("masterVolume"))*(float(config.GetInt("sfxVolume"))/100.f));

        ///Add DON drum to user input table
        if(add_to_commandtable)
        commandInput.push_back("DON");

        drumToLoad = "don";
        currentPattern = patterns[drumToLoad];

        current_drum = "don";

        patterns[drumToLoad]++;
    }

    ///IF statement that applies to all drum keybinds (to not repeat the same code over and over)
    if(pata || pon || don || chaka)
    {
        //cout<<"Input registered"<<endl;
        config.debugOut->RhythmnDebugMessage(current_drum + " " + std::to_string(masterTimer) + " ms \n");
        config.debugOut->RhythmnDebugMessage("drum quality was " + std::to_string(drum_quality) +"\n");

        ///If drum was already hit and you hit once again, or you hit BAD, reset user input and break combo
        if((hit) || (drum_quality == 2))
        {
            command_perfects.clear();
            perfects.clear();
            commandInput.clear();

            if(combo >= 2)
            breakCombo = true;
            config.debugOut->RhythmnDebugMessage("break combo #1\n");
        }

        ///If drum was hit above the minimum range, determine it's quality and mark as being already hit
        if(drum_quality <= 1)
        {
            if(drum_quality == 0)
            {
                command_perfects.push_back(true);
            }

            if(drum_quality == 1)
            {
                command_perfects.push_back(false);
            }

            hit = true;
        }

        ///Check config if drum sound effect should be played
        if(config.GetInt("enableDrums"))
        {
            ///And play it
            s_drums.push_back(drum_nc);
            s_drums[s_drums.size()-1].play();
        }

        ///Check config if drum chant sound effect should be played
        if(config.GetInt("enableDrumChants"))
        {
            ///And play it
            s_drums.push_back(drum_c);
            s_drums[s_drums.size()-1].play();
        }

        ///Remove drums from user input if user has hit more than 4 drums
        if(commandInput.size() > 4)
        commandInput.erase(commandInput.begin());

        ///Calculate how many perfect beats were in the command
        if(command_perfects.size() >= 4)
        {
            while(command_perfects.size() > 4)
            command_perfects.erase(command_perfects.begin());

            perfect = command_perfects[0]+command_perfects[1]+command_perfects[2]+command_perfects[3];
        }
        //cout<<"Input registered"<<commandInput.size()<<endl;
        if(commandInput.size() == 4)
        {
            string fullcom = commandInput[0]+commandInput[1]+commandInput[2]+commandInput[3]; ///Create a full command using 4 individual hits
            config.debugOut->RhythmnDebugMessage("fullcom: " + fullcom+"\n");

            if(std::find(av_commands.begin(), av_commands.end(), fullcom) != av_commands.end()) ///Check if the command exists in available commands
            {
                if(perfect == 4)
                {
                    s_perfect.stop();
                    s_perfect.setBuffer(b_perfect);
                    s_perfect.setVolume(float(config.GetInt("masterVolume"))*(float(config.GetInt("sfxVolume"))/100.f));
                    s_perfect.play();
                }
            }
        }

        if(patterns["pata"] >= 8)
        patterns["pata"] = 0;

        if(patterns["pon"] >= 8)
        patterns["pon"] = 0;

        if(patterns["don"] >= 8)
        patterns["don"] = 0;

        if(patterns["chaka"] >= 8)
        patterns["chaka"] = 0;

        drum_perfection = drum_quality;

        return true;
    }

    return false;
}

void RhythmController::resetValues()
{
    drumToLoad = "";
    drum_perfection = 0;
    currentPattern = 0;
    breakCombo = false;
}
