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
        int rl_inputs = 0;

        for(auto beat : beat_data)
        {
            result += drum_values[beat] * pow(5, power);
            power--;
            rl_inputs += (drum_values[beat] <= 3);
        }

        base5_commands.push_back(result);
        rl_input_commands.push_back(rl_inputs);
        SPDLOG_DEBUG("Added result quinary value: {}", result);
    }

    //CoreManager::getInstance().getConfig()->LoadConfig(CoreManager::getInstance().getConfig()->thisCore);
}

void Rhythm::Stop()
{
    s_theme[0].stop();
    s_theme[1].stop();
    s_fever_fail.stop();  ///Dying fever sound
    s_fever_start.stop(); ///FEVER!
}
void Rhythm::LoadTheme(string theme)
{
    //TO-DO: uncomment this later
    //low_range = CoreManager::getInstance().getConfig()->Get<int>("lowRange");
    //high_range = CoreManager::getInstance().getConfig()->Get<int>("highRange");
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

    combo = 0;

    ///restart values
    current_perfect = 0;
    advanced_prefever = false;
    updateworm = false;

    satisfaction_value.clear();
}

void Rhythm::PlaySong(SongController::SongType songType)
{
    song_channel = (song_channel + 1) % 2;
    SPDLOG_DEBUG("Playing songType: {}, song_channel: {}", static_cast<int>(songType), song_channel);

    SongController* songController = CoreManager::getInstance().getSongController();
    
    s_theme[song_channel].stop();
    s_theme[song_channel].setBuffer(songController->getSong(songType));
    s_theme[song_channel].setVolume(float(CoreManager::getInstance().getConfig()->Get<int>("masterVolume")) * (float(CoreManager::getInstance().getConfig()->Get<int>("bgmVolume")) / 100.f));
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
    hitAllowed = true;

    SongController* songController = CoreManager::getInstance().getSongController();
    songController->flushOrder();

    // additional handling for certain combo breaks
    switch(reason)
    {
        case 7:
            command.clear();
            rhythmController->commandInputProcessed.clear();
            break;
        case 8:
            command.clear();
            rhythmController->commandInput.clear();
            rhythmController->commandInputProcessed.clear();
            break;
        default:
            break;
    }

    SPDLOG_DEBUG("Combo break! Reason code: #{}", reason);
}

int Rhythm::GetCombo()
{
    return combo;
}

float Rhythm::getAccRequirement(int combo) {
    if(combo <= 10) {
        return acc_req[combo];
    }

    return 0;
}

