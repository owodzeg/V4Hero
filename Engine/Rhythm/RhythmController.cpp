#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "RhythmController.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"

using namespace std;

RhythmController::RhythmController()
{
    for (int i = 0; i <= 2; i++)
    {
        string num = "_" + to_string(i + 1);

        if (i == 0)
            num = "";

        b_drum[Drums::PATA][i].loadFromFile("resources/sfx/drums/pata" + num + ".ogg");
        b_drum[Drums::PON][i].loadFromFile("resources/sfx/drums/pon" + num + ".ogg");
        b_drum[Drums::DON][i].loadFromFile("resources/sfx/drums/don" + num + ".ogg");
        b_drum[Drums::CHAKA][i].loadFromFile("resources/sfx/drums/chaka" + num + ".ogg");

        b_drumchant[Drums::PATA][i].loadFromFile("resources/sfx/drums/ch_pata" + num + ".ogg");
        b_drumchant[Drums::PON][i].loadFromFile("resources/sfx/drums/ch_pon" + num + ".ogg");
        b_drumchant[Drums::DON][i].loadFromFile("resources/sfx/drums/ch_don" + num + ".ogg");
        b_drumchant[Drums::CHAKA][i].loadFromFile("resources/sfx/drums/ch_chaka" + num + ".ogg");
    }

    b_perfect.loadFromFile("resources/sfx/drums/perfect.ogg");

    patterns["pata"] = 0;
    patterns["pon"] = 0;
    patterns["don"] = 0;
    patterns["chaka"] = 0;
}

