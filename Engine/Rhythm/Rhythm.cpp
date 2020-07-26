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

    t_drums["pata"].loadFromFile("resources/graphics/rhythm/drums/pata.png");
    t_drums["pon"].loadFromFile("resources/graphics/rhythm/drums/pon.png");
    t_drums["don"].loadFromFile("resources/graphics/rhythm/drums/don.png");
    t_drums["chaka"].loadFromFile("resources/graphics/rhythm/drums/chaka.png");

    t_drums["pata"].setSmooth(true);
    t_drums["pon"].setSmooth(true);
    t_drums["don"].setSmooth(true);
    t_drums["chaka"].setSmooth(true);

    t_flash.loadFromFile("resources/graphics/rhythm/drums/flash.png");
    t_flash.setSmooth(true);
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
    rl_combo = 0;

    ///restart values
    current_perfect = 0;
    advanced_prefever = false;
    updateworm = false;
    last_satisfaction = 0;

    satisfaction_value.clear();

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
    updateworm = true;
    satisfaction_value.clear();
    rl_combo = 0;
    advanced_prefever = false;

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

int Rhythm::GetCombo()
{
    return combo;
}

int Rhythm::GetRealCombo()
{
    return rl_combo;
}

float Rhythm::GetSatisfaction()
{
    return last_satisfaction;
}

void Rhythm::checkRhythmController(InputController& inputCtrl)
{
    ///RHYTHM CONTROLLER SETUP
    rhythmController.combo = combo;

    rhythmController.masterTimer = abs(rhythmClock.getElapsedTime().asMilliseconds() - 250);
    rhythmController.av_commands = av_commands;

    rhythmController.low_range = low_range;
    rhythmController.high_range = high_range;

    if(rhythmController.checkForInput(inputCtrl))
    {
        Drum temp;
        temp.Load(rhythmController.drumToLoad,rhythmController.drum_perfection,t_drums[rhythmController.drumToLoad], t_flash);
        temp.pattern = rhythmController.currentPattern;
        drums.push_back(temp);

        if(rhythmController.breakCombo)
        {
            BreakCombo();
        }
    }

    rhythmController.resetValues();
}

void Rhythm::doRhythm(InputController& inputCtrl)
{
    checkRhythmController(inputCtrl);

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
                        rl_combo++;
                        cout << "rl_combo: " << rl_combo << endl;

                        ///Clear user input
                        rhythmController.commandInput.clear();

                        ///Push the amount of perfect hits to the table and reset them
                        rhythmController.perfects.push_back(rhythmController.perfect);

                        current_perfect = rhythmController.perfect;
                        rhythmController.perfect = 0;
                        rhythmController.command_perfects.clear();

                        float acc = current_perfect;
                        satisfaction_value.push_back(perfects_reward[acc]);

                        cout << "acc: " << acc << " reward: " << perfects_reward[acc] << endl;

                        if(satisfaction_value.size() > 3)
                        satisfaction_value.erase(satisfaction_value.begin());

                        float satisfaction = 0;

                        if(satisfaction_value.size() == 3)
                        satisfaction = satisfaction_value[2] * 0.8 + satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
                        if(satisfaction_value.size() == 2)
                        satisfaction = satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
                        if(satisfaction_value.size() == 1)
                        satisfaction = satisfaction_value[0] * 0.05;

                        cout << "Satisfaction: " << satisfaction << endl;
                        last_satisfaction = satisfaction;

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
                    updateworm = true;

                    rl_combo++;
                    cout << "rl_combo: " << rl_combo << endl;
                    combo += 1;

                    if(combo >= 28)
                    combo = 12;

                    current_song = av_songs[index];

                    ///Clear user input
                    rhythmController.commandInput.clear();

                    ///Push the amount of perfect hits to the table and reset them
                    rhythmController.perfects.push_back(rhythmController.perfect);
                    current_perfect = rhythmController.perfect;
                    rhythmController.perfect = 0;

                    rhythmController.command_perfects.clear();

                    while(rhythmController.perfects.size() > 4)
                    rhythmController.perfects.erase(rhythmController.perfects.begin());

                    if(combo < 11)
                    {
                        float acc = current_perfect;
                        satisfaction_value.push_back(perfects_reward[acc]);

                        cout << "acc: " << acc << " reward: " << perfects_reward[acc] << endl;

                        if(satisfaction_value.size() > 3)
                        satisfaction_value.erase(satisfaction_value.begin());

                        float satisfaction = 0;

                        if(satisfaction_value.size() == 3)
                        satisfaction = satisfaction_value[2] * 0.8 + satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
                        if(satisfaction_value.size() == 2)
                        satisfaction = satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
                        if(satisfaction_value.size() == 1)
                        satisfaction = satisfaction_value[0] * 0.05;

                        cout << "Satisfaction: " << satisfaction << ", requirement: " << satisfaction_requirement[rl_combo] << " adv_pre: " << advanced_prefever << " theme_combo: " << combo << endl;
                        last_satisfaction = satisfaction;

                        config.debugOut->RhythmnDebugMessage("Accuracy: " + std::to_string(acc/16*100) + "%\n" );

                        if((rl_combo >= 2) && (combo < 6))
                        {
                            if((current_perfect >= 2) && ((satisfaction >= 150) || ((satisfaction_value.size() == 2) && (satisfaction >= 60))))
                            {
                                if(!advanced_prefever)
                                {
                                    advanced_prefever = true;
                                    combo = 6;
                                }
                            }
                        }

                        if((rl_combo >= 3) && (combo >= 6))
                        {
                            if((current_perfect <= 1) || (satisfaction < 150))
                            {
                                advanced_prefever = false;
                                combo = 2;
                            }
                        }

                        if(rl_combo > 9)
                        {
                            combo = 11;
                        }

                        if(satisfaction > satisfaction_requirement[rl_combo])
                        {
                            if(advanced_prefever)
                            combo = 11;
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

void Rhythm::Draw(sf::RenderWindow& window)
{
    r_gui.doVisuals(window,bgm_cycle,&rhythmClock,combo,&flicker,fps,&drums);
}
