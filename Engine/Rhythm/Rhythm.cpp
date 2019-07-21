#include <iostream>
#include <sstream>
#include "Rhythm.h"
#include "SongController.h"
#include <cmath>
using namespace std;

Rhythm::Rhythm()
{


    b_fever_start.loadFromFile("resources/sfx/bgm/fever_start.ogg");
    b_fever_fail.loadFromFile("resources/sfx/bgm/fever_fail.ogg");
}
void Rhythm::Stop()
{
    s_theme[0].stop();
    s_theme[1].stop();
    s_fever_fail.stop(); ///Dying fever sound
    s_fever_start.stop(); ///FEVER!
    s_chant.stop();
}
void Rhythm::LoadTheme(string theme)
{
    config.debugOut->DebugMessage("theme:"+theme+"\n");
    b_chant.clear();
    Stop();
    ///Load the BGM
    songController = new SongController();
    songController->LoadSongByName(theme);
    this->currentThemeName = theme;

    cycle = 0;
    cycle_mode = 0;

    bgm_cycle = 0;

    combo = 0;

    ///Stop any current action
    current_song = "";

    ///Restart the Rhythm clocks
    rhythmClock.restart();

    s_theme[0].setBuffer(songController->GetSongByNumber(0,0));
    s_theme[0].play();

    beat_timer = floor(songController->GetSongByNumber(0,0).getDuration().asMilliseconds() / float(8.08));
    cout << "Beat timer set to: " << beat_timer << endl;
}

void Rhythm::BreakCombo()
{
    config.debugOut->RhythmnDebugMessage("Oops! You broke your combo!\n");

    rhythmClock.restart();

    s_chant.stop();
    if(combo >= 11) ///Play dying fever sound when BGM is in fever state
    {
        ///Dying fever sound
        s_fever_fail.setBuffer(b_fever_fail);
        s_fever_fail.play();
    }

    ///Reset combo to idle BGM point
    combo = 1;

    ///Reset Perfects table
    rhythmController.perfect = 0;
    rhythmController.perfects.clear();

    ///Reset command input
    rhythmController.commandInput.clear();
    rhythmController.command_perfects.clear();

    ///Stop the theme
    s_theme[0].stop();
    s_theme[1].stop();

    ///Play the idle loop
    s_theme[0].setBuffer(songController->GetSongByNumber(0,1));
    s_theme[0].play();

    ///Stop any current action
    current_song = "";

    ///Beat cycles
    cycle = 4;
    cycle_mode = 1;

    bgm_cycle = 0;
}

