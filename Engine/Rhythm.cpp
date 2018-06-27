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

   /// change it to vector{elems}
    av_commands.push_back("PATAPATAPATAPON");
    av_commands.push_back("PONPONPATAPON");
    av_commands.push_back("CHAKACHAKAPATAPON");
    av_commands.push_back("PONPONCHAKACHAKA");
    av_commands.push_back("PATAPONDONCHAKA");
    av_commands.push_back("PONPATAPONPATA");
    av_commands.push_back("DONDONCHAKACHAKA");
    av_commands.push_back("CHAKAPATACHAKAPATA");

    av_songs.push_back("patapata");
    av_songs.push_back("ponpon");
    av_songs.push_back("chakachaka");
    av_songs.push_back("ponchaka");
    av_songs.push_back("donchaka");
    av_songs.push_back("ponpata");
    av_songs.push_back("dondon");
    av_songs.push_back("patapata");
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


    s_theme1.setBuffer(b_theme[0]);
    s_theme1.play();

    tclock.restart();
    cclock.restart();
    timer = 500;
}

void Rhythm::Draw(sf::RenderWindow& window)
{
    bool broke = false;

    if(cclock.getElapsedTime().asSeconds() >= 2)
    {
        cclock.restart();
    }

    //cout << "tclock time elapsed: " << tclock.getElapsedTime().asSeconds() << endl;

    if(tclock.getElapsedTime().asSeconds() >= 4)
    {
        if(test >= 2)
        {
            cout << "command.size(): " << command.size() << endl;

            if(command.size() == 4)
            {
                string fullcom = command[0]+command[1]+command[2]+command[3];
                cout << fullcom << endl;

                if(std::find(av_commands.begin(), av_commands.end(), fullcom) != av_commands.end())
                {
                    command.clear();

                    perfects.push_back(perfect);
                    cout << "Perfect drums: " << perfect << endl;
                    perfect = 0;

                    float total_perfects = 0;
                    float total_commands = 0;

                    if(perfects.size() > acc_count)
                    {
                        perfects.erase(perfects.begin());
                    }

                    for(int i=0; i<=acc_count; i++)
                    {
                        if(perfects.size() > i)
                        {
                            total_perfects += perfects[i];
                            total_commands += 4;
                        }
                    }

                    accuracy = total_perfects / total_commands;
                    cout << "Total accuracy: " << accuracy*100 << "%" << endl;

                    test++;

                    if(test < 11)
                    {
                        if(test == 10)
                        {
                            if(accuracy >= 0.80)
                            {
                                test = 11;
                            }
                        }

                        if(test == 9)
                        {
                            if(accuracy >= 0.825)
                            {
                                test = 11;
                            }
                        }

                        if(test == 8)
                        {
                            if(accuracy >= 0.85)
                            {
                                test = 11;
                            }
                        }

                        if(test == 7)
                        {
                            if(accuracy >= 0.875)
                            {
                                test = 11;
                            }
                        }

                        if(test == 6)
                        {
                            if(accuracy >= 0.9)
                            {
                                test = 11;
                            }
                        }

                        if(test == 5)
                        {
                            if(accuracy >= 0.75)
                            {
                                test = 6;
                            }
                        }

                        if(test == 4)
                        {
                            if(accuracy >= 0.8)
                            {
                                test = 6;
                            }
                        }

                        if(test == 3)
                        {
                            if(accuracy >= 0.85)
                            {
                                test = 6;
                            }
                        }

                        if(test == 2)
                        {
                            if(accuracy >= 0.875)
                            {
                                test = 6;
                            }
                        }
                    }

                    cout << "Command is being inputted - play the chant here." << endl;
                    int chant_id = 0;

                    if(test < 6)
                    {
                        chant_id = 1;
                    }

                    if(test >= 6)
                    if(test <= 10)
                    {
                        chant_id = 2;
                    }

                    if(test >= 11)
                    {
                        /// bool = true/false
                        /// bool = 1/0
                        /// if(0) 0 = false
                        /// if(1) 1 = true

                        if(test % 2) ///if it's odd
                        {
                            if(fullcom == "PATAPATAPATAPON" or fullcom == "PONPONPATAPON" or fullcom == "CHAKACHAKAPATAPON")
                            {
                                chant_id = 4;
                            }
                            else
                            {
                                chant_id = 3;
                            }
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

                    cout << "test is equal to: " << test << endl;

                    if(test != 11)
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

        if(test == 0)
        test = 1;

        if(test >= 28)
        test = 12;

        timer = 450;
        timermode = 0;

        if(test2 == 1)
        {
            s_theme2.setBuffer(b_theme[test]);

            s_theme2.stop();
            s_theme2.play();

            test2 = 2;
        }

        if(test2 == 0)
        {
            s_theme1.setBuffer(b_theme[test]);

            s_theme1.stop();
            s_theme1.play();

            test2 = 1;
        }

        if(test2 == 2)
        {
            test2 = 0;
        }

        comcycle = 1;
        com = 1;
        tclock.restart();
    }

    if(timer >= 500)
    {
        cout << comcycle << ": " << com << endl;
        if(comcycle == 2)
        {
            if(test >= 2)
            {
                if(command.size() < com-1)
                {
                    broke = true;
                }
            }
        }

        com++;

        if(com > 4)
        {
            com = 1;
            comcycle++;
        }

        timermode = 1;
        timer = 500;
    }

    if(timer <= 0)
    {
        timermode = 0;
        timer = 0;

        cycle = false;
    }

    if(timermode == 1)
    r_rhythm.setFillColor(sf::Color(255,255,255,timer/float(2)));
    window.draw(r_rhythm);

    if(timermode == 1)
    timer -= float(2000) / fps;

    if(timermode == 0)
    timer += float(2000) / fps;

    //cout << test << endl;

    if(keyMap[config.GetInt("keybindPata")])
    {
        if(test >= 2)
        {
            if(cycle == true)
            broke = true;
        }

        if(test <= 1)
        {
            if(cycle == true)
            command.clear();
        }

        string far;
        if(timermode == 1)
        far = "too far";
        else
        far = "too early";

        if(timer < 300)
        {
            cout << "PATA (BAD), " << timer << " ms, " << far << endl;
            command.push_back("PATA");

            command_perfects.clear();
            perfects.clear();

            if(config.GetInt("enableDrums"))
            {
                sf::Sound drum;
                drum.setBuffer(b_pata[2]);
                s_drums.push_back(drum);
                s_drums[s_drums.size()-1].play();
            }

            if(config.GetInt("enableDrumChants"))
            {
                sf::Sound drum;
                drum.setBuffer(b_chpata[2]);
                s_drums.push_back(drum);
                s_drums[s_drums.size()-1].play();
            }

            if(test >= 2)
            {
                broke = true;
            }

            command.clear();
        }

        if((timer >= 300) && (timer < 425))
        {
            cout << "PATA (GOOD), " << timer << " ms, " << far << endl;
            command.push_back("PATA");
            command_perfects.push_back(0);

            if(config.GetInt("enableDrums"))
            {
                sf::Sound drum;
                drum.setBuffer(b_pata[1]);
                s_drums.push_back(drum);
                s_drums[s_drums.size()-1].play();
            }

            if(config.GetInt("enableDrumChants"))
            {
                sf::Sound drum;
                drum.setBuffer(b_chpata[1]);
                s_drums.push_back(drum);
                s_drums[s_drums.size()-1].play();
            }

            cycle = true;
        }

        if(timer >= 425)
        {
            cout << "PATA (BEST), " << timer << " ms, " << far << endl;
            command.push_back("PATA");
            command_perfects.push_back(1);

            if(config.GetInt("enableDrums"))
            {
                sf::Sound drum;
                drum.setBuffer(b_pata[0]);
                s_drums.push_back(drum);
                s_drums[s_drums.size()-1].play();
            }

            if(config.GetInt("enableDrumChants"))
            {
                sf::Sound drum;
                drum.setBuffer(b_chpata[0]);
                s_drums.push_back(drum);
                s_drums[s_drums.size()-1].play();
            }

            cycle = true;
        }

        if(command.size() >= 5)
        command.erase(command.begin());

        erasecommand.restart();
        beforefever.restart();

        if(comcycle == 1)
        {
            if(test >= 2)
            {
                broke = true;
            }
        }

        keyMap[config.GetInt("keybindPata")] = false;

        if(command_perfects.size() > 4)
        command_perfects.erase(command_perfects.begin());

        int temp_perfect = 0;

        for(int i=0; i<=4; i++)
        {
            if(command_perfects.size() > i)
            {
                 temp_perfect += command_perfects[i];
            }
        }

        perfect = temp_perfect;
    }

    if(keyMap[config.GetInt("keybindPon")])
    {
        if(test >= 2)
        {
            if(cycle == true)
            broke = true;
        }

        if(test <= 1)
        {
            if(cycle == true)
            command.clear();
        }

        string far;
        if(timermode == 1)
        far = "too far";
        else
        far = "too early";

        if(timer < 300)
        {
            cout << "PON (BAD), " << timer << " ms, " << far << endl;

            command_perfects.clear();
            perfects.clear();

            sf::Sound drum;
            drum.setBuffer(b_pon[2]);
            s_drums.push_back(drum);
            s_drums[s_drums.size()-1].play();

            if(test >= 2)
            {
                broke = true;
            }

            command.clear();
        }

        if((timer >= 300) && (timer < 425))
        {
            cout << "PON (GOOD), " << timer << " ms, " << far << endl;
            command.push_back("PON");
            command_perfects.push_back(0);

            sf::Sound drum;
            drum.setBuffer(b_pon[1]);
            s_drums.push_back(drum);
            s_drums[s_drums.size()-1].play();

            cycle = true;
        }

        if(timer >= 425)
        {
            cout << "PON (BEST), " << timer << " ms, " << far << endl;
            command.push_back("PON");
            command_perfects.push_back(1);

            sf::Sound drum;
            drum.setBuffer(b_pon[0]);
            s_drums.push_back(drum);
            s_drums[s_drums.size()-1].play();

            cycle = true;
        }

        if(command.size() >= 5)
        command.erase(command.begin());

        erasecommand.restart();
        beforefever.restart();

        if(comcycle == 1)
        {
            if(test >= 2)
            {
                broke = true;
            }
        }

        keyMap[config.GetInt("keybindPon")] = false;

        if(command_perfects.size() > 4)
        command_perfects.erase(command_perfects.begin());

        int temp_perfect = 0;

        for(int i=0; i<=4; i++)
        {
            if(command_perfects.size() > i)
            {
                 temp_perfect += command_perfects[i];
            }
        }

        perfect = temp_perfect;
    }

    if(keyMap[config.GetInt("keybindChaka")])
    {
        if(test >= 2)
        {
            if(cycle == true)
            broke = true;
        }

        if(test <= 1)
        {
            if(cycle == true)
            command.clear();
        }

        string far;
        if(timermode == 1)
        far = "too far";
        else
        far = "too early";

        if((timer >= 0) && (timer < 300))
        {
            cout << "CHAKA (BAD), " << timer << " ms, " << far << endl;

            command_perfects.clear();
            perfects.clear();

            sf::Sound drum;
            drum.setBuffer(b_chaka[2]);
            s_drums.push_back(drum);
            s_drums[s_drums.size()-1].play();

            if(test >= 2)
            {
                broke = true;
            }

            command.clear();
        }

        if((timer >= 300) && (timer < 425))
        {
            cout << "CHAKA (GOOD), " << timer << " ms, " << far << endl;
            command.push_back("CHAKA");
            command_perfects.push_back(0);

            sf::Sound drum;
            drum.setBuffer(b_chaka[1]);
            s_drums.push_back(drum);
            s_drums[s_drums.size()-1].play();

            cycle = true;
        }

        if(timer >= 425)
        {
            cout << "CHAKA (BEST), " << timer << " ms, " << far << endl;
            command.push_back("CHAKA");
            command_perfects.push_back(1);

            sf::Sound drum;
            drum.setBuffer(b_chaka[0]);
            s_drums.push_back(drum);
            s_drums[s_drums.size()-1].play();

            cycle = true;
        }

        if(command.size() >= 5)
        command.erase(command.begin());

        erasecommand.restart();
        beforefever.restart();

        if(comcycle == 1)
        {
            if(test >= 2)
            {
                broke = true;
            }
        }

        keyMap[config.GetInt("keybindChaka")] = false;

        if(command_perfects.size() > 4)
        command_perfects.erase(command_perfects.begin());

        int temp_perfect = 0;

        for(int i=0; i<=4; i++)
        {
            if(command_perfects.size() > i)
            {
                 temp_perfect += command_perfects[i];
            }
        }

        perfect = temp_perfect;
    }

    if(keyMap[config.GetInt("keybindDon")])
    {
        if(test >= 2)
        {
            if(cycle == true)
            broke = true;
        }

        if(test <= 1)
        {
            if(cycle == true)
            command.clear();
        }

        string far;
        if(timermode == 1)
        far = "too far";
        else
        far = "too early";

        if((timer >= 0) && (timer < 300))
        {
            cout << "DON (BAD), " << timer << " ms, " << far << endl;

            command_perfects.clear();
            perfects.clear();

            sf::Sound drum;
            drum.setBuffer(b_don[2]);
            s_drums.push_back(drum);
            s_drums[s_drums.size()-1].play();

            if(test >= 2)
            {
                broke = true;
            }

            command.clear();
        }

        if((timer >= 300) && (timer < 425))
        {
            cout << "DON (GOOD), " << timer << " ms, " << far << endl;
            command.push_back("DON");
            command_perfects.push_back(0);

            sf::Sound drum;
            drum.setBuffer(b_don[1]);
            s_drums.push_back(drum);
            s_drums[s_drums.size()-1].play();

            cycle = true;
        }

        if(timer >= 425)
        {
            cout << "DON (BEST), " << timer << " ms, " << far << endl;
            command.push_back("DON");
            command_perfects.push_back(1);

            sf::Sound drum;
            drum.setBuffer(b_don[0]);
            s_drums.push_back(drum);
            s_drums[s_drums.size()-1].play();

            cycle = true;
        }

        if(command.size() >= 5)
        command.erase(command.begin());

        erasecommand.restart();
        beforefever.restart();

        if(comcycle == 1)
        {
            if(test >= 2)
            {
                broke = true;
            }
        }

        keyMap[config.GetInt("keybindDon")] = false;

        if(command_perfects.size() > 4)
        command_perfects.erase(command_perfects.begin());

        int temp_perfect = 0;

        for(int i=0; i<=4; i++)
        {
            if(command_perfects.size() > i)
            {
                 temp_perfect += command_perfects[i];
            }
        }

        perfect = temp_perfect;
    }

    if(erasecommand.getElapsedTime().asSeconds() >= 1)
    {
        command.clear();
    }

    if(beforefever.getElapsedTime().asMilliseconds() >= 505)
    {
        if(command.size() == 4)
        {
            if(test <= 1)
            {
                string fullcom = command[0]+command[1]+command[2]+command[3];

                if(std::find(av_commands.begin(), av_commands.end(), fullcom) != av_commands.end())
                {
                    cout << "beng" << endl;

                    tclock.restart();
                    com = 1;
                    comcycle = 1;
                    timer = 450;
                    timermode = 0;
                    test = 2;
                    test2 = 0;

                    perfects.push_back(perfect);
                    cout << "Perfect drums: " << perfect << endl;
                    perfect = 0;

                    command.clear();
                    s_theme1.stop();
                    s_theme2.stop();

                    s_theme1.setBuffer(b_theme[test]);
                    s_theme1.play();



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
        if(test >= 11)
        {
            ///dying fever sound

            s_theme3.setBuffer(b_theme[28]);
            s_theme3.play();

        }

        tclock.restart();
        com = 1;
        comcycle = 1;
        timer = 450;
        timermode = 0;
        test = 1;
        test2 = 0;

        perfect = 0;
        perfects.clear();

        command.clear();
        s_theme1.stop();
        s_theme2.stop();

        s_theme1.setBuffer(b_theme[test]);
        s_theme1.play();
    }
}
