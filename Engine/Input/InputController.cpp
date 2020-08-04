#include "InputController.h"
#include <iostream>

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
    for(int k=0; k<12; k++)
    {
        for(int i=1; i<=9; i++)
        {
            string confkey = "keybind"+keyLabels[k]+to_string(i);
            cout << "Checking confkey " << confkey << endl;

            if(config.keyExists(confkey))
            {
                keybinds[k].push_back(config.GetInt(confkey));
                cout << "Loading keybind " << k << " with key id " << confkey << endl;
            }
        }
    }
}

int translateKeybind(int keyID)
{
    ///for joystick translations, add 1000
    ///regular keyboard: key press 1 = 1
    ///joystick: key press 1 = 1001

}

///Returns true if any key has been pressed
bool InputController::isAnyKeyPressed()
{
    bool r = keyRegistered;
    keyRegistered = false;

    return r;
}

///Returns what key is being currently pressed (for input detection)
int InputController::whatKeyPressed()
{
    if(keyRegistered)
    {
        keyRegistered = false;
        return currentKey;
    }
    else
    {
        return false;
    }
}

bool InputController::isKeyPressed(int keyID)
{
    for(int i=0; i<keybinds[keyID].size(); i++)
    {
        int realKey = keybinds[keyID][i];

        if(keyMap[realKey] == true)
        {
            keyMap[realKey] = false;
            return true;
        }
    }

    return false;
}

bool InputController::isKeyHeld(int keyID)
{
    for(int i=0; i<keybinds[keyID].size(); i++)
    {
        int realKey = keybinds[keyID][i];

        if(keyMapHeld[realKey] == true)
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
