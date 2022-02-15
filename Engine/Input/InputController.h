#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "../Config.h"
#include <map>

class InputController
{
public:
    enum Keys
    {
        SQUARE = 0,
        CIRCLE = 1,
        CROSS = 2,
        TRIANGLE = 3,
        LEFT = 4,
        RIGHT = 5,
        DOWN = 6,
        UP = 7,
        LTRIGGER = 8,
        RTRIGGER = 9,
        START = 10,
        SELECT = 11
    };

    enum RestrictMode
    {
        OFF = 0,
        ONLYKEYBOARD = 1,
        ONLYJOYSTICK = 2
    };

    std::vector<std::string> keyLabels = {"Square", "Circle", "Cross", "Triangle", "Left", "Right", "Down", "Up", "LTrigger", "RTrigger", "Start", "Select"};

    std::map<int, bool> keyMap;
    std::map<int, bool> keyMapHeld;

    std::map<int, std::vector<int>> keybinds;

    ///For Any Key presses
    bool keyRegistered = false; //checks if any key is pressed
    int currentKey = -1;        //reads what key has been pressed

    InputController();

    void LoadKeybinds(Config& config);
    void parseEvents(sf::Event& event);
    int translateKeybind(int keyID);
    bool isAnyKeyPressed();
    int whatKeyPressed(int restrictMode = 0);
    bool isKeyPressed(int keyID, int restrictMode = 0);
    bool isKeyHeld(int keyID, int restrictMode = 0);
    void Flush(); ///Flush the currently pressed controls
};

#endif // INPUTCONTROLLER_H
