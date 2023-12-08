#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "InputController.h"
#include "../CoreManager.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <mutex>

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

void InputController::LoadKeybinds()
{
    Config* config = CoreManager::getInstance().getConfig();

    SPDLOG_INFO("Loading keybinds for InputController");

    for (int k = 0; k < 12; k++)
    {
        for (int i = 1; i <= 9; i++)
        {
            string confkey = "keybind" + keyLabels[k] + to_string(i);
            SPDLOG_TRACE("Checking confkey {}", confkey);

            if (config->keyExists(confkey))
            {
                keybinds[k].push_back(config->GetInt(confkey));
                SPDLOG_DEBUG("Loading keybind {} with key id {}", k, confkey);
            }
        }
    }
}

void InputController::parseEvents(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        ///keyMap[event.key.code] = true/false??? would that do the trick?
        SPDLOG_DEBUG("Key pressed: {}", event.key.code);

        keyRegistered = true;
        currentKey = event.key.code;

        keyMap[event.key.code] = true;
        keyMapHeld[event.key.code] = true;
    }

    if (event.type == sf::Event::KeyReleased)
    {
        SPDLOG_DEBUG("Key released: {}", event.key.code);

        keyMapHeld[event.key.code] = false;
    }

    /** Joystick buttons need to be somewhat manually assigned **/

    if (event.type == sf::Event::JoystickButtonPressed)
    {
        if (event.joystickButton.joystickId == 0)
        {
            SPDLOG_DEBUG("Joystick ({}) key pressed: {}", event.joystickButton.joystickId, event.joystickButton.button);

            keyRegistered = true;
            currentKey = 1000 + event.joystickButton.button;
            keyMap[1000 + event.joystickButton.button] = true;
            keyMapHeld[1000 + event.joystickButton.button] = true;
        }
    }

    if (event.type == sf::Event::JoystickButtonReleased)
    {
        if (event.joystickButton.joystickId == 0)
        {
            SPDLOG_DEBUG("Joystick ({}) key released: {}", event.joystickButton.joystickId, event.joystickButton.button);

            keyMapHeld[1000 + event.joystickButton.button] = false;
        }
    }

    if (event.type == sf::Event::JoystickMoved)
    {
        if (event.joystickMove.joystickId == 0)
        {
            if (event.joystickMove.axis == sf::Joystick::PovX)
            {
                if (event.joystickMove.position == -100) ///left
                {
                    keyRegistered = true;
                    currentKey = 1100;
                    keyMap[1100] = true;
                    keyMapHeld[1100] = true;
                } else
                {
                    keyMapHeld[1100] = false;
                }

                if (event.joystickMove.position == 100) ///right
                {
                    keyRegistered = true;
                    currentKey = 1101;
                    keyMap[1101] = true;
                    keyMapHeld[1101] = true;
                } else
                {
                    keyMapHeld[1101] = false;
                }
            }

            if (event.joystickMove.axis == sf::Joystick::PovY)
            {
                if (event.joystickMove.position == -100) ///down
                {
                    keyRegistered = true;
                    currentKey = 1102;
                    keyMap[1102] = true;
                    keyMapHeld[1102] = true;
                } else
                {
                    keyMapHeld[1102] = false;
                }

                if (event.joystickMove.position == 100) ///up
                {
                    keyRegistered = true;
                    currentKey = 1103;
                    keyMap[1103] = true;
                    keyMapHeld[1103] = true;
                } else
                {
                    keyMapHeld[1103] = false;
                }
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

    for (unsigned int i = 0; i < keybinds[keyID].size(); i++)
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

        mtx.lock();
        if (keyMap[realKey] == true)
        {
            keyMap[realKey] = false;
            mtx.unlock();

            return true;
        }
        mtx.unlock();
    }

    return false;
}

bool InputController::isKeyHeld(int keyID, int restrictMode)
{
    for (unsigned int i = 0; i < keybinds[keyID].size(); i++)
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
    mtx.lock();
    keyMap.clear();
    mtx.unlock();

    keyRegistered = false;
    currentKey = -1;
}
