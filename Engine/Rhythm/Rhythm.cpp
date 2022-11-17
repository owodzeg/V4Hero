#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Rhythm.h"
#include "SongController.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"

using namespace std;

Rhythm::Rhythm()
{
    b_fever_start.loadFromFile("resources/sfx/bgm/fever_start.ogg");
    b_fever_fail.loadFromFile("resources/sfx/bgm/fever_fail.ogg");

    s_badrhythm1.loadFromFile("resources/sfx/level/badrhythm_1.ogg");
    s_badrhythm2.loadFromFile("resources/sfx/level/badrhythm_2.ogg");

    //CoreManager::getInstance().getConfig()->LoadConfig(CoreManager::getInstance().getConfig()->thisCore);
}

void Rhythm::Clear()
{
    vector<std::unique_ptr<SongController>>().swap(songController);
    std::map<std::string, sf::SoundBuffer>().swap(b_chant);
}

void Rhythm::Stop()
{
    s_theme[0].stop();
    s_theme[1].stop();
    s_fever_fail.stop();  ///Dying fever sound
    s_fever_start.stop(); ///FEVER!
    s_chant.stop();
}
void Rhythm::LoadTheme(string theme)
{
    low_range = CoreManager::getInstance().getConfig()->GetInt("lowRange");
    high_range = CoreManager::getInstance().getConfig()->GetInt("highRange");
    SPDLOG_INFO("Low Range: {} ms, High Range: {} ms", low_range, high_range);
    SPDLOG_INFO("Selected theme: {}", theme);

    Stop();
    ///Load the BGM
    std::unique_ptr<SongController> s = make_unique<SongController>();
    songController.push_back(std::move(s));
    songController[0].get()->LoadSongByName(theme);
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
    //current_song = "";

    ///Restart the Rhythm clocks
    //rhythmClock.restart();

    //s_theme[0].setBuffer(songController[0].get()->GetSongByNumber(0,0));
    //cout << "Volume is " << float(CoreManager::getInstance().getConfig()->GetInt("masterVolume"))*(float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume"))/100.f) << " " << CoreManager::getInstance().getConfig()->GetInt("masterVolume") << " " << CoreManager::getInstance().getConfig()->GetInt("bgmVolume") << endl;
    //s_theme[0].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume"))*(float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume"))/100.f));
    //s_theme[0].play();

    //beat_timer = floor(songController[0].get()->GetSongByNumber(0,0).getDuration().asMilliseconds() / float(8.08));
    //cout << "Beat timer set to: " << beat_timer << endl;
}

