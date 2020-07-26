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

    std::vector<std::string> keyLabels = {"Square", "Circle", "Cross", "Triangle", "Left", "Right", "Down", "Up", "LTrigger", "RTrigger", "Start", "Select"};

    std::map<int,bool> keyMap;
    std::map<int,bool> keyMapHeld;

    std::map<int, std::vector<int>> keybinds;

    InputController();

    void LoadKeybinds(Config& config);
    int translateKeybind(int keyID);
    bool isKeyPressed(int keyID);
    bool isKeyHeld(int keyID);
    void Flush(); ///Flush the currently pressed controls
};

#endif // INPUTCONTROLLER_H