void Rhythm::checkRhythmController(sf::RenderWindow& window)
{
    ///RHYTHM CONTROLLER SETUP
    rhythmController.combo = combo;

    rhythmController.masterTimer = abs(rhythmClock.getElapsedTime().asMilliseconds() - 250);
    rhythmController.av_commands = av_commands;

    rhythmController.low_range = low_range;
    rhythmController.high_range = high_range;

    if(rhythmController.checkForInput())
    {
        Drum temp;
        temp.Load(rhythmController.drumToLoad,rhythmController.drum_perfection,window);
        temp.pattern = rhythmController.currentPattern;
        drums.push_back(temp);

        if(rhythmController.breakCombo)
        {
            BreakCombo();
        }
    }

    rhythmController.resetValues();
}
void Rhythm::Draw(sf::RenderWindow& window)
{
    checkRhythmController(window);
    r_gui.doVisuals(window,bgm_cycle,&rhythmClock,combo,&flicker,fps,&drums);

    /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        LoadTheme(config.GetString("debugTheme"));
    }*/

    if(rhythmClock.getElapsedTime().asMilliseconds() > (beat_timer/float(2)))
    {
        ///cout << "Small: " << (beat_timer/float(2)) << endl;

        if(!count_cycle)
        {
            int tmp_cycle = floor(beatCycleClock.getElapsedTime().asMilliseconds() / (beat_timer));

            if(tmp_cycle >= 8)
            {
                beatCycleClock.restart();
            }

            cycle_mode = abs(floor((tmp_cycle)/4) - 1);
            cycle = (tmp_cycle)%4 + 1;

            if(combo >= 2)
            {
                if(rhythmController.hit)
                {
                    if(cycle_mode == 1)
                    {
                        BreakCombo();
                    }
                }
                else
                {
                    if(cycle_mode == 0)
                    {
                        BreakCombo();
                    }
                }
            }
            else
            {
                if(!rhythmController.hit)
                {
                    ///Clear user input
                    rhythmController.commandInput.clear();
                }
            }

            rhythmController.hit = false;
            count_cycle = true;
        }
    }

    if(rhythmClock.getElapsedTime().asMilliseconds() > beat_timer)
    {
        ///cout << "Big: " << beat_timer << endl;

        count_cycle = false;
        bgm_cycle++;

        rhythmClock.restart();

        if(combo <= 1) ///start anytime function
        {
            if(rhythmController.hit == false)
            {
                if(rhythmController.commandInput.size() == 4) ///If user input is 4 drums
                {
                    string fullcom = rhythmController.commandInput[0]+rhythmController.commandInput[1]+rhythmController.commandInput[2]+rhythmController.commandInput[3]; ///Create a full command using 4 individual hits
                    int index = distance(av_commands.begin(), find(av_commands.begin(), av_commands.end(), fullcom));

                    if(index < av_commands.size()) ///Check if the command exists in available commands
                    {
                        ///Clear user input
                        rhythmController.commandInput.clear();

                        ///Push the amount of perfect hits to the table and reset them
                        rhythmController.perfects.push_back(rhythmController.perfect);
                        rhythmController.perfect = 0;

                        rhythmController.command_perfects.clear();

                        combo = 2;

                        current_song = av_songs[index];

                        ///To not overlap with the music
                        s_theme[0].stop();
                        s_theme[1].stop();

                        s_theme[combo%2].setBuffer(songController->GetSongByNumber(0,combo));

                        s_theme[combo%2].stop();
                        s_theme[combo%2].play();

                        if(config.GetInt("enablePataponChants"))
                        {
                            s_chant.setBuffer(songController->GetChantByNumber(0,av_songs[index]+"_1"));
                            s_chant.play();
                        }

                        beatCycleClock.restart();

                        bgm_cycle = 0;
                    }
                }
            }
        }

        if(bgm_cycle >= 8)
        {
            bgm_cycle = 0;

            if(combo < 2)
            {
                combo = 1;

                s_theme[0].setBuffer(songController->GetSongByNumber(0,combo));

                s_theme[0].stop();
                s_theme[1].stop();

                s_theme[0].play();
            }

            if(combo >= 2) /// If combo is not idle bgm
            {
                config.debugOut->RhythmnDebugMessage("Combo: " + std::to_string(combo) +"\n");

                string fullcom = rhythmController.commandInput[0]+rhythmController.commandInput[1]+rhythmController.commandInput[2]+rhythmController.commandInput[3]; ///Create a full command using 4 individual hits

                int index = distance(av_commands.begin(), find(av_commands.begin(), av_commands.end(), fullcom));

                if(index < av_commands.size()) ///Check if the command exists in available commands
                {
                    combo += 1;

                    if(combo >= 28)
                    combo = 12;

                    current_song = av_songs[index];

                    ///Clear user input
                    rhythmController.commandInput.clear();

                    ///Push the amount of perfect hits to the table and reset them
                    rhythmController.perfects.push_back(rhythmController.perfect);
                    rhythmController.perfect = 0;

                    rhythmController.command_perfects.clear();

                    while(rhythmController.perfects.size() > 4)
                    rhythmController.perfects.erase(rhythmController.perfects.begin());

                    float acc = (rhythmController.perfects[0]+rhythmController.perfects[1]+rhythmController.perfects[2]+rhythmController.perfects[3]) / float(16);
                    config.debugOut->RhythmnDebugMessage("Accuracy: " + std::to_string(acc*100) + "%\n" );

                    if(combo < 11)
                    {
                        if(acc > acc_req[combo])
                        {
                            if(combo < 6)
                            {
                                combo = 6;
                            }
                            else
                            {
                                combo = 11;
                            }
                        }
                    }

                    s_theme[combo%2].setBuffer(songController->GetSongByNumber(0,combo));

                    s_theme[combo%2].stop();
                    s_theme[combo%2].play();

                    if(config.GetInt("enablePataponChants"))
                    {
                        int song_id = 1;

                        if((combo >= 6) && (combo <= 11))
                        {
                            song_id = 2;
                        }
                        else if(combo > 11)
                        {
                            song_id = 3;

                            if((av_songs[index] == "patapata") || (av_songs[index] == "ponpon") || (av_songs[index] == "chakachaka"))
                            {
                                song_id = 3 + combo%2;
                            }
                        }


                        string song = av_songs[index]+"_"+to_string(song_id);
                        s_chant.setBuffer(songController->GetChantByNumber(0,song));

                        if(combo != 11)
                        s_chant.play();
                    }

                    if(combo == 11)
                    {
                        s_fever_start.setBuffer(b_fever_start);
                        s_fever_start.play();
                    }

                    beatCycleClock.restart();
                }
                else
                {
                    BreakCombo();
                }
            }
        }
    }
}
