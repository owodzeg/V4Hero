#ifndef RYTHMGUI_H
#define RYTHMGUI_H

#include "../Config.h"
#include <SFML/Graphics.hpp>
#include <chrono>

#include "Drum.h"


class RhythmGUI
{
public:
    /// Visuals ///
    sf::RectangleShape r_rhythm;
    sf::RectangleShape r_rhythm2;

    float beatBounce = 0;
    float flicker = 0; ///For beat frame flickering
    float BPM = 120;
    float beat_timer = 60.f / BPM * 1000.f;
    sf::Clock beatClock;

    uint64_t lastRhythmCheck;

    /// Drums in-game ///
    std::vector<Drum> drums;

    void Initialise(Config& config, std::map<int, bool>& keymap);
    void click();
    void doVisuals(int bgm_cycle, int combo);
    RhythmGUI();
    ~RhythmGUI();
};


#endif // CAMERA_H
