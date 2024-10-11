#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "RhythmGUI.h"
#include <cmath>
#include <iostream>
#include <spdlog/spdlog.h>
#include "../CoreManager.h"

using namespace std;
using namespace std::chrono;

RhythmGUI::RhythmGUI()
{
    //ctor
    r_rhythm.setSize(sf::Vector2f(100, 100));
    r_rhythm.setFillColor(sf::Color::White);

    lastRhythmCheck = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    auto strRepo = CoreManager::getInstance().getStrRepo();

    d_text.setFont(strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage()));
    d_text.setColor(sf::Color::Black);
    d_text.setCharacterSize(20);
}

void RhythmGUI::click()
{
    beatClock.restart();
}

void RhythmGUI::doVisuals(int bgm_cycle, int combo)
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    auto lastView = window->getView();

    window->setView(window->getDefaultView());

    int v_cycle_mode = abs(floor((bgm_cycle) / 4) - 1);
    int v_cycle = (bgm_cycle) % 4 + 1;

    float beatClockTime = float(beatClock.getElapsedTime().asMilliseconds()) / float(beat_timer) * 255; 
    int rhythmAlpha = 255 - beatClockTime;
    
    if(rhythmAlpha <= 0)
    rhythmAlpha = 0;

    ///Visuals
    ///Calculate the ratio for other window sizes (default is 1280x720)
    float ratio_X = window->getSize().x / float(1280);
    float ratio_Y = window->getSize().y / float(720);
    float ratio_universal = (window->getSize().x * window->getSize().y) / (float(1280) * float(720));

    /// Beat frame
    if ((combo <= 1) || ((combo > 1) && (combo < 11) && (v_cycle_mode == 0)))
    {
        r_rhythm.setFillColor(sf::Color(0, 0, 0, 0));
        r_rhythm.setOutlineThickness(-ceil(3 * ratio_universal));
        r_rhythm.setOutlineColor(sf::Color(255, 255, 255, rhythmAlpha));
        r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (24 * ratio_X), (720 * ratio_Y) - (24 * ratio_Y)));
        r_rhythm.setPosition(12 * ratio_X, 12 * ratio_Y);

        r_rhythm2.setOutlineColor(sf::Color(0, 0, 0, 0));
    } else if ((combo > 1) && (v_cycle_mode == 1))
    {
        r_rhythm.setFillColor(sf::Color(0, 0, 0, 0));
        r_rhythm.setOutlineThickness(-ceil(2 * ratio_universal));
        r_rhythm.setOutlineColor(sf::Color(64, 64, 64, rhythmAlpha));
        r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (20 * ratio_X), (720 * ratio_Y) - (20 * ratio_Y)));
        r_rhythm.setPosition(10 * ratio_X, 10 * ratio_Y);

        r_rhythm2.setFillColor(sf::Color(0, 0, 0, 0));
        r_rhythm2.setOutlineThickness(-ceil(2 * ratio_universal));
        r_rhythm2.setOutlineColor(sf::Color(64, 64, 64, rhythmAlpha));
        r_rhythm2.setSize(sf::Vector2f((1280 * ratio_X) - (30 * ratio_X), (720 * ratio_Y) - (30 * ratio_Y)));
        r_rhythm2.setPosition(15 * ratio_X, 15 * ratio_Y);

        if (v_cycle == 4)
        {
            if (floor(flicker) == 0)
            {
                r_rhythm.setOutlineColor(sf::Color(64, 64, 64, rhythmAlpha));
                r_rhythm2.setOutlineColor(sf::Color(64, 64, 64, rhythmAlpha));
            } else if (floor(flicker) == 1)
            {
                r_rhythm.setOutlineColor(sf::Color(220, 220, 220, rhythmAlpha));
                r_rhythm2.setOutlineColor(sf::Color(220, 220, 220, rhythmAlpha));
            }

            flicker += float(1) / fps * 30;

            if (flicker >= 2)
                flicker = 0;
        }
    } else if ((combo >= 11) && (v_cycle_mode == 0))
    {
        r_rhythm.setFillColor(sf::Color(0, 0, 0, 0));
        r_rhythm.setOutlineThickness(-ceil(7 * ratio_universal));

        r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (20 * ratio_X), (720 * ratio_Y) - (20 * ratio_Y)));
        r_rhythm.setPosition(10 * ratio_X, 10 * ratio_Y);

        r_rhythm2.setOutlineColor(sf::Color(0, 0, 0, 0));

        int flick = floor(flicker);

        switch (flick)
        {
            case 0:
                r_rhythm.setOutlineColor(sf::Color(255, 255, 0, rhythmAlpha));
                break;

            case 1:
                r_rhythm.setOutlineColor(sf::Color(255, 255, 255, rhythmAlpha));
                break;

            case 2:
                r_rhythm.setOutlineColor(sf::Color(0, 255, 255, rhythmAlpha));
                break;

            case 3:
                r_rhythm.setOutlineColor(sf::Color(0, 255, 0, rhythmAlpha));
                break;
        }

        flicker += float(1) / fps * 30;

        if (flicker >= 4)
            flicker = 0;
    }

    float sizeMod = rhythmAlpha / float(80);
    beatBounce = sizeMod / 30;

    window->draw(r_rhythm);
    window->draw(r_rhythm2);

    Rhythm* rhythm = CoreManager::getInstance().getRhythm();

    std::vector<Rhythm::RhythmMessage> messages = rhythm->fetchRhythmMessages(lastRhythmCheck);

    uint64_t lastDebugTimestamp = 0;
    if(rhythm->messages.size() > 0)
    {
        lastDebugTimestamp = rhythm->messages[rhythm->messages.size()-1].timestamp;
    }

    SPDLOG_TRACE("Checked rhythm messages. Last check set to {}, size {}, last message timestamp {}", lastRhythmCheck, messages.size(), lastDebugTimestamp);
    
    if(messages.size() > 0)
    {
        SPDLOG_DEBUG("Received {} rhythm messages", messages.size());
    }

    for(unsigned int i=0; i<messages.size(); i++)
    {
        SPDLOG_DEBUG("message i={}, action: {}, message: {}, timestamp {}", i, messages[i].action, messages[i].message, messages[i].timestamp);

        //messages[i].action directly translates into drums BEST, GOOD and BAD
        Rhythm::RhythmAction action = messages[i].action;

        if(action == Rhythm::RhythmAction::DRUM_BEST || action == Rhythm::RhythmAction::DRUM_GOOD || action == Rhythm::RhythmAction::DRUM_BAD)
        {
            std::vector<std::string> params = Func::Split(messages[i].message, ' ');

            std::string drum_name = params[0];
            int drum_pattern = atoi(params[1].c_str());

            std::string drum_path = "resources/graphics/rhythm/drums/" + drum_name + ".png";

            Drum temp;
            temp.Load(drum_name, action, drum_path);
            temp.pattern = drum_pattern;
            drums.push_back(temp);
        }
    }

    if(debugToggle) {
        /*
        BPM: {}, beat_timer: {}, beat_ms: {}, halfbeat_ms: {}, measure_ms: {}
        low_range: {}, high_range: {}
        combo: {}
        accuracy: {}%, satisfaction: {}, requirement: {}, current_perfect: {}
        currentSongType: {}, song_channel: {}, advanced_prefever: {}
        drumTicks: {}, drumTicksNoInput: {}, measureCycle: {}
        commandWaitClock: {}ms, firstCommandDelayClock: {}ms, afterMeasureClock: {}ms, newRhythmClock: {}us
        metronomeVal: {}, metronomeOldVal, metronomeState: {}
        rhythmMessages:
        ...
        ...
        ...
        */

        Rhythm* rhythm = CoreManager::getInstance().getRhythm();
        RhythmController* rhythmController = CoreManager::getInstance().getRhythmController();

        std::vector<std::string> songTypes = {"IDLE", "PREFEVER_CALM", "PREFEVER_INTENSE", "FEVER", "START", "PREFEVER_INTENSE_START", "FEVER_START"};

        std::string debug_text = "";
        debug_text += std::format("BPM: {}, beat_timer: {}us, beat_ms: {}ms, halfbeat_ms: {}ms, measure_ms: {}ms\n", rhythm->BPM, rhythm->beat_timer, rhythm->beat_ms, rhythm->halfbeat_ms, rhythm->measure_ms);
        debug_text += std::format("low_range: {}us, high_range: {}us\n", rhythm->low_range, rhythm->high_range);
        debug_text += std::format("combo: {}\n", rhythm->GetCombo());
        debug_text += std::format("accuracy: {}%, satisfaction: {}, requirement: {}, for insta fever: {}, current_perfect: {}\n", rhythmController->rl_input_perfection*100.f, rhythm->satisfaction, rhythm->getAccRequirement(rhythm->combo), rhythm->getAccRequirementFever(rhythm->combo), rhythm->current_perfect);
        debug_text += std::format("currentSongType: {}, song_channel: {}, advanced_prefever: {}\n", songTypes[static_cast<int>(rhythm->currentSongType)], rhythm->song_channel, rhythm->advanced_prefever);
        debug_text += std::format("drumTicks: {}, drumTicksNoInput: {}, measureCycle: {}\n", rhythm->drumTicks, rhythm->drumTicksNoInput, rhythm->measureCycle);
        debug_text += std::format("commandWaitClock: {}ms, firstCommandDelayClock: {}ms\nafterMeasureClock: {}ms, afterPerfectClock: {}ms, newRhythmClock: {}us\n", rhythm->commandWaitClock.getElapsedTime().asMilliseconds(), rhythm->firstCommandDelayClock.getElapsedTime().asMilliseconds(), rhythm->afterMeasureClock.getElapsedTime().asMilliseconds(), rhythm->afterPerfectClock.getElapsedTime().asMilliseconds(), rhythm->newRhythmClock.getElapsedTime().asMicroseconds()); 
        debug_text += std::format("metronomeVal: {}, metronomeOldVal: {}, metronomeState: {}\n", rhythm->metronomeVal, rhythm->metronomeOldVal, rhythm->metronomeState);
        debug_text += std::format("masterTimer: {}, masterTimerNoAbs: {}\n", rhythmController->masterTimer, rhythmController->masterTimerNoAbs);
        debug_text += "command input: ";
        for(auto c : rhythmController->commandInput)
        {
            debug_text += to_string(c);
        }
        debug_text += "\n";
        debug_text += std::format("can hit? (double input prevention): {}", rhythm->hitAllowed);

        auto &view = window->getView();
        window->setView(window->getDefaultView());

        d_text.disable_processing = true;
        d_text.setString(debug_text);
        d_text.draw();

        d_green.setSize(sf::Vector2f(250, 40));
        d_green.setFillColor(sf::Color::Green);
        d_orange.setFillColor(sf::Color(255, 128, 0, 255));
        d_red.setFillColor(sf::Color::Red);

        float o_width = ((rhythm->high_range-rhythm->low_range) / rhythm->beat_timer) * 250;
        float r_width = (rhythm->low_range / rhythm->beat_timer) * 250;

        d_marker.setSize(sf::Vector2f(2, 60));
        d_marker.setFillColor(sf::Color(0, 64, 255, 255));

        d_orange.setSize(sf::Vector2f(o_width, 40));
        d_red.setSize(sf::Vector2f(r_width, 40));

        d_green.setPosition(window->getSize().x/2-125, window->getSize().y-80);
        window->draw(d_green);

        d_red.setPosition(window->getSize().x/2-125, window->getSize().y-80);
        window->draw(d_red);
        d_red.setPosition(window->getSize().x/2+125-r_width, window->getSize().y-80);
        window->draw(d_red);

        d_orange.setPosition(window->getSize().x/2-125+r_width, window->getSize().y-80);
        window->draw(d_orange);
        d_orange.setPosition(window->getSize().x/2+125-r_width-o_width, window->getSize().y-80);
        window->draw(d_orange);

        int marker_alpha = 25;
        for(auto x : rhythmController->drum_hits) {
            float marker_pos = x;
            if(marker_pos < 0)
                marker_pos += rhythm->beat_timer / 2;
            else
                marker_pos -= rhythm->beat_timer / 2;
            
            marker_pos = (marker_pos / rhythm->beat_timer) * 250;

            d_marker.setPosition((window->getSize().x/2-1) + marker_pos, window->getSize().y-90);
            d_marker.setFillColor(sf::Color(0, 64, 255, marker_alpha));
            window->draw(d_marker);
            marker_alpha += 22;
        }

        window->setView(view);
    }

    std::vector<int> drumsToErase;

    for (int i = 0; i < drums.size(); i++)
    {
        drums[i].Draw();

        if (drums[i].alpha <= 0)
        {
            drumsToErase.push_back(i);
        }
    }

    for (int i = 0; i < drumsToErase.size(); i++)
    {
        SPDLOG_DEBUG("Erased drum {}", drumsToErase[i] - i);
        drums.erase(drums.begin() + (drumsToErase[i] - i));
    }

    window->setView(lastView);
}

void RhythmGUI::toggleDebugUI() {
    debugToggle = !debugToggle;
}

//void RhythmGUI::Update(sf::RenderWindow &window, float fps){

//}

RhythmGUI::~RhythmGUI()
{
    //dtor
}