void Rhythm::Start()
{
    ///Stop any current action
    current_song = "";

    ///Restart the Rhythm clocks
    rhythmClock.restart();

    s_theme[0].setBuffer(songController[0].get()->GetSongByNumber(0, 0));
    SPDLOG_INFO("Volume is {} {} {}", float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume")) / 100.f), CoreManager::getInstance().getConfig()->GetInt("masterVolume"), CoreManager::getInstance().getConfig()->GetInt("bgmVolume"));
    s_theme[0].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume")) / 100.f));
    s_theme[0].play();

    beat_timer = floor(songController[0].get()->GetSongByNumber(0, 0).getDuration().asMilliseconds() / float(8.08));
    SPDLOG_INFO("Beat timer set to: {}", beat_timer);
}

void Rhythm::BreakCombo()
{
    updateworm = true;
    satisfaction_value.clear();
    rl_combo = 0;
    advanced_prefever = false;

    SPDLOG_DEBUG("Oops! You broke your combo!");

    rhythmClock.restart();

    s_chant.stop();
    if (combo >= 11) ///Play dying fever sound when BGM is in fever state
    {
        ///Dying fever sound
        s_fever_fail.setBuffer(b_fever_fail);
        s_fever_fail.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
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
    s_theme[0].setBuffer(songController[0].get()->GetSongByNumber(0, 1));
    s_theme[0].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume")) / 100.f));
    s_theme[0].play();

    ///Stop any current action
    current_song = "";

    ///Beat cycles
    cycle = 4;
    cycle_mode = 1;

    bgm_cycle = 0;

    pata_react.stop();
    pata_react.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));

    pata_react.setBuffer(s_badrhythm2);

    pata_react.play();
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

void Rhythm::checkRhythmController()
{
    ///RHYTHM CONTROLLER SETUP
    rhythmController.combo = combo;

    rhythmController.masterTimer = abs(rhythmClock.getElapsedTime().asMilliseconds() - 250);
    rhythmController.av_commands = av_commands;

    rhythmController.low_range = low_range;
    rhythmController.high_range = high_range;

    if (rhythmController.checkForInput())
    {
        std::string drum_path = "resources/graphics/rhythm/drums/" + rhythmController.drumToLoad + ".png";

        Drum temp;
        temp.Load(rhythmController.drumToLoad, rhythmController.drum_perfection, drum_path);
        temp.pattern = rhythmController.currentPattern;
        drums.push_back(temp);

        if (rhythmController.breakCombo)
        {
            BreakCombo();
        }
    }

    rhythmController.resetValues();
}

void Rhythm::doRhythm()
{
    checkRhythmController();

    if (rhythmClock.getElapsedTime().asMilliseconds() > (beat_timer / float(2)))
    {
        ///cout << "Small: " << (beat_timer/float(2)) << endl;

        if (!count_cycle)
        {
            int tmp_cycle = floor(beatCycleClock.getElapsedTime().asMilliseconds() / (beat_timer));

            if (tmp_cycle >= 8)
            {
                beatCycleClock.restart();
            }

            cycle_mode = abs(floor((tmp_cycle) / 4) - 1);
            cycle = (tmp_cycle) % 4 + 1;

            if (combo >= 2)
            {
                if (rhythmController.hit)
                {
                    if (cycle_mode == 1)
                    {
                        BreakCombo();
                    }
                } else
                {
                    if (cycle_mode == 0)
                    {
                        BreakCombo();
                    }
                }
            } else
            {
                if (!rhythmController.hit)
                {
                    ///Clear user input
                    rhythmController.commandInput.clear();
                }
            }

            rhythmController.hit = false;
            count_cycle = true;
        }
    }

    if (rhythmClock.getElapsedTime().asMilliseconds() > beat_timer)
    {
        ///cout << "Big: " << beat_timer << endl;

        count_cycle = false;
        bgm_cycle++;

        rhythmClock.restart();

        if (combo <= 1) ///start anytime function
        {
            if (rhythmController.hit == false)
            {
                if (rhythmController.commandInput.size() == 4) ///If user input is 4 drums
                {
                    string fullcom = rhythmController.commandInput[0] + rhythmController.commandInput[1] + rhythmController.commandInput[2] + rhythmController.commandInput[3]; ///Create a full command using 4 individual hits
                    unsigned int index = distance(av_commands.begin(), find(av_commands.begin(), av_commands.end(), fullcom));

                    if (index < av_commands.size()) ///Check if the command exists in available commands
                    {
                        rl_combo++;
                        SPDLOG_DEBUG("rl_combo: {}", rl_combo);

                        ///Clear user input
                        rhythmController.commandInput.clear();

                        ///Push the amount of perfect hits to the table and reset them
                        rhythmController.perfects.push_back(rhythmController.perfect);

                        current_perfect = rhythmController.perfect;
                        rhythmController.perfect = 0;
                        rhythmController.command_perfects.clear();

                        float acc = current_perfect;
                        satisfaction_value.push_back(perfects_reward[acc]);

                        SPDLOG_DEBUG("acc: {} reward: {}", acc, perfects_reward[acc]);

                        if (satisfaction_value.size() > 3)
                            satisfaction_value.erase(satisfaction_value.begin());

                        float satisfaction = 0;

                        if (satisfaction_value.size() == 3)
                            satisfaction = satisfaction_value[2] * 0.8 + satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
                        if (satisfaction_value.size() == 2)
                            satisfaction = satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
                        if (satisfaction_value.size() == 1)
                            satisfaction = satisfaction_value[0] * 0.05;

                        SPDLOG_DEBUG("Satisfaction: {}", satisfaction);
                        last_satisfaction = satisfaction;

                        combo = 2;

                        current_song = av_songs[index];

                        ///To not overlap with the music
                        s_theme[0].stop();
                        s_theme[1].stop();

                        s_theme[combo % 2].setBuffer(songController[0].get()->GetSongByNumber(0, combo));
                        s_theme[combo % 2].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume")) / 100.f));

                        s_theme[combo % 2].stop();
                        s_theme[combo % 2].play();

                        if (CoreManager::getInstance().getConfig()->GetInt("enablePataponChants"))
                        {
                            if (current_song != "chakapata")
                                s_chant.setBuffer(songController[0].get()->GetChantByNumber(0, current_song + "_1"));
                            else
                                s_chant.setBuffer(songController[0].get()->GetChantByNumber(0, "patapata_1"));

                            s_chant.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                            s_chant.play();
                        }

                        beatCycleClock.restart();

                        bgm_cycle = 0;
                    }
                }
            }
        }

        if (bgm_cycle >= 8)
        {
            bgm_cycle = 0;

            if (combo < 2)
            {
                combo = 1;

                s_theme[0].setBuffer(songController[0].get()->GetSongByNumber(0, combo));
                s_theme[0].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume")) / 100.f));

                s_theme[0].stop();
                s_theme[1].stop();

                s_theme[0].play();
            }

            if (combo >= 2) /// If combo is not idle bgm
            {
                SPDLOG_DEBUG("Combo: {}", combo);

                string fullcom = rhythmController.commandInput[0] + rhythmController.commandInput[1] + rhythmController.commandInput[2] + rhythmController.commandInput[3]; ///Create a full command using 4 individual hits

                unsigned int index = distance(av_commands.begin(), find(av_commands.begin(), av_commands.end(), fullcom));

                if (index < av_commands.size()) ///Check if the command exists in available commands
                {
                    updateworm = true;

                    rl_combo++;
                    SPDLOG_DEBUG("rl_combo: {}", rl_combo);
                    combo += 1;

                    if (combo >= 28)
                        combo = 12;

                    current_song = av_songs[index];

                    ///Clear user input
                    rhythmController.commandInput.clear();

                    ///Push the amount of perfect hits to the table and reset them
                    rhythmController.perfects.push_back(rhythmController.perfect);
                    current_perfect = rhythmController.perfect;
                    rhythmController.perfect = 0;

                    rhythmController.command_perfects.clear();

                    while (rhythmController.perfects.size() > 4)
                        rhythmController.perfects.erase(rhythmController.perfects.begin());

                    if (combo < 11)
                    {
                        float acc = current_perfect;
                        satisfaction_value.push_back(perfects_reward[acc]);

                        SPDLOG_DEBUG("acc: {} reward: {}", acc, perfects_reward[acc]);

                        if (satisfaction_value.size() > 3)
                            satisfaction_value.erase(satisfaction_value.begin());

                        float satisfaction = 0;

                        if (satisfaction_value.size() == 3)
                            satisfaction = satisfaction_value[2] * 0.8 + satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
                        if (satisfaction_value.size() == 2)
                            satisfaction = satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
                        if (satisfaction_value.size() == 1)
                            satisfaction = satisfaction_value[0] * 0.05;

                        SPDLOG_DEBUG("Satisfaction: {}, requirement: {}, adv_pre: {}, theme_combo: {}", satisfaction, satisfaction_requirement[rl_combo], advanced_prefever, combo);
                        last_satisfaction = satisfaction;

                        SPDLOG_DEBUG("Accuracy: {}%", acc / 16 * 100);

                        if ((rl_combo >= 2) && (combo < 6))
                        {
                            if ((current_perfect >= 2) && ((satisfaction >= 150) || ((satisfaction_value.size() == 2) && (satisfaction >= 60))))
                            {
                                if (!advanced_prefever)
                                {
                                    advanced_prefever = true;
                                    combo = 6;
                                }
                            }
                        }

                        if ((rl_combo >= 3) && (combo >= 6))
                        {
                            if ((current_perfect <= 1) || (satisfaction < 150))
                            {
                                advanced_prefever = false;
                                combo = 2;
                            }
                        }

                        if (rl_combo > 9)
                        {
                            combo = 11;
                        }

                        if (satisfaction > satisfaction_requirement[rl_combo])
                        {
                            if (advanced_prefever)
                                combo = 11;
                        }
                    }

                    s_theme[combo % 2].setBuffer(songController[0].get()->GetSongByNumber(0, combo));
                    s_theme[combo % 2].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume")) / 100.f));

                    s_theme[combo % 2].stop();
                    s_theme[combo % 2].play();

                    if (CoreManager::getInstance().getConfig()->GetInt("enablePataponChants"))
                    {
                        int song_id = 1;

                        if ((combo >= 6) && (combo <= 11))
                        {
                            song_id = 2;
                        } else if (combo > 11)
                        {
                            song_id = 3;

                            if ((av_songs[index] == "patapata") || (av_songs[index] == "ponpon") || (av_songs[index] == "chakachaka") || (av_songs[index] == "chakapata"))
                            {
                                song_id = 3 + combo % 2;
                            }
                        }

                        string song;

                        if (av_songs[index] != "chakapata")
                            song = av_songs[index] + "_" + to_string(song_id);
                        else
                            song = "patapata_" + to_string(song_id);

                        s_chant.setBuffer(songController[0].get()->GetChantByNumber(0, song));
                        s_chant.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));

                        if (combo != 11)
                            s_chant.play();
                    }

                    if (combo == 11)
                    {
                        s_fever_start.setBuffer(b_fever_start);
                        s_fever_start.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));
                        s_fever_start.play();
                    }

                    beatCycleClock.restart();
                } else
                {
                    BreakCombo();
                }
            }
        }
    }
}

void Rhythm::Draw()
{
    r_gui.doVisuals(bgm_cycle, &rhythmClock, combo, &flicker, fps, &drums);
}