void Rhythm::decideSongType()
{
    RhythmController* rhythmController = CoreManager::getInstance().getRhythmController();
    SongController* songController = CoreManager::getInstance().getSongController();

    float perfection = rhythmController->rl_input_perfection; // Accuracy, float between 0 and 1

    if(perfection == 1)
        current_perfect += 1; // current_perfect = amount of consecutive perfect commands
    else
        current_perfect = 0;

    satisfaction_value.push_back(perfection);

    if (satisfaction_value.size() > 3)
        satisfaction_value.erase(satisfaction_value.begin());

    satisfaction = 0;

    if (satisfaction_value.size() == 3)
        satisfaction = (satisfaction_value[2] * 0.8 + satisfaction_value[1] * 0.15 + satisfaction_value[0] * 0.05);
    if (satisfaction_value.size() == 2)
        satisfaction = (satisfaction_value[1] * 0.75 + satisfaction_value[0] * 0.25);
    if (satisfaction_value.size() == 1)
        satisfaction = satisfaction_value[0];

    SPDLOG_DEBUG("Satisfaction: {}, requirement: {}, current_perfect: {}, theme_combo: {}", satisfaction, getAccRequirement(combo), current_perfect, combo);
    SPDLOG_DEBUG("Accuracy: {}%", perfection * 100);

    // If combo == 0, we are in idle loop. No commands made
    if(combo == 0)
    {
        currentSongType = SongController::SongType::IDLE;
        SPDLOG_DEBUG("I am deciding: IDLE");
        addRhythmMessage(RhythmAction::SONG_TYPE_CHANGED, to_string(currentSongType));
        return;
    }

    if(combo == 1) {
        currentSongType = SongController::SongType::PREFEVER_CALM;
        SPDLOG_DEBUG("I am deciding: PREFEVER_CALM");
        addRhythmMessage(RhythmAction::SONG_TYPE_CHANGED, to_string(currentSongType));
        return;
    }

    SPDLOG_DEBUG("Current song type: {}, advanced_prefever: {}", currentSongType, advanced_prefever);
    if((currentSongType != SongController::SongType::FEVER) && (currentSongType != SongController::SongType::FEVER_START)) {
        if(advanced_prefever) {
            if(satisfaction <= getAccRequirement(combo) * 0.75) {
                advanced_prefever = false;
                currentSongType = SongController::SongType::PREFEVER_CALM;
                songController->flushOrder();
                SPDLOG_DEBUG("Poor timing! Go back to PREFEVER_CALM");
                addRhythmMessage(RhythmAction::SONG_TYPE_CHANGED, to_string(currentSongType));
                return;
            }
        }

        if(combo >= 2) {
            if(satisfaction >= getAccRequirement(combo)) {
                if(!advanced_prefever) {
                    advanced_prefever = true;
                    currentSongType = SongController::SongType::PREFEVER_INTENSE_START;
                    SPDLOG_DEBUG("Great! Go to PREFEVER_INTENSE_START");
                    addRhythmMessage(RhythmAction::SONG_TYPE_CHANGED, to_string(currentSongType));
                    return;
                } else {
                    currentSongType = SongController::SongType::FEVER_START;
                    SPDLOG_DEBUG("Awesome! Go to FEVER_START");
                    addRhythmMessage(RhythmAction::SONG_TYPE_CHANGED, to_string(currentSongType));
                    return;
                }
            }
        }

        if(currentSongType == SongController::SongType::PREFEVER_INTENSE_START) {
            currentSongType = SongController::SongType::PREFEVER_INTENSE;
            SPDLOG_DEBUG("PREFEVER_INTENSE_START -> PREFEVER_INTENSE");
            addRhythmMessage(RhythmAction::SONG_TYPE_CHANGED, to_string(currentSongType));
            return;
        }

        if(!advanced_prefever) {
            currentSongType = SongController::SongType::PREFEVER_CALM;
            SPDLOG_DEBUG("No choice made, also no advanced prefever. Choosing PREFEVER_CALM"); 
            addRhythmMessage(RhythmAction::SONG_TYPE_CHANGED, to_string(currentSongType));
            return;
        } else {
            currentSongType = SongController::SongType::PREFEVER_INTENSE;
            SPDLOG_DEBUG("No choice made, advanced prefever set. Chooding PREFEVER_INTENSE");
            addRhythmMessage(RhythmAction::SONG_TYPE_CHANGED, to_string(currentSongType)); 
            return;
        }
    }

    if(currentSongType == SongController::SongType::FEVER_START) {
        currentSongType = SongController::SongType::FEVER;
        SPDLOG_DEBUG("FEVER_START -> FEVER");
        addRhythmMessage(RhythmAction::SONG_TYPE_CHANGED, to_string(currentSongType));
        return;
    }
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
    rhythmController->masterTimerNoAbs = rhythmClockValue % int(beat_timer) - (beat_timer/2);
    rhythmController->base5_commands = base5_commands;
    rhythmController->rl_input_commands = rl_input_commands;

    rhythmController->low_range = low_range;
    rhythmController->high_range = high_range;

    if(rhythmController->checkForInput())
    {
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

        if(!hitAllowed)
        {
            BreakCombo(8);
        }
        else
        {
            hitAllowed = false;
            afterPressClock.restart();
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

            if(!command.empty())
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

    // allow hit after minimal timeout
    // we don't want to block presses that were made in the last allowed microsecond on GOOD range
    // so we wait the amount of time in the BAD hit timeframe (times two for both sides, remember: metronome tick starts at 0ms, - is early, + is late)
    if(afterPressClock.getElapsedTime().asMicroseconds() >= low_range*2)
    {
        hitAllowed = true;
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

    if(debug_controls) {
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
    }

    metronomeClick = false;
}

void Rhythm::toggleDebug() {
    debug_controls = !debug_controls;
}