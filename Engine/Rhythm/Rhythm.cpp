#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Rhythm.h"
#include "SongController.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"

#include <nlohmann/json.hpp>

using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

Rhythm::Rhythm()
{
    b_fever_start.loadFromFile("resources/sfx/bgm/fever_start.ogg");
    b_fever_fail.loadFromFile("resources/sfx/bgm/fever_fail.ogg");

    s_badrhythm1.loadFromFile("resources/sfx/level/badrhythm_1.ogg");
    s_badrhythm2.loadFromFile("resources/sfx/level/badrhythm_2.ogg");

    b_metronome.loadFromFile("resources/sfx/drums/metronome.ogg");
    s_metronome.setBuffer(b_metronome);

    b_ding.loadFromFile("resources/sfx/drums/ding.ogg");
    s_ding.setBuffer(b_ding);

    b_anvil.loadFromFile("resources/sfx/drums/anvil.ogg");
    s_anvil.setBuffer(b_anvil);

    // fetch commands from file
    ifstream t("resources/data/commands.json", std::ios::in);
    nlohmann::json command_data;
    t >> command_data;

    // map of values equivalent to the quinary system from RhythmController.h
    std::map<std::string, int> drum_values;
    drum_values["PATA"] = 0;
    drum_values["PON"] = 1;
    drum_values["DON"] = 2;
    drum_values["CHAKA"] = 3;
    drum_values[""] = 4;

    for(auto command : command_data["commands"])
    {
        SPDLOG_DEBUG("Found command, name: {}, song: {}", command["name"], command["song"]);
        av_songs.push_back(command["song"]);

        nlohmann::json beat_data = command["beat"];
        int power = 7;
        int result = 0;
        
        for(auto beat : beat_data)
        {
            result += drum_values[beat] * pow(5, power);
            power--;    
        }

        base5_commands.push_back(result);
        SPDLOG_DEBUG("Added result quinary value: {}", result);
    }

    //CoreManager::getInstance().getConfig()->LoadConfig(CoreManager::getInstance().getConfig()->thisCore);
}

