#include <iostream>
#include <sstream>
#include "Rhythm.h"

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
}

void Rhythm::LoadTheme(string theme)
{
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

    b_theme[28].loadFromFile("resources/sfx/bgm/fever_fail.ogg");

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


    s_theme[0].setBuffer(b_theme[0]);
    s_theme[0].play();

    rhythmClock.restart();
    masterTimer = 500;
}

void Rhythm::Draw(sf::RenderWindow& window)
{
    bool broke = false;

    if(rhythmClock.getElapsedTime().asSeconds() >= 4)
    {
        if(combo >= 2)
        {
            if(command.size() == 4)
            {
                string fullcom = command[0]+command[1]+command[2]+command[3];

                if(std::find(av_commands.begin(), av_commands.end(), fullcom) != av_commands.end())
                {
                    command.clear();

                    perfects.push_back(perfect);
                    perfect = 0;

                    float total_perfects = 0;

                    if(perfects.size() > acc_count)
                    {
                        perfects.erase(perfects.begin());
                    }

                    for(int i=0; i<=acc_count; i++)
                    {
                        if(perfects.size() > i)
                        {
                            total_perfects += perfects[i];
                        }
                    }

                    accuracy = total_perfects / (4 * perfects.size());
                    cout << "Total accuracy: " << accuracy*100 << "%" << endl;

                    combo++;

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

                    cout << "Command is being inputted - play the chant here." << endl;
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

                    int song_ID = -1;

                    for(int i=0; i<av_commands.size(); i++)
                    {
                        if(av_commands[i] == fullcom)
                        {
                            song_ID = i;
                        }
                    }

                    cout << "combo is equal to: " << combo << endl;

                    if(combo != 11)
                    {
                        string chant_name = av_songs[song_ID]+"_"+to_string(chant_id);
                        s_chant.stop();
                        s_chant.setBuffer(b_chant[chant_name]);
                        s_chant.play();
                    }
                }
                else
                {
                    broke = true;
                }
            }
            else
            {
                broke = true;
            }
        }

        if(combo == 0)
        combo = 1;

        if(combo >= 28)
        combo = 12;

        masterTimer = 450;
        masterTimerMode = 0;

        s_theme[combo%2].setBuffer(b_theme[combo]);

        s_theme[combo%2].stop();
        s_theme[combo%2].play();

        commandValue = 1;
        beatValue = 1;

        rhythmClock.restart();
    }

    if(masterTimer >= 500)
    {
        if((commandValue == 2) && (combo >= 2) && (command.size() < beatValue-1))
        {
            broke = true;
        }

        beatValue++;

        if(beatValue > 4)
        {
            beatValue = 1;
            commandValue++;
        }

        masterTimerMode = 1;
        masterTimer = 500;
    }
    else if(masterTimer <= 0)
    {
        masterTimerMode = 0;
        masterTimer = 0;

        drumAlreadyHit = false;
    }

    if(masterTimerMode == 1)
    r_rhythm.setFillColor(sf::Color(255,255,255,masterTimer/float(2)));
    window.draw(r_rhythm);

    if(masterTimerMode == 1)
    masterTimer -= float(2000) / fps;

    if(masterTimerMode == 0)
    masterTimer += float(2000) / fps;

    //cout << combo << endl;

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
            ///Add drum to command table
            add_to_commandtable = true;

            ///Apply GOOD drum sound effect
            drum_quality = 1;
        }
        else if(masterTimer >= high_range)
        {
            ///Add drum to command table
            add_to_commandtable = true;

            ///Apply BEST drum sound effect
            drum_quality = 0;
        }
    }

    if(keyMap[config.GetInt("keybindPata")] || keyMap[config.GetInt("secondaryKeybindPata")])
    {
        drum_nc.setBuffer(b_pata[drum_quality]);
        drum_c.setBuffer(b_chpata[drum_quality]);

        if(add_to_commandtable)
        command.push_back("PATA");
    }

    if(keyMap[config.GetInt("keybindPon")] || keyMap[config.GetInt("secondaryKeybindPon")])
    {
        drum_nc.setBuffer(b_pon[drum_quality]);
        drum_c.setBuffer(b_chpon[drum_quality]);

        if(add_to_commandtable)
        command.push_back("PON");
    }

    if(keyMap[config.GetInt("keybindChaka")] || keyMap[config.GetInt("secondaryKeybindChaka")])
    {
        drum_nc.setBuffer(b_chaka[drum_quality]);
        drum_c.setBuffer(b_chchaka[drum_quality]);

        if(add_to_commandtable)
        command.push_back("CHAKA");
    }

    if(keyMap[config.GetInt("keybindDon")] || keyMap[config.GetInt("secondaryKeybindDon")])
    {
        drum_nc.setBuffer(b_don[drum_quality]);
        drum_c.setBuffer(b_chdon[drum_quality]);

        if(add_to_commandtable)
        command.push_back("DON");
    }

    ///IF statement that applies to all drum keybinds (to not repeat the same code over and over)
    if(keyMap[config.GetInt("keybindPata")] || keyMap[config.GetInt("secondaryKeybindPata")] || keyMap[config.GetInt("keybindPon")] || keyMap[config.GetInt("secondaryKeybindPon")] || keyMap[config.GetInt("keybindDon")] || keyMap[config.GetInt("secondaryKeybindDon")] || keyMap[config.GetInt("keybindChaka")] || keyMap[config.GetInt("secondaryKeybindChaka")])
    {
        ///If combo is less than 1,
        if(combo <= 1)
        {
            if(drumAlreadyHit == true)
            command.clear();
        }
        else
        {
            if(drumAlreadyHit == true)
            broke = true;
        }

        if(masterTimer < low_range)
        {
            command_perfects.clear();
            perfects.clear();
            command.clear();

            if(combo >= 2)
            {
                broke = true;
            }
        }

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

        if(config.GetInt("enableDrums"))
        {
            s_drums.push_back(drum_nc);
            s_drums[s_drums.size()-1].play();
        }

        if(config.GetInt("enableDrumChants"))
        {
            s_drums.push_back(drum_c);
            s_drums[s_drums.size()-1].play();
        }

        if(command.size() >= 5)
        command.erase(command.begin());

        commandTimeout.restart();
        beforeFeverClock.restart();

        if(commandValue == 1)
        {
            if(combo >= 2)
            {
                broke = true;
            }
        }

        while(command_perfects.size() > 4)
        command_perfects.erase(command_perfects.begin());

        perfect = 0;

        for(int i=0; i<=command_perfects.size(); i++)
        {
            perfect += command_perfects[i];
        }

        keyMap[config.GetInt("keybindPata")] = false;
        keyMap[config.GetInt("secondaryKeybindPata")] = false;
        keyMap[config.GetInt("keybindPon")] = false;
        keyMap[config.GetInt("secondaryKeybindPon")] = false;
        keyMap[config.GetInt("keybindChaka")] = false;
        keyMap[config.GetInt("secondaryKeybindChaka")] = false;
        keyMap[config.GetInt("keybindDon")] = false;
        keyMap[config.GetInt("secondaryKeybindDon")] = false;
    }

    if(commandTimeout.getElapsedTime().asSeconds() >= 1)
    {
        command.clear();
    }

    if(beforeFeverClock.getElapsedTime().asMilliseconds() >= 505)
    {
        if(command.size() == 4)
        {
            if(combo <= 1)
            {
                string fullcom = command[0]+command[1]+command[2]+command[3];

                if(std::find(av_commands.begin(), av_commands.end(), fullcom) != av_commands.end())
                {
                    rhythmClock.restart();

                    beatValue = 1;
                    commandValue = 1;
                    masterTimer = 450;
                    masterTimerMode = 0;
                    combo = 2;

                    perfects.push_back(perfect);
                    cout << "Perfect drums: " << perfect << endl;
                    perfect = 0;

                    command.clear();
                    s_theme[0].stop();
                    s_theme[1].stop();

                    s_theme[0].setBuffer(b_theme[combo]);
                    s_theme[0].play();



                    int song_ID = -1;

                    for(int i=0; i<av_commands.size(); i++)
                    {
                        if(av_commands[i] == fullcom)
                        {
                            song_ID = i;
                        }
                    }

                    string chant_name = av_songs[song_ID]+"_1";
                    s_chant.stop();
                    s_chant.setBuffer(b_chant[chant_name]);
                    s_chant.play();
                }
            }
        }
    }

    if(broke)
    {
        if(combo >= 11)
        {
            ///Dying fever sound
            s_fever_fail.setBuffer(b_theme[28]);
            s_fever_fail.play();

        }

        rhythmClock.restart();

        beatValue = 1;
        commandValue = 1;
        masterTimer = 450;
        masterTimerMode = 0;
        combo = 1;

        perfect = 0;
        perfects.clear();

        command.clear();
        s_theme[0].stop();
        s_theme[1].stop();

        s_theme[0].setBuffer(b_theme[combo]);
        s_theme[1].play();
    }
}
