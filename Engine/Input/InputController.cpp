#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "InputController.h"
#include <iostream>
#include <spdlog/spdlog.h>

using namespace std;

InputController::InputController()
{
}

/**
isKeyPressed(SQUARE); = isKeyPressed(0);
check config for key press 0 (keybindSquare)
compare with keyMap
stonks
**/

void InputController::LoadKeybinds(Config& config)
{
    for (int k = 0; k < 12; k++)
    {
        for (int i = 1; i <= 9; i++)
        {
            string confkey = "keybind" + keyLabels[k] + to_string(i);
            SPDLOG_TRACE("Checking confkey {}", confkey);

            if (config.keyExists(confkey))
            {
                keybinds[k].push_back(config.GetInt(confkey));
                SPDLOG_DEBUG("Loading keybind {} with key id {}", k, confkey);
            }
        }
    }
}

int translateKeybind(int keyID)
{
    ///for joystick translations, add 1000
    ///regular keyboard: key press 1 = 1
    ///joystick: key press 1 = 1001

    return 0;
}

///Returns true if any key has been pressed
bool InputController::isAnyKeyPressed()
{
    bool r = keyRegistered;
    keyRegistered = false;

    return r;
}

///Returns what key is being currently pressed (for input detection)
int InputController::whatKeyPressed(int restrictMode)
{
    if (keyRegistered)
    {
        keyRegistered = false;

        if (restrictMode == 1)
        {
            if (currentKey >= 1000)
            {
                return -2;
            }
        }

        if (restrictMode == 2)
        {
            if (currentKey < 1000)
            {
                return -2;
            }
        }

        return currentKey;
    } else
    {
        return -2;
    }
}

bool InputController::isKeyPressed(int keyID, int restrictMode)
{
    ///RestrictMode is for checking ONLY keyboard/joystick presses.
    ///RestrictMode = 0 - off
    ///RestrictMode = 1 - only keyboard
    ///RestrictMode = 2 - only joystick

    for (int i = 0; i < keybinds[keyID].size(); i++)
    {
        int realKey = keybinds[keyID][i];

        if (restrictMode == 1)
        {
            if (realKey >= 1000)
            {
                return false;
            }
        }

        if (restrictMode == 2)
        {
            if (realKey < 1000)
            {
                return false;
            }
        }

        if (keyMap[realKey] == true)
        {
            keyMap[realKey] = false;
            return true;
        }
    }

    return false;
}

bool InputController::isKeyHeld(int keyID, int restrictMode)
{
    for (int i = 0; i < keybinds[keyID].size(); i++)
    {
        int realKey = keybinds[keyID][i];

        if (restrictMode == 1)
        {
            if (realKey >= 1000)
            {
                return false;
            }
        }

        if (restrictMode == 2)
        {
            if (realKey < 1000)
            {
                return false;
            }
        }

        if (keyMapHeld[realKey] == true)
        {
            return true;
        }
    }

    return false;
}

void InputController::Flush()
{
    keyMap.clear();
    keyRegistered = false;
    currentKey = -1;
}