bool RhythmController::checkForInput()
{
    InputController* inputCtrl = CoreManager::getInstance().getInputController();

    if (inputCtrl->lockRhythm)
        return false;

    ///Flush the buffers
    vector<int> s_rm;

    //cout << "RhythmController currently holds " << s_drums.size() << "/220 sounds." << endl;

    for (int i = s_drums.size() - 1; i > 0; i--)
    {
        if (s_drums[i].getStatus() == sf::Sound::Status::Stopped)
        {
            s_drums.erase(s_drums.begin() + i);
        } else
        {
            break;
        }
    }

    ///Set initial values for Drum quality check
    int drum_quality = 2;

    bool drumHit = false;

    // go through the 4 drums
    for(int i=0; i<4; i++)
    {
        drumHit = inputCtrl->isKeyPressed(keybinds[i]);

        if(drumHit)
        {
            if (masterTimer < low_range) ///BEST hit
            {
                ///Apply BAD drum sound effect
                drum_quality = DrumQuality::BAD;
            } else
            {
                ///Apply GOOD drum sound effect
                if (masterTimer >= high_range) ///BEST hit
                {
                    drum_quality = DrumQuality::BEST;
                } else if (masterTimer >= low_range) ///GOOD hit
                {
                    drum_quality = DrumQuality::GOOD;
                }
            }

            // calculate volume
            float drumVolume = float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f);

            // set buffers and volumes
            drum_nc.setBuffer(b_drum[i][drum_quality]);
            drum_c.setBuffer(b_drumchant[i][drum_quality]);

            drum_nc.setVolume(drumVolume);
            drum_c.setVolume(drumVolume);

            // add drum to user input table
            if(drum_quality != DrumQuality::BAD)
            {
                commandInput.push_back(i);
                canHit = false;
            }

            drumToLoad = drum_pngs[i];
            current_drum = drumToLoad;
            currentPattern = patterns[drumToLoad];
            patterns[drumToLoad]++;

            // register the input
            SPDLOG_DEBUG("drum: {} masterTimer: {}us masterTimerNoAbs: {}us", current_drum, masterTimer, masterTimerNoAbs);
            //SPDLOG_DEBUG("drum quality was {}", drum_quality);

            ///If drum was already hit and you hit once again, or you hit BAD, reset user input and break combo
            /*if ((hit) || (drum_quality == 2))
            {
                command_perfects.clear();
                perfects.clear();
                commandInput.clear();

                if (combo >= 2)
                    breakCombo = true;
                //SPDLOG_DEBUG("break combo #1");
            }*/

            // now to decide what command has been inputted, we use a new scheme
            // the BASE-5 drum system 
            // we got 4 beats = 8 halfbeats
            // each drum has 5 variations = PATA(0) + PON(1) + DON(2) + CHAKA(3) + NONE(4) <- its important
            // so calculate the drums as:
            // (5^7 * drum[7]) + (5^6 * drum[6]) + (5^5 * drum[5]) + (5^4 * drum[4]) + (5^3 * drum[3]) + (5^2 * drum[2]) + (5^1 * drum[1]) + (5^0 * drum[0])
            // will give us unique values per each drum combination!

            ///If drum was hit above the minimum range, determine it's quality and mark as being already hit
            if (drum_quality <= 1)
            {
                if (drum_quality == 0)
                {
                    command_perfects.push_back(true);
                }

                if (drum_quality == 1)
                {
                    command_perfects.push_back(false);
                }
            }

            ///Check config if drum sound effect should be played
            if (CoreManager::getInstance().getConfig()->GetInt("enableDrums"))
            {
                ///And play it
                s_drums.push_back(drum_nc);
                s_drums[s_drums.size() - 1].play();
            }

            ///Check config if drum chant sound effect should be played
            if (CoreManager::getInstance().getConfig()->GetInt("enableDrumChants"))
            {
                ///And play it
                s_drums.push_back(drum_c);
                s_drums[s_drums.size() - 1].play();
            }

            ///Remove drums from user input if user has hit more than 4 drums (8 half-beats)
            while (commandInput.size() > 8)
                commandInput.erase(commandInput.begin());

            int command = 0;

            for (unsigned int c = 0; c < commandInput.size(); c++)
            {
                command += commandInput[c] * pow(5, 7-c);
            }

            ///Calculate how many perfect beats were in the command
            if (command_perfects.size() >= 8)
            {
                while (command_perfects.size() > 8)
                    command_perfects.erase(command_perfects.begin());

                perfect = 0;

                for (unsigned int c = 0; c < command_perfects.size(); c++)
                {
                    perfect += command_perfects[c];
                }
            }
            
            SPDLOG_DEBUG("CommandInput.size() == {}", commandInput.size());

            //cout<<"Input registered"<<commandInput.size()<<endl;
            if (commandInput.size() == 8)
            {
                SPDLOG_DEBUG("Looking for command {}", command);

                if (std::find(base5_commands.begin(), base5_commands.end(), command) != base5_commands.end()) ///Check if the command exists in available commands
                {
                    SPDLOG_DEBUG("Command found! Perfects: {}", perfect);
                    if (perfect == 8)
                    {
                        s_perfect.stop();
                        s_perfect.setBuffer(b_perfect);
                        s_perfect.setVolume(drumVolume);
                        s_perfect.play();
                    }
                }
                else
                {
                    // here is a hack to get the "perfect shwing" noise, we don't want to wait for the remaining halfbeat,
                    // but rather to shwing upon the last drum
                    // so we kinda artificially add the last halfbeat here
                    std::vector<int> tmp_command;
                    tmp_command = commandInput;
                    tmp_command.erase(tmp_command.begin());
                    tmp_command.push_back(Drums::NONE);

                    command = 0;

                    for (unsigned int c = 0; c < tmp_command.size(); c++)
                    {
                        command += tmp_command[c] * pow(5, 7-c);
                    }

                    SPDLOG_DEBUG("[Hack] Looking for command {}", command);

                    // ask again
                    if (std::find(base5_commands.begin(), base5_commands.end(), command) != base5_commands.end()) 
                    {
                        SPDLOG_DEBUG("[Hack] Command found! Perfects: {}", perfect);
                        if (perfect == 8)
                        {
                            s_perfect.stop();
                            s_perfect.setBuffer(b_perfect);
                            s_perfect.setVolume(drumVolume);
                            s_perfect.play();
                        }
                    }
                }
            }

            drum_perfection = drum_quality;
            break;
        }
    }

    if(masterTimer < low_range && belowHitRange == false)
    {
        belowHitRange = true;

        if(canHit == true)
        {
            commandInput.push_back(Drums::NONE);
            command_perfects.push_back(true);
        }
        else
        {
            canHit = true;
        }
    }

    if(masterTimer >= low_range)
    {
        belowHitRange = false;
    }

     ///Remove drums from user input if user has hit more than 4 drums (8 half-beats)
    while (commandInput.size() > 8)
        commandInput.erase(commandInput.begin());

    ///Calculate how many perfect beats were in the command
    if (command_perfects.size() >= 8)
    {
        while (command_perfects.size() > 8)
            command_perfects.erase(command_perfects.begin());

        perfect = 0;

        for (unsigned int c = 0; c < command_perfects.size(); c++)
        {
            perfect += command_perfects[c];
        }
    }

    std::string s_command = "";
    std::string s_perfects = "";

    for (unsigned int c = 0; c < commandInput.size(); c++)
    {
        s_command += to_string(commandInput[c]) + " ";
    }

    for (unsigned int c = 0; c < command_perfects.size(); c++)
    {
        s_perfects += to_string(command_perfects[c]) + " ";
    }

    int command = 0;

    for (unsigned int c = 0; c < commandInput.size(); c++)
    {
        command += commandInput[c] * pow(5, 7-c);
    }

    //SPDLOG_DEBUG("Pending command: {}, int: {}, perfects: {}", s_command, command, s_perfects);

    if(command != 0)
    {
        bool found = false;

        if (std::find(base5_commands.begin(), base5_commands.end(), command) != base5_commands.end())
        {
            found = true;
            //SPDLOG_DEBUG("Command found! {}", command);
        }

        if (!found)
        {
            if (combo >= 2)
            {
                command_perfects.clear();
                perfects.clear();
                commandInput.clear();

                breakCombo = true;
                SPDLOG_DEBUG("break combo #2");
            }
        }
    }

    for(int i=0; i<4; i++)
    {
        patterns[drum_pngs[i]] = patterns[drum_pngs[i]] % 8;
    }

    return drumHit;
}

void RhythmController::resetDrums()
{
    drumToLoad = "";
    drum_perfection = 0;
    currentPattern = 0;
}

void RhythmController::resetCombo()
{
    breakCombo = false;
}