#include <iostream>
#include <sstream>
#include "Rhythm.h"
#include "SongController.h"
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

    b_fever_start.loadFromFile("resources/sfx/bgm/fever_start.ogg");
    b_fever_fail.loadFromFile("resources/sfx/bgm/fever_fail.ogg");
}

void Rhythm::LoadTheme(string theme)
{
    ///Load the BGM
    songController = new SongController();
    songController->LoadSongByName(theme);
    this->currentThemeName = theme;

    ///Play the BGM beginning after loading
    s_theme[0].setBuffer(songController->GetSongByNumber(0,0));
    s_theme[0].play();

    ///Restart the Rhythm clocks
    rhythmClock.restart();

    canPlay = false;
}

void Rhythm::BreakCombo()
{
    s_chant.stop();
    if(combo >= 11) ///Play dying fever sound when BGM is in fever state
    {
        ///Dying fever sound
        s_fever_fail.setBuffer(b_fever_fail);
        s_fever_fail.play();
    }

    ///Reset Rhythm Clock
    rhythmClock.restart();

    ///Reset combo to idle BGM point
    combo = 1;

    ///Reset Perfects table
    rhythmController.perfect = 0;
    rhythmController.perfects.clear();

    ///Reset command input
    rhythmController.commandInput.clear();

    ///Reset BGM
    s_theme[0].stop();
    s_theme[1].stop();

    ///Play BGM from the combo of idle point
    s_theme[0].setBuffer(songController->GetSongByNumber(0,combo));
    s_theme[0].play();

    current_song = "";
}

void Rhythm::checkRhythmController(sf::RenderWindow& window)
{
    ///RHYTHM CONTROLLER SETUP
    rhythmController.combo = combo;

    rhythmController.masterTimer = rhythmClock.getElapsedTime().asMilliseconds();
    rhythmController.av_commands = av_commands;

    rhythmController.low_range = low_range;
    rhythmController.high_range = high_range;

    if(rhythmController.checkForInput())
    {
        Drum temp;
        temp.Load(rhythmController.drumToLoad,rhythmController.drum_perfection,window);
        temp.pattern = rhythmController.currentPattern;
        drums.push_back(temp);

        ///Restart the Timeout and Before Fever clocks every time a drum is being hit
        commandTimeout.restart();
        beforeFeverClock.restart();

        if(rhythmController.breakCombo)
        BreakCombo();
    }

    rhythmController.resetValues();
}

