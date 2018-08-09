#include <iostream>
#include <sstream>
#include "Rhythm.h"
#include <cmath>
using namespace std;

template < class T >
string to_string( T t )
{
    stringstream ss;
    ss << t;
    return ss.str();
}

Rhythm::Rhythm()
{
    r_rhythm.setSize(sf::Vector2f(100,100));
    r_rhythm.setFillColor(sf::Color::White);

    r_fever.setSize(sf::Vector2f(100,10));
    r_fever.setFillColor(sf::Color::Red);

    r_fever_meter.setSize(sf::Vector2f(100,10));
    r_fever_meter.setFillColor(sf::Color::Yellow);

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

    b_fever_start.loadFromFile("resources/sfx/bgm/fever_start.ogg");
    b_fever_fail.loadFromFile("resources/sfx/bgm/fever_fail.ogg");
}

void Rhythm::LoadTheme(string theme)
{
    ///Load the BGM
    b_theme[0].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_start.ogg");
    b_theme[1].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_1.ogg");
    b_theme[2].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_2.ogg");
    b_theme[3].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_3.ogg");
    b_theme[4].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_4.ogg");
    b_theme[5].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_5.ogg");
    b_theme[6].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_6.ogg");
    b_theme[7].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_7.ogg");
    b_theme[8].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_8.ogg");
    b_theme[9].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_9.ogg");
    b_theme[10].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_10.ogg");
    b_theme[11].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_feverstart.ogg");
    b_theme[12].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever1.ogg");
    b_theme[13].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever2.ogg");
    b_theme[14].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever3.ogg");
    b_theme[15].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever4.ogg");
    b_theme[16].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever5.ogg");
    b_theme[17].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever6.ogg");
    b_theme[18].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever7.ogg");
    b_theme[19].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever8.ogg");
    b_theme[20].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever9.ogg");
    b_theme[21].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever10.ogg");
    b_theme[22].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever11.ogg");
    b_theme[23].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever12.ogg");
    b_theme[24].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever13.ogg");
    b_theme[25].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever14.ogg");
    b_theme[26].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever15.ogg");
    b_theme[27].loadFromFile("resources/sfx/bgm/"+theme+"/bgm_fever16.ogg");

    ///All chants
    b_chant["patapata_1"].loadFromFile("resources/sfx/bgm/"+theme+"/s_patapata1.ogg");
    b_chant["patapata_2"].loadFromFile("resources/sfx/bgm/"+theme+"/s_patapata2.ogg");
    b_chant["patapata_3"].loadFromFile("resources/sfx/bgm/"+theme+"/s_patapata3.ogg");
    b_chant["patapata_4"].loadFromFile("resources/sfx/bgm/"+theme+"/s_patapata4.ogg");
    b_chant["chakachaka_1"].loadFromFile("resources/sfx/bgm/"+theme+"/s_chakachaka1.ogg");
    b_chant["chakachaka_2"].loadFromFile("resources/sfx/bgm/"+theme+"/s_chakachaka2.ogg");
    b_chant["chakachaka_3"].loadFromFile("resources/sfx/bgm/"+theme+"/s_chakachaka3.ogg");
    b_chant["chakachaka_4"].loadFromFile("resources/sfx/bgm/"+theme+"/s_chakachaka4.ogg");
    b_chant["ponpon_1"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponpon1.ogg");
    b_chant["ponpon_2"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponpon2.ogg");
    b_chant["ponpon_3"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponpon3.ogg");
    b_chant["ponpon_4"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponpon4.ogg");
    b_chant["ponpata_1"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponpata1.ogg");
    b_chant["ponpata_2"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponpata2.ogg");
    b_chant["ponpata_3"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponpata3.ogg");
    b_chant["ponchaka_1"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponchaka1.ogg");
    b_chant["ponchaka_2"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponchaka2.ogg");
    b_chant["ponchaka_3"].loadFromFile("resources/sfx/bgm/"+theme+"/s_ponchaka3.ogg");
    b_chant["donchaka_1"].loadFromFile("resources/sfx/bgm/"+theme+"/s_donchaka1.ogg");
    b_chant["donchaka_2"].loadFromFile("resources/sfx/bgm/"+theme+"/s_donchaka2.ogg");
    b_chant["donchaka_3"].loadFromFile("resources/sfx/bgm/"+theme+"/s_donchaka3.ogg");
    b_chant["dondon_1"].loadFromFile("resources/sfx/bgm/"+theme+"/s_dondon1.ogg");
    b_chant["dondon_2"].loadFromFile("resources/sfx/bgm/"+theme+"/s_dondon2.ogg");
    b_chant["dondon_3"].loadFromFile("resources/sfx/bgm/"+theme+"/s_dondon3.ogg");

    ///Play the BGM beginning after loading
    s_theme[0].setBuffer(b_theme[0]);
    s_theme[0].play();

    ///Restart the Rhythm clocks
    rhythmClock.restart();
    masterTimer = 500;
}

void Rhythm::Draw(sf::RenderWindow& window)
{
    bool breakCombo = false; ///Initialize a temporary value used for breaking the combo

    if(rhythmClock.getElapsedTime().asSeconds() >= 4) ///If the 2 commands will pass
    {
        if(combo >= 2) /// If combo is not idle bgm
        {
            if(commandInput.size() == 4) ///If user input is 4 drums
            {
                string fullcom = commandInput[0]+commandInput[1]+commandInput[2]+commandInput[3]; ///Create a full command using 4 individual hits

                if(std::find(av_commands.begin(), av_commands.end(), fullcom) != av_commands.end()) ///Check if the command exists in available commands
                {
                    ///Clear user input
                    commandInput.clear();

                    ///Push the amount of perfect hits to the table and reset them
                    perfects.push_back(perfect);
                    perfect = 0;

                    ///Remove unnecessary perfects
                    while(perfects.size() > acc_count)
                    perfects.erase(perfects.begin());

                    ///Calculate total perfect value
                    float total_perfects = 0;

                    for(int i=0; i<acc_count; i++)
                    {
                        total_perfects += perfects[i];
                    }

                    ///Calculate beat accuracy
                    accuracy = total_perfects / (4 * perfects.size());

                    ///Increment combo
                    combo++;

                    ///Check for prefever accuracy to determine combo skip
                    if((combo <= 10) && (combo >= 6))
                    {
                        if(accuracy >= 0.875 - (0.025 * (combo - 7)))
                        {
                            combo = 11;
                        }
                    }
                    else if((combo <= 5) && (combo >= 2))
                    {
                        if(accuracy >= 0.9 - (0.05 * (combo - 2)))
                        {
                            combo = 6;
                        }
                    }

                    ///Calculate which chant should be used
                    int chant_id = 0;

                    if(combo < 6)
                    {
                        chant_id = 1;
                    }
                    else if((combo >= 6) && (combo <= 10))
                    {
                        chant_id = 2;
                    }
                    else if(combo >= 11)
                    {
                        if(fullcom == "PATAPATAPATAPON" or fullcom == "PONPONPATAPON" or fullcom == "CHAKACHAKAPATAPON")
                        {
                            chant_id = 3 + combo % 2;
                        }
                        else
                        {
                            chant_id = 3;
                        }
                    }

                    ///Determine which song should be played
                    int song_ID = -1;

                    for(int i=0; i<av_commands.size(); i++)
                    {
                        if(av_commands[i] == fullcom)
                        {
                            song_ID = i;
                        }
                    }

                    ///Prevent chants from playing at FEVER combo
                    if(combo != 11)
                    {
                        string chant_name = av_songs[song_ID]+"_"+to_string(chant_id);
                        s_chant.stop();
                        s_chant.setBuffer(b_chant[chant_name]);
                        s_chant.play();
                    }

                    ///Make FEVER! sound playing at FEVER combo
                    if(combo == 11)
                    {
                        ///FEVER! sound
                        s_fever_start.setBuffer(b_fever_start);
                        s_fever_start.play();
                    }

                    flicker = 0;
                }
                else
                {
                    ///Break the combo if user input was less than 4 drums
                    breakCombo = true;
                }
            }
            else
            {
                breakCombo = true;
            }
        }

        ///Set the combo to 1 if Rhythm system just started
        if(combo == 0)
        combo = 1;

        ///Set the combo back to fever beginning, when it reaches the end of BGM
        if(combo >= 28)
        combo = 12;

        ///Set the master timer to proper value
        masterTimer = 450;
        masterTimerMode = 0;

        ///Play the BGM
        s_theme[combo%2].setBuffer(b_theme[combo]);

        s_theme[combo%2].stop();
        s_theme[combo%2].play();

        ///Reset command and beat values
        commandValue = 1;
        beatValue = 1;

        ///Reset Rhythm Clock
        rhythmClock.restart();
    }

    ///Patch for unusable command and beat values
    int totalComValue = ((commandValue - 1) * 4) + beatValue - 1;
    int usableCommandValue = floor(totalComValue / float(5));
    int usableBeatValue = floor(totalComValue - (usableCommandValue * 4));
    usableCommandValue += 1;

    ///Visuals
    if(masterTimerMode == 1)
    {
        ///Calculate the ratio for other window sizes (default is 1280x720)
        float ratio_X = window.getSize().x / float(1280);
        float ratio_Y = window.getSize().y / float(720);
        float ratio_universal = (window.getSize().x * window.getSize().y) / (float(1280) * float(720));

        /// Beat frame
        if((combo <= 1) or ((combo > 1) and (combo < 11) and (usableCommandValue == 2)))
        {
            r_rhythm.setFillColor(sf::Color(0,0,0,0));
            r_rhythm.setOutlineThickness(-ceil(3 * ratio_universal));
            r_rhythm.setOutlineColor(sf::Color(255,255,255,masterTimer/float(2)));
            r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (24 * ratio_X), (720 * ratio_Y) - (24 * ratio_Y)));
            r_rhythm.setPosition(12*ratio_X,12*ratio_Y);

            r_rhythm2.setOutlineColor(sf::Color(0,0,0,0));
        }
        else if((combo > 1) and (usableCommandValue == 1))
        {
            r_rhythm.setFillColor(sf::Color(0,0,0,0));
            r_rhythm.setOutlineThickness(-ceil(2 * ratio_universal));
            r_rhythm.setOutlineColor(sf::Color(64,64,64,masterTimer/float(2)));
            r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (20 * ratio_X), (720 * ratio_Y) - (20 * ratio_Y)));
            r_rhythm.setPosition(10*ratio_X,10*ratio_Y);

            r_rhythm2.setFillColor(sf::Color(0,0,0,0));
            r_rhythm2.setOutlineThickness(-ceil(2 * ratio_universal));
            r_rhythm2.setOutlineColor(sf::Color(64,64,64,masterTimer/float(2)));
            r_rhythm2.setSize(sf::Vector2f((1280 * ratio_X) - (30 * ratio_X), (720 * ratio_Y) - (30 * ratio_Y)));
            r_rhythm2.setPosition(15*ratio_X,15*ratio_Y);

            if(usableBeatValue == 4)
            {
                if(flicker == 0)
                {
                    r_rhythm.setOutlineColor(sf::Color(64,64,64,masterTimer/float(2)));
                    r_rhythm2.setOutlineColor(sf::Color(64,64,64,masterTimer/float(2)));
                }

                if(flicker == 1)
                {
                    r_rhythm.setOutlineColor(sf::Color(220,220,220,masterTimer/float(2)));
                    r_rhythm2.setOutlineColor(sf::Color(220,220,220,masterTimer/float(2)));
                }

                flicker++;

                if(flicker >= 2)
                {
                    flicker = 0;
                }
            }
        }
        else if((combo >= 11) and (usableCommandValue == 2))
        {
            r_rhythm.setFillColor(sf::Color(0,0,0,0));
            r_rhythm.setOutlineThickness(-ceil(7 * ratio_universal));

            r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (20 * ratio_X), (720 * ratio_Y) - (20 * ratio_Y)));
            r_rhythm.setPosition(10*ratio_X,10*ratio_Y);

            r_rhythm2.setOutlineColor(sf::Color(0,0,0,0));

            if(floor(flicker) == 0)
            {
                r_rhythm.setOutlineColor(sf::Color(255,255,0,masterTimer/float(2)));
            }
            else if(floor(flicker) == 1)
            {
                r_rhythm.setOutlineColor(sf::Color(255,255,255,masterTimer/float(2)));
            }
            else if(floor(flicker) == 2)
            {
                r_rhythm.setOutlineColor(sf::Color(0,255,255,masterTimer/float(2)));
            }
            else if(floor(flicker) == 3)
            {
                r_rhythm.setOutlineColor(sf::Color(0,255,0,masterTimer/float(2)));
            }

            flicker += float(1) / fps * 30;

            if(flicker >= 4)
            flicker = 0;
        }

        /// Fever meter
        int feverMeterWidth = 10*(combo-12);
        if(feverMeterWidth > 100)
        {
            feverMeterWidth = 100;
        }

        int sizeMod = masterTimer/float(100);

        r_fever.setSize(sf::Vector2f(100+sizeMod*2,10+sizeMod*2));
        r_fever_meter.setSize(sf::Vector2f(feverMeterWidth+sizeMod*2,10+sizeMod*2));

        r_fever.setPosition(50*ratio_X-sizeMod,50*ratio_Y-sizeMod);
        r_fever_meter.setPosition(50*ratio_X-sizeMod,50*ratio_Y-sizeMod);
    }

    window.draw(r_rhythm);
    window.draw(r_rhythm2);
    window.draw(r_fever);

    if(combo > 12)
    {
        window.draw(r_fever_meter);
    }

    for(int i=0; i<drums.size(); i++)
    {
        drums[i].fps = fps;
        drums[i].Draw(window);

        if(drums[i].alpha <= 0)
        {
            drums.erase(drums.begin() + i);
        }
    }

    ///Master Timer speed (Higher value = Faster Rhythm system)
    if(masterTimerMode == 1)
    masterTimer -= float(2000) / fps;

    if(masterTimerMode == 0)
    masterTimer += float(2000) / fps;

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
        else if((masterTimer >= low_range) && (masterTimer < high_range)) ///GOOD hit
        {
            ///Add drum to commandInput table
            add_to_commandtable = true;

            ///Apply GOOD drum sound effect
            drum_quality = 1;
        }
        else if(masterTimer >= high_range)
        {
            ///Add drum to commandInput table
            add_to_commandtable = true;

            ///Apply BEST drum sound effect
            drum_quality = 0;
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

        Drum temp;
        temp.Load("pata",window);
        drums.push_back(temp);
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

        Drum temp;
        temp.Load("pon",window);
        drums.push_back(temp);
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

        Drum temp;
        temp.Load("chaka",window);
        drums.push_back(temp);
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

        Drum temp;
        temp.Load("don",window);
        drums.push_back(temp);
    }

    ///IF statement that applies to all drum keybinds (to not repeat the same code over and over)
    if(keyMap[config.GetInt("keybindPata")] || keyMap[config.GetInt("secondaryKeybindPata")] || keyMap[config.GetInt("keybindPon")] || keyMap[config.GetInt("secondaryKeybindPon")] || keyMap[config.GetInt("keybindDon")] || keyMap[config.GetInt("secondaryKeybindDon")] || keyMap[config.GetInt("keybindChaka")] || keyMap[config.GetInt("secondaryKeybindChaka")])
    {
        ///If drum was already hit and you hit once again, or you hit BAD, reset user input and break combo
        if((drumAlreadyHit == true) || (masterTimer < low_range))
        {
            command_perfects.clear();
            perfects.clear();
            commandInput.clear();

            if(combo >= 2)
            breakCombo = true;
        }

        ///If drum was hit above the minimum range, determine it's quality and mark as being already hit
        if(masterTimer >= low_range)
        {
            bool perfect_command = false;

            if(masterTimer > high_range)
            {
                perfect_command = true;
            }

            command_perfects.push_back(perfect_command);
            drumAlreadyHit = true;
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

        ///Restart the Timeout and Before Fever clocks every time a drum is being hit
        commandTimeout.restart();
        beforeFeverClock.restart();

        ///If you start drumming when commandValue is at 1 (Patapon singing), break the combo
        if((commandValue == 1) && (combo >= 2))
        {
            breakCombo = true;
        }

        ///Calculate how many perfect beats were in the command
        if(command_perfects.size() >= 4)
        {
            while(command_perfects.size() > 4)
            command_perfects.erase(command_perfects.begin());

            perfect = command_perfects[0]+command_perfects[1]+command_perfects[2]+command_perfects[3];
        }

        ///Reset all keybindings
        keyMap[config.GetInt("keybindPata")] = false;
        keyMap[config.GetInt("secondaryKeybindPata")] = false;
        keyMap[config.GetInt("keybindPon")] = false;
        keyMap[config.GetInt("secondaryKeybindPon")] = false;
        keyMap[config.GetInt("keybindChaka")] = false;
        keyMap[config.GetInt("secondaryKeybindChaka")] = false;
        keyMap[config.GetInt("keybindDon")] = false;
        keyMap[config.GetInt("secondaryKeybindDon")] = false;
    }

    ///Timeout for inputting drums, clear all user input when user didn't took action for longer than a second
    if(commandTimeout.getElapsedTime().asSeconds() >= 1)
    {
        commandInput.clear();
    }

    ///Before Fever Clock function, made specifically to be able to start drumming from any point in music (when idle)
    if(beforeFeverClock.getElapsedTime().asMilliseconds() >= 505)
    {
        if(commandInput.size() == 4) ///Check if there are exactly 4 drums in user input
        {
            if(combo <= 1) ///If combo is less or equal to 1 (because it's the idle loop, aka the start)
            {
                string fullcom = commandInput[0]+commandInput[1]+commandInput[2]+commandInput[3]; ///Compose one big command string

                if(std::find(av_commands.begin(), av_commands.end(), fullcom) != av_commands.end()) ///Check if the command exists
                {
                    ///Reset the Rhythm Clock
                    rhythmClock.restart();

                    ///Reset the initial values
                    beatValue = 1;
                    commandValue = 1;
                    masterTimer = 450;
                    masterTimerMode = 0;

                    ///Increment the combo to BGM starting point
                    combo = 2;

                    ///Push the amount of perfect drums to the Perfects table
                    perfects.push_back(perfect);
                    perfect = 0;

                    ///Clear user input
                    commandInput.clear();

                    ///Reset BGM
                    s_theme[0].stop();
                    s_theme[1].stop();

                    ///Start playing BGM from the starting point
                    s_theme[0].setBuffer(b_theme[combo]);
                    s_theme[0].play();

                    ///Determine the song ID
                    int song_ID = -1;

                    for(int i=0; i<av_commands.size(); i++)
                    {
                        if(av_commands[i] == fullcom)
                        {
                            song_ID = i;
                        }
                    }

                    ///Determine the chant ID
                    string chant_name = av_songs[song_ID]+"_1";
                    s_chant.stop();
                    s_chant.setBuffer(b_chant[chant_name]);
                    s_chant.play();
                }
            }
        }
    }

    ///When Master timer has reached max value (PERFECT 500 ms, one beat)
    if(masterTimer >= 500)
    {
        ///Check if command value is set at 2 (User input), if combo is equal or above 2, check if user input is correct
        if((commandValue == 2) && (combo >= 2) && (commandInput.size() < beatValue-1))
        {
            breakCombo = true;
        }

        ///Increment the beat value
        beatValue++;

        ///If 4 beats has passed, increment the command value
        if(beatValue > 4)
        {
            beatValue = 1;
            commandValue++;
        }

        ///Reset master timer and change the mode
        masterTimerMode = 1;
        masterTimer = 500;

        flicker = 0;
    }
    else if(masterTimer <= 0)
    {
        ///Reset master timer and change the mode
        masterTimerMode = 0;
        masterTimer = 0;

        ///Disable drum block
        drumAlreadyHit = false;
    }

    ///Break Combo function (NOTE: can be ported to a function in class later)
    if(breakCombo)
    {
        if(combo >= 11) ///Play dying fever sound when BGM is in fever state
        {
            ///Dying fever sound
            s_fever_fail.setBuffer(b_fever_fail);
            s_fever_fail.play();
        }

        ///Reset Rhythm Clock
        rhythmClock.restart();

        ///Reset initial values
        beatValue = 1;
        commandValue = 1;
        masterTimer = 450;
        masterTimerMode = 0;

        ///Reset combo to idle BGM point
        combo = 1;

        ///Reset Perfects table
        perfect = 0;
        perfects.clear();

        ///Reset command input
        commandInput.clear();

        ///Reset BGM
        s_theme[0].stop();
        s_theme[1].stop();

        ///Play BGM from the combo of idle point
        s_theme[0].setBuffer(b_theme[combo]);
        s_theme[1].play();
    }

}
