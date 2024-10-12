#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 

#include "InputController.h"
#include "../CoreManager.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <mutex>

using namespace std;
using namespace std::chrono;

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

void InputController::addKeyPressMessage(int keyID, bool state)
{
    mtx.lock();
    Input::KeyPressMessage message;
    message.keyCode = keyID;
    message.state = state;
    message.timestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    SPDLOG_DEBUG("Added new KeyPressMessage: key {}, state {}, timestamp {}", message.keyCode, message.state, message.timestamp);

    messages.push_back(message);
    mtx.unlock();
}

std::vector<Input::KeyPressMessage> InputController::fetchKeyPressMessages()
{
    return messages;
}

void InputController::cleanExpiredMessages()
{
    mtx.lock();
    // cleanup expired messages
    int messageExpirationTime = 100; // milliseconds. set at 100ms because windows couldnt handle 33ms
    uint64_t currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    messages.erase(std::remove_if(messages.begin(), messages.end(), 
                       [currentTime, messageExpirationTime](Input::KeyPressMessage m) { return m.timestamp + messageExpirationTime < currentTime; }), messages.end());
    mtx.unlock();
}

bool InputController::processKeyPressMessages(int keyID)
{
    /*std::vector<Input::KeyPressMessage> last_messages = fetchKeyPressMessages();
    
    if(last_messages.size() > 0)
        SPDLOG_DEBUG("Processing {} key press messages", last_messages.size());

    for( auto message : last_messages )
    {
        if(message.state)
        {   
            keyMap[message.keyCode] = true;
            keyMapHeld[message.keyCode] = true;
            keyRegistered = true;
        }
        else
        {
            keyMap[message.keyCode] = false;
            keyMapHeld[message.keyCode] = false;
        }
    }*/

    cleanExpiredMessages();

    mtx.lock();
    for( unsigned int i=0; i<messages.size(); i++ )
    {
        if(messages[i].keyCode == keyID && messages[i].state)
        {
            messages.erase(messages.begin() + i);
            mtx.unlock();
            return true;
        }
    }
    mtx.unlock();

    return false;
}

void InputController::processKeyHolds()
{
    mtx.lock();
    for( unsigned int i=0; i<messages.size(); i++ )
    {
        if(messages[i].state)
        {
            keyMapHeld[messages[i].keyCode] = true;
        }
        else
        {
            keyMapHeld[messages[i].keyCode] = false;
        }
    }
    mtx.unlock();
}

void InputController::parseEvents(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        addKeyPressMessage(event.key.code, true);
    }

    if (event.type == sf::Event::KeyReleased)
    {
        addKeyPressMessage(event.key.code, false);
    }

    /** Joystick buttons need to be somewhat manually assigned **/

    /* TODO: figure out how to implement it into the new system
    */

    std::unordered_map<int, int> ds4map = {
        {3, 0},  //pata
        {1, 23}, //pon
        {0, 25}, //don
        {2, 18}, //chaka
        {4, 16}, //ltrig
        {5, 4}, //rtrig
        {9, 58}, //options (start)
        {8, 57}  //share (select)
    };

    if(CoreManager::getInstance().getConfig()->GetInt("controllerType") == 1)
    {
        ds4map = {
            {3, 0},  //pata
            {2, 23}, //pon
            {1, 25}, //don
            {4, 18}, //chaka
            {5, 16}, //ltrig
            {6, 4}, //rtrig
            {8, 58}, //options (start)
            {7, 57}  //share (select)
        };
    }

    if (event.type == sf::Event::JoystickButtonPressed)
    {
        if (event.joystickButton.joystickId == 0)
        {
            SPDLOG_DEBUG("Joystick ({}) key pressed: {}", event.joystickButton.joystickId, event.joystickButton.button);

            addKeyPressMessage(ds4map[event.joystickButton.button], true);
        }
    }

    if (event.type == sf::Event::JoystickButtonReleased)
    {
        if (event.joystickButton.joystickId == 0)
        {
            SPDLOG_DEBUG("Joystick ({}) key released: {}", event.joystickButton.joystickId, event.joystickButton.button);

            addKeyPressMessage(ds4map[event.joystickButton.button], false);
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
    cleanExpiredMessages();

    return fetchKeyPressMessages().size() > 0;
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
        
        if( processKeyPressMessages(realKey) )
            return true;
    }

    return false;
}

bool InputController::isKeyHeld(int keyID, int restrictMode)
{
    processKeyHolds();

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