void Rhythm::Clear()
{
    //vector<std::unique_ptr<SongController>>().swap(songController);
    //std::map<std::string, sf::SoundBuffer>().swap(b_chant);
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
    //TO-DO: uncomment this later
    //low_range = CoreManager::getInstance().getConfig()->GetInt("lowRange");
    //high_range = CoreManager::getInstance().getConfig()->GetInt("highRange");
    //SPDLOG_INFO("Low Range: {} ms, High Range: {} ms", low_range, high_range);
    SPDLOG_INFO("Selected theme: {}", theme);

    Stop();
    
    ///Load the BGM
    try
    {
        SongController* songController = CoreManager::getInstance().getSongController();
        songController->LoadTheme("ahwoon");

        // after loading SongController, get BPM and re-do the calculations
        BPM = songController->getBPM(); ///beats per minute
        beat_timer = 60.f / (BPM*2) * 1000000.f; ///Amount of microseconds for each halfbeat to be made
        beat_ms = 60.f / BPM * 1000.f; ///Amount of milliseconds for each beat
        halfbeat_ms = beat_ms / 2.f;
        measure_ms = beat_ms * 4.f;
        low_range = beat_timer / 6.25f;  ///Anything below that range will be treated as BAD hit
        high_range = beat_timer / 3.125f; ///Anything between this and low range will be treated as GOOD hit. Higher will be treated as BEST hit.   

        // set bpm for rhythm gui
        CoreManager::getInstance().getRhythmGUI()->BPM = BPM;
        CoreManager::getInstance().getRhythmGUI()->beat_timer = 60.f / BPM * 1000.f;
    }
    catch ( SongControllerException& ex )
    {
        SPDLOG_ERROR("Got SongController exception when loading theme: {}", ex.what());
        // TODO: in future, throw an exception to be caught by MissionController and stop mission loading
    }
    catch ( std::exception& ex )
    {
        SPDLOG_ERROR("Got an exception when loading theme: {}", ex.what());
    }
    catch ( ... )
    {
        SPDLOG_ERROR("Got unknown exception when loading theme.");
    }

    //std::unique_ptr<SongController> s = make_unique<SongController>();
    //songController.push_back(std::move(s));
    //songController[0].get()->LoadSongByName(theme);
    //this->currentThemeName = theme;


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

void Rhythm::PlaySong(SongController::SongType songType)
{
    song_channel = (song_channel + 1) % 2;
    SPDLOG_DEBUG("Playing songType: {}, song_channel: {}", static_cast<int>(songType), song_channel);

    SongController* songController = CoreManager::getInstance().getSongController();
    
    s_theme[song_channel].stop();
    s_theme[song_channel].setBuffer(songController->getSong(songType));
    s_theme[song_channel].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume")) / 100.f));
    s_theme[song_channel].play();
}

void Rhythm::Start()
{
    ///Stop any current action
    current_song = "";

    ///Restart the Rhythm clocks
    rhythmClock.restart();
    newRhythmClock.restart();

    //Start the rhythm
    started = true;

    //Play the theme start song
    PlaySong(SongController::SongType::START);
}

void Rhythm::BreakCombo(int reason)
{
    RhythmController* rhythmController = CoreManager::getInstance().getRhythmController();

    /*
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
    rhythmController->perfect = 0;
    rhythmController->perfects.clear();

    ///Reset command input
    rhythmController->commandInput.clear();
    rhythmController->command_perfects.clear();

    ///Stop the theme
    s_theme[0].stop();
    s_theme[1].stop();

    ///Play the idle loop
    
    //s_theme[0].setBuffer(songController[0].get()->GetSongByNumber(0, 1));
    //s_theme[0].setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("bgmVolume")) / 100.f));
    //s_theme[0].play();

    ///Stop any current action
    current_song = "";

    ///Beat cycles
    cycle = 4;
    cycle_mode = 1;

    bgm_cycle = 0;

    pata_react.stop();
    pata_react.setVolume(float(CoreManager::getInstance().getConfig()->GetInt("masterVolume")) * (float(CoreManager::getInstance().getConfig()->GetInt("sfxVolume")) / 100.f));

    pata_react.setBuffer(s_badrhythm2);

    pata_react.play();*/

    combo = 0;
    s_anvil.play();

    newRhythmClock.restart();
    
    metronomeVal = 0;
    metronomeOldVal = 9999;
    metronomeState = 1;
    metronomeClick = false;

    s_theme[0].stop();
    s_theme[1].stop();

    drumTicks = -1;
    measureCycle = 0;
    satisfaction_value.clear();
    advanced_prefever = false;

    SongController* songController = CoreManager::getInstance().getSongController();
    songController->flushOrder();

    // additional handling for certain combo breaks
    switch(reason)
    {
        case 7:
            command.clear();
            rhythmController->commandInputProcessed.clear();
        break;
    }

    SPDLOG_DEBUG("Combo break! Reason code: #{}", reason);
}

int Rhythm::GetCombo()
{
    return combo;
}

int Rhythm::GetBgmCycle()
{
    return bgm_cycle;
}

int Rhythm::GetRealCombo()
{
    return rl_combo;
}

float Rhythm::GetSatisfaction()
{
    return last_satisfaction;
}

void Rhythm::decideSongType()
{
    if(combo == 0)
    {
        currentSongType = SongController::SongType::IDLE;
        SPDLOG_DEBUG("I am deciding: IDLE");
        return;
    }

    if(combo == 6 && currentSongType == SongController::SongType::PREFEVER_INTENSE_START)
    {
        currentSongType = SongController::SongType::PREFEVER_INTENSE;
        SPDLOG_DEBUG("I am deciding: PREFEVER_INTENSE");
        return;
    }

    if(combo > 0)
    {
        RhythmController* rhythmController = CoreManager::getInstance().getRhythmController();

        ///Push the amount of perfect hits to the table and reset them
        float perfection = rhythmController->perfection;
        int perfectBeats = perfection * 8.f;

        if(perfection == 1)
            current_perfect += 1;
        else
            current_perfect = 0;

        if (combo < 11)
        {
            satisfaction_value.push_back(perfects_reward[perfectBeats]);

            SPDLOG_DEBUG("acc: {} reward: {}", perfection, perfects_reward[perfectBeats]);

            if (satisfaction_value.size() > 3)
                satisfaction_value.erase(satisfaction_value.begin());

            float satisfaction = 0;

            if (satisfaction_value.size() == 3)
                satisfaction = satisfaction_value[2] * 0.8 + satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
            if (satisfaction_value.size() == 2)
                satisfaction = satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05;
            if (satisfaction_value.size() == 1)
                satisfaction = satisfaction_value[0] * 0.05;

            SPDLOG_DEBUG("Satisfaction: {}, requirement: {}, adv_pre: {}, theme_combo: {}", satisfaction, satisfaction_requirement[combo], advanced_prefever, combo);
            SPDLOG_DEBUG("Accuracy: {}%", perfection * 100);

            if ((combo >= 2) && (combo < 6))
            {
                if ((current_perfect >= 2) && ((satisfaction >= 150) || ((satisfaction_value.size() == 2) && (satisfaction >= 60))))
                {
                    if (!advanced_prefever)
                    {
                        advanced_prefever = true;
                        currentSongType = SongController::SongType::PREFEVER_INTENSE_START;
                        combo = 6;
                        SPDLOG_DEBUG("I am deciding: doing great! PREFEVER_INTENSE_START");
                        return;
                    }
                }
            }

            if (combo >= 6)
            {
                if ((current_perfect <= 1) || (satisfaction < 150))
                {
                    advanced_prefever = false;
                    currentSongType = SongController::SongType::PREFEVER_CALM;
                    combo = 2;
                    SPDLOG_DEBUG("I am deciding: you goofed. PREFEVER_CALM");
                    return;
                }
            }

            if (satisfaction > satisfaction_requirement[combo])
            {
                if (advanced_prefever)
                {
                    SPDLOG_DEBUG("Skip to combo 10 = FEVER!");
                    combo = 10;
                }
            }
        }

    }

    if(combo == 10)
    {
        currentSongType = SongController::SongType::FEVER_START;
        SPDLOG_DEBUG("I am deciding: FEVER_START");
        return;
    }

    if(combo > 10)
    {
        currentSongType = SongController::SongType::FEVER;
        SPDLOG_DEBUG("I am deciding: FEVER! :D");
        return;
    }

    SPDLOG_DEBUG("No decision made. PREFEVER_CALM");
    currentSongType = SongController::SongType::PREFEVER_CALM;
}

void Rhythm::addRhythmMessage(RhythmAction action_id, std::string message)
{
    mtx.lock();

    RhythmMessage new_message;
    new_message.action = action_id;
    new_message.message = message;
    new_message.timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    SPDLOG_DEBUG("Adding new rhythm message: action {}, message {}, timestamp {}", action_id, message, new_message.timestamp);
    messages.push_back(new_message);

    mtx.unlock();
}

std::vector<Rhythm::RhythmMessage> Rhythm::fetchRhythmMessages(uint64_t& timestamp)
{
    mtx.lock();

    std::vector<Rhythm::RhythmMessage> messages_time;

    for(unsigned int i=0; i<messages.size(); i++)
    {
        //SPDLOG_DEBUG("Comparing rhythm message[{}] {} with timestamp {}", i, messages[i].timestamp, timestamp);
        if(messages[i].timestamp >= timestamp)
        {
            messages_time.push_back(messages[i]);
        }
    }

    mtx.unlock();

    timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    return messages_time;
}

void Rhythm::checkRhythmController()
{
    RhythmController* rhythmController = CoreManager::getInstance().getRhythmController();

    ///RHYTHM CONTROLLER SETUP
    rhythmController->combo = combo;

    sf::Int64 rhythmClockValue = newRhythmClock.getElapsedTime().asMicroseconds();

    rhythmController->masterTimer = abs((rhythmClockValue % int(beat_timer)) - (beat_timer/2));
    rhythmController->masterTimerNoAbs = rhythmClockValue % int(beat_timer);
    rhythmController->base5_commands = base5_commands;

    rhythmController->low_range = low_range;
    rhythmController->high_range = high_range;

    if(rhythmController->checkForInput())
    {
        //if(lazyClock.getElapsedTime().asMilliseconds() > 1000)
        //{
        //    drumTicks = 0;
        //}

        //lazyClock.restart();

        if(rhythmController->commandWithMissingHalfBeat)
        {
            firstCommandDelayClock.restart();
            firstCommandDelay = true;

            rhythmController->commandWithMissingHalfBeat = false;
        }
        
        if(drumTicksNoInput < 0 && combo > 0)
        {
            BreakCombo(3);
        }

        if(combo > 0 && afterMeasureClock.getElapsedTime().asMilliseconds() < measure_ms - halfbeat_ms)
        {
            BreakCombo(5);
        }

        if(afterPerfectClock.getElapsedTime().asMilliseconds() < beat_ms)
        {
            BreakCombo(7);
        }

        drumTicksNoInput = 0;

        commandWaitClock.restart();
    }

    if(rhythmController->commandInputProcessed.size() > 0)
    {
        command = rhythmController->commandInputProcessed;
        rhythmController->commandInputProcessed.clear();
    }
}

void Rhythm::doRhythm()
{
    if(!started)
    return;

    sf::Int64 rhythmClockValue = newRhythmClock.getElapsedTime().asMicroseconds();

    metronomeVal = (rhythmClockValue % int(beat_timer));
    //SPDLOG_DEBUG("metronome: {}", metronomeVal);

    if(metronomeVal < metronomeOldVal)
    {    
        metronomeState += 1;
        metronomeState = metronomeState % 2;

        metronomeClick = true;
        SPDLOG_DEBUG("[metronome] click... {} {} {}", metronomeOldVal, metronomeVal, metronomeState);

        if(metronomeState == 0)
        {
            s_metronome.play();

            CoreManager::getInstance().getRhythmGUI()->click();
            
            drumTicks += 1;
            drumTicks = drumTicks % 4;

            drumTicksNoInput += 1;

            SPDLOG_DEBUG("Drum ticks: {}, combo: {}, drumTicksNoInput: {}, commandWaitClock: {}ms", drumTicks, combo, drumTicksNoInput, commandWaitClock.getElapsedTime().asMilliseconds());

            if(command.size() > 0)
            {
                // start of rhythm, no commands yet
                if(combo == 0)
                {
                    // check if a command with last blank halfbeat was executed
                    if(firstCommandDelay)
                    {
                        if(firstCommandDelayClock.getElapsedTime().asMilliseconds() > halfbeat_ms)
                        {
                            SPDLOG_DEBUG("[CASE 1] Execute a command here!");
                            command.clear();

                            combo += 1;
                            drumTicks = 0; //reset drum ticks to keep the beat for next commands

                            firstCommandDelay = false;
                            drumTicksNoInput = -3;

                            commandWaitClock.restart();
                            afterMeasureClock.restart();

                            decideSongType();
                            PlaySong(currentSongType);

                            s_ding.play();
                        }
                    }
                    else //otherwise, handle the command that contains last halfbeat as drum
                    {
                        // commands that should start instantly (ones that would end with a halfnote)
                        SPDLOG_DEBUG("[CASE 2] Execute a command here!");
                        command.clear();

                        combo += 1;
                        drumTicks = 0; //reset drum ticks to keep the beat for next commands
                        drumTicksNoInput = -3;

                        commandWaitClock.restart();
                        afterMeasureClock.restart();
                            
                        decideSongType();
                        PlaySong(currentSongType);

                        s_ding.play();
                    }
                }
                else // combo system in place, command needs to follow drum tick pattern
                {
                    if(drumTicks == 0)
                    {
                        SPDLOG_DEBUG("[CASE 3] Execute a command here!");
                        command.clear();

                        combo += 1;
                        drumTicksNoInput = -3;

                        commandWaitClock.restart();
                        afterMeasureClock.restart();

                        decideSongType();
                        PlaySong(currentSongType);

                        s_ding.play();
                    }
                }
            }
            else //everything that happens outside of the command input
            {
                if(drumTicksNoInput >= 3 && combo > 0)
                {
                    BreakCombo(1);
                }

                if(drumTicks == 0 && combo == 0 && drumTicksNoInput >= 1)
                {
                    if(measureCycle == 0)
                        PlaySong(SongController::SongType::IDLE);

                    measureCycle += 1;

                    if(measureCycle == 2)
                    measureCycle = 0;
                }
            }
        }
    }

    metronomeOldVal = metronomeVal;

    // here goes everything outside of the metronome
    if(combo > 0 && commandWaitClock.getElapsedTime().asMilliseconds() > measure_ms + halfbeat_ms)
    {
        BreakCombo(2);
    }

    std::vector<RhythmMessage> last_messages = fetchRhythmMessages(lastMessageCheck);
    
    for(auto message : last_messages)
    {
        if(message.action == RhythmAction::DRUM_BAD)
        {
            if(combo > 0)
            {
                BreakCombo(4);
            }
        }

        if(message.action == RhythmAction::FOUND_COMMAND || message.action == RhythmAction::PERFECT_COMMAND) //we've found a command, we want to lock the immediately-next input
        {
            afterPerfectClock.restart();
        }
    }

    if(combo > 0 && afterMeasureClock.getElapsedTime().asMilliseconds() > measure_ms*2) // response + measure
    {
        BreakCombo(6);
    }

    checkRhythmController();

    InputController* inputCtrl = CoreManager::getInstance().getInputController();

    if(inputCtrl->isKeyPressed(Input::Keys::LTRIGGER))
    {
        PlaySong(currentSongType);
    }
    
    if(inputCtrl->isKeyPressed(Input::Keys::RTRIGGER))
    {
        debug_song_type = (debug_song_type + 1) % 7;
        currentSongType = static_cast<SongController::SongType>(debug_song_type);
    }

    // NEW RHYTHM SYSTEM
    // metronomeState = 0: click
    // metronomeState = 1: half beat
    // we have to reach to the metronomeState=0 right after we receive a full command

    // OLD RHYTHM SYSTEM

    /*
    if (rhythmClock.getElapsedTime().asMilliseconds() > beat_timer)
    {
        ///cout << "Big: " << beat_timer << endl;

        count_cycle = false;
        bgm_cycle++;

        rhythmClock.restart();

        if (combo <= 1) ///start anytime function
        {
            if (rhythmController->hit == false)
            {
                if (rhythmController->commandInput.size() == 8) ///If user input is 4 drums (8 halfbeats)
                {
                    int fullcom = 0;

                    for (unsigned int i = 0; i < rhythmController->commandInput.size(); i++)
                    {
                        fullcom += pow(rhythmController->commandInput[i], 7-i);
                    }

                    unsigned int index = distance(base5_commands.begin(), find(base5_commands.begin(), base5_commands.end(), fullcom));

                    if (std::find(base5_commands.begin(), base5_commands.end(), fullcom) != base5_commands.end()) ///Check if the command exists in available commands
                    {
                        rl_combo++;
                        SPDLOG_DEBUG("rl_combo: {}", rl_combo);

                        ///Clear user input
                        rhythmController->commandInput.clear();

                        ///Push the amount of perfect hits to the table and reset them
                        rhythmController->perfects.push_back(rhythmController->perfect);

                        current_perfect = rhythmController->perfect;
                        rhythmController->perfect = 0;
                        rhythmController->command_perfects.clear();

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

                int fullcom = 0;

                for (unsigned int i = 0; i < rhythmController->commandInput.size(); i++)
                {
                    fullcom += pow(rhythmController->commandInput[i], 7-i);
                }
                
                unsigned int index = distance(base5_commands.begin(), find(base5_commands.begin(), base5_commands.end(), fullcom));

                if (std::find(base5_commands.begin(), base5_commands.end(), fullcom) != base5_commands.end()) ///Check if the command exists in available commands
                {
                    updateworm = true;

                    rl_combo++;
                    SPDLOG_DEBUG("rl_combo: {}", rl_combo);
                    combo += 1;

                    if (combo >= 28)
                        combo = 12;

                    current_song = av_songs[index];

                    ///Clear user input
                    rhythmController->commandInput.clear();

                    ///Push the amount of perfect hits to the table and reset them
                    rhythmController->perfects.push_back(rhythmController->perfect);
                    current_perfect = rhythmController->perfect;
                    rhythmController->perfect = 0;

                    rhythmController->command_perfects.clear();

                    while (rhythmController->perfects.size() > 4)
                        rhythmController->perfects.erase(rhythmController->perfects.begin());

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
    */

   metronomeClick = false;
}

void Rhythm::Draw()
{
    //r_gui.doVisuals(bgm_cycle, combo);
}