///TO BE PORTED TO AN EXTERNAL CLASS
void Rhythm::doVisuals(sf::RenderWindow& window)
{
    auto lastView = window.getView();

    window.setView(window.getDefaultView());

    ///Patch for unusable command and beat values
    int totalComValue = ((commandValue - 1) * 4) + beatValue - 1;
    int usableCommandValue = floor(totalComValue / float(5));
    int usableBeatValue = floor(totalComValue - (usableCommandValue * 4));
    usableCommandValue += 1;

    /**
    clock250 - full white
    clock400 - faded white
    clock
    */

    int rhythmAlpha = 0;

    if(rhythmClock.getElapsedTime().asMilliseconds() < 250)
    rhythmAlpha = 250 - rhythmClock.getElapsedTime().asMilliseconds();

    if(rhythmClock.getElapsedTime().asMilliseconds() >= 250)
    rhythmAlpha = 500 - rhythmClock.getElapsedTime().asMilliseconds()/2;

    ///Visuals
    if(true)
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
            r_rhythm.setOutlineColor(sf::Color(255,255,255,rhythmAlpha/float(2)));
            r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (24 * ratio_X), (720 * ratio_Y) - (24 * ratio_Y)));
            r_rhythm.setPosition(12*ratio_X,12*ratio_Y);

            r_rhythm2.setOutlineColor(sf::Color(0,0,0,0));
        }
        else if((combo > 1) and (usableCommandValue == 1))
        {
            r_rhythm.setFillColor(sf::Color(0,0,0,0));
            r_rhythm.setOutlineThickness(-ceil(2 * ratio_universal));
            r_rhythm.setOutlineColor(sf::Color(64,64,64,rhythmAlpha/float(2)));
            r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (20 * ratio_X), (720 * ratio_Y) - (20 * ratio_Y)));
            r_rhythm.setPosition(10*ratio_X,10*ratio_Y);

            r_rhythm2.setFillColor(sf::Color(0,0,0,0));
            r_rhythm2.setOutlineThickness(-ceil(2 * ratio_universal));
            r_rhythm2.setOutlineColor(sf::Color(64,64,64,rhythmAlpha/float(2)));
            r_rhythm2.setSize(sf::Vector2f((1280 * ratio_X) - (30 * ratio_X), (720 * ratio_Y) - (30 * ratio_Y)));
            r_rhythm2.setPosition(15*ratio_X,15*ratio_Y);

            if(usableBeatValue == 4)
            {
                if(floor(flicker) == 0)
                {
                    r_rhythm.setOutlineColor(sf::Color(64,64,64,rhythmAlpha/float(2)));
                    r_rhythm2.setOutlineColor(sf::Color(64,64,64,rhythmAlpha/float(2)));
                }
                else if(floor(flicker) == 1)
                {
                    r_rhythm.setOutlineColor(sf::Color(220,220,220,rhythmAlpha/float(2)));
                    r_rhythm2.setOutlineColor(sf::Color(220,220,220,rhythmAlpha/float(2)));
                }

                flicker += float(1) / fps * 30;

                if(flicker >= 2)
                flicker = 0;
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
                r_rhythm.setOutlineColor(sf::Color(255,255,0,rhythmAlpha/float(2)));
            }
            else if(floor(flicker) == 1)
            {
                r_rhythm.setOutlineColor(sf::Color(255,255,255,rhythmAlpha/float(2)));
            }
            else if(floor(flicker) == 2)
            {
                r_rhythm.setOutlineColor(sf::Color(0,255,255,rhythmAlpha/float(2)));
            }
            else if(floor(flicker) == 3)
            {
                r_rhythm.setOutlineColor(sf::Color(0,255,0,rhythmAlpha/float(2)));
            }

            flicker += float(1) / fps * 30;

            if(flicker >= 4)
            flicker = 0;
        }

        /// Fever meter - now with 16 segments!
        int feverMeterWidth = 10*(combo-12);
        if(feverMeterWidth > 160)
        {
            feverMeterWidth = 160;
        }

        int sizeMod = rhythmAlpha/float(160);

        r_fever.setSize(sf::Vector2f(160+sizeMod*2,10+sizeMod*2));
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

    window.setView(lastView);
}

void Rhythm::Draw(sf::RenderWindow& window)
{
    checkRhythmController(window);
    doVisuals(window);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        LoadTheme("Freakout Rock Theme");
    }

    if(rhythmClock.getElapsedTime().asMilliseconds() > 495)
    {
        cycle++;
        cout << "cycle: " << cycle << " mode: " << cycle_mode << endl;

        if(rhythmController.hit)
        {
            cout << "good" << endl;
            rhythmController.hit = false;
        }
        else
        {
            ///Reset Perfects table
            rhythmController.perfect = 0;
            rhythmController.perfects.clear();

            ///Reset command input
            rhythmController.commandInput.clear();
        }

        rhythmController.drumAlreadyHit = false;

        rhythmClock.restart();
    }

    if(rhythmClock.getElapsedTime().asMilliseconds() > 245)
    {
        if(cycle >= 4)
        {
            cycle = 0;

            if(combo == 0)
            combo = 1;

            cycle_mode += 1;
            cycle_mode = cycle_mode%2;

            if(cycle_mode == 1)
            {
                ///Play the BGM
                s_theme[combo%2].setBuffer(songController->GetSongByNumber(0,combo));

                s_theme[combo%2].stop();
                s_theme[combo%2].play();

                combo += 1;

                if(combo >= 28)
                combo = 12;
            }
        }
    }
}
