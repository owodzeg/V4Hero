#include "RhythmController.h"
#include <iostream>

using namespace std;

RhythmController::RhythmController()
{
    b_pata[0].loadFromFile("resources/sfx/drums/pata.ogg");
    b_pata[1].loadFromFile("resources/sfx/drums/pata_2.ogg");
    b_pata[2].loadFromFile("resources/sfx/drums/pata_3.ogg");

    b_pon[0].loadFromFile("resources/sfx/drums/pon.ogg");
    b_pon[1].loadFromFile("resources/sfx/drums/pon_2.ogg");
    b_pon[2].loadFromFile("resources/sfx/drums/pon_3.ogg");

    b_chaka[0].loadFromFile("resources/sfx/drums/chaka.ogg");
    b_chaka[1].loadFromFile("resources/sfx/drums/chaka_2.ogg");
    b_chaka[2].loadFromFile("resources/sfx/drums/chaka_3.ogg");

    b_don[0].loadFromFile("resources/sfx/drums/don.ogg");
    b_don[1].loadFromFile("resources/sfx/drums/don_2.ogg");
    b_don[2].loadFromFile("resources/sfx/drums/don_3.ogg");

    b_chpata[0].loadFromFile("resources/sfx/drums/ch_pata.ogg");
    b_chpata[1].loadFromFile("resources/sfx/drums/ch_pata_2.ogg");
    b_chpata[2].loadFromFile("resources/sfx/drums/ch_pata_3.ogg");

    b_chpon[0].loadFromFile("resources/sfx/drums/ch_pon.ogg");
    b_chpon[1].loadFromFile("resources/sfx/drums/ch_pon_2.ogg");
    b_chpon[2].loadFromFile("resources/sfx/drums/ch_pon_3.ogg");

    b_chchaka[0].loadFromFile("resources/sfx/drums/ch_chaka.ogg");
    b_chchaka[1].loadFromFile("resources/sfx/drums/ch_chaka_2.ogg");
    b_chchaka[2].loadFromFile("resources/sfx/drums/ch_chaka_3.ogg");

    b_chdon[0].loadFromFile("resources/sfx/drums/ch_don.ogg");
    b_chdon[1].loadFromFile("resources/sfx/drums/ch_don_2.ogg");
    b_chdon[2].loadFromFile("resources/sfx/drums/ch_don_3.ogg");

    b_perfect.loadFromFile("resources/sfx/drums/perfect.ogg");

    patterns["pata"] = 0;
    patterns["pon"] = 0;
    patterns["don"] = 0;
    patterns["chaka"] = 0;
}

bool RhythmController::checkForInput()
{
    ///Set initial values for Drum quality check
    int drum_quality = 2;
    bool add_to_commandtable = false;

    ///Determine the quality of given drum input
    if(keyMap[config.GetInt("keybindPata")] || keyMap[config.GetInt("secondaryKeybindPata")] || keyMap[config.GetInt("keybindPon")] || keyMap[config.GetInt("secondaryKeybindPon")] || keyMap[config.GetInt("keybindDon")] || keyMap[config.GetInt("secondaryKeybindDon")] || keyMap[config.GetInt("keybindChaka")] || keyMap[config.GetInt("secondaryKeybindChaka")])
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
    if(keyMap[config.GetInt("keybindPata")] || keyMap[config.GetInt("secondaryKeybindPata")])
    {
        ///Add PATA drum sound effect to the buffer
        drum_nc.setBuffer(b_pata[drum_quality]);
        drum_c.setBuffer(b_chpata[drum_quality]);

        ///Add PATA drum to user input table
        if(add_to_commandtable)
        commandInput.push_back("PATA");

        drumToLoad = "pata";
        currentPattern = patterns[drumToLoad];

        current_drum = "pata";

        patterns[drumToLoad]++;
    }

    ///Keybind for PON drum
    if(keyMap[config.GetInt("keybindPon")] || keyMap[config.GetInt("secondaryKeybindPon")])
    {
        ///Add PON drum sound effect to the buffer
        drum_nc.setBuffer(b_pon[drum_quality]);
        drum_c.setBuffer(b_chpon[drum_quality]);

        ///Add PON drum to user input table
        if(add_to_commandtable)
        commandInput.push_back("PON");

        drumToLoad = "pon";
        currentPattern = patterns[drumToLoad];

        current_drum = "pon";

        patterns[drumToLoad]++;
    }

    ///Keybind for CHAKA drum
    if(keyMap[config.GetInt("keybindChaka")] || keyMap[config.GetInt("secondaryKeybindChaka")])
    {
        ///Add CHAKA drum sound effect to the buffer
        drum_nc.setBuffer(b_chaka[drum_quality]);
        drum_c.setBuffer(b_chchaka[drum_quality]);

        ///Add CHAKA drum to user input table
        if(add_to_commandtable)
        commandInput.push_back("CHAKA");

        drumToLoad = "chaka";
        currentPattern = patterns[drumToLoad];

        current_drum = "chaka";

        patterns[drumToLoad]++;
    }

    ///Keybind for DON drum
    if(keyMap[config.GetInt("keybindDon")] || keyMap[config.GetInt("secondaryKeybindDon")])
    {
        ///Add DON drum sound effect to the buffer
        drum_nc.setBuffer(b_don[drum_quality]);
        drum_c.setBuffer(b_chdon[drum_quality]);

        ///Add DON drum to user input table
        if(add_to_commandtable)
        commandInput.push_back("DON");

        drumToLoad = "don";
        currentPattern = patterns[drumToLoad];

        current_drum = "don";

        patterns[drumToLoad]++;
    }

    ///IF statement that applies to all drum keybinds (to not repeat the same code over and over)
    if(keyMap[config.GetInt("keybindPata")] || keyMap[config.GetInt("secondaryKeybindPata")] || keyMap[config.GetInt("keybindPon")] || keyMap[config.GetInt("secondaryKeybindPon")] || keyMap[config.GetInt("keybindDon")] || keyMap[config.GetInt("secondaryKeybindDon")] || keyMap[config.GetInt("keybindChaka")] || keyMap[config.GetInt("secondaryKeybindChaka")])
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

        ///Reset all keybindings
        keyMap[config.GetInt("keybindPata")] = false;
        keyMap[config.GetInt("secondaryKeybindPata")] = false;
        keyMap[config.GetInt("keybindPon")] = false;
        keyMap[config.GetInt("secondaryKeybindPon")] = false;
        keyMap[config.GetInt("keybindChaka")] = false;
        keyMap[config.GetInt("secondaryKeybindChaka")] = false;
        keyMap[config.GetInt("keybindDon")] = false;
        keyMap[config.GetInt("secondaryKeybindDon")] = false;

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
