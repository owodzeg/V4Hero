#include <iostream>
#include <cstdlib>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "V4Core.h"

using namespace std;

V4Core::V4Core()
{
    /** Detect when the build was compiled **/

    time_t t;

    struct stat result;
    if(stat("Patafour.exe", &result) == 0)
    {
        t = result.st_mtime;
    }

    //struct tm *st = localtime(&t);

    /*//int day = st->tm_mday;
    //int month = st->tm_mon;
    //int year = st->tm_year + 1900;

    //vector<string> months = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    //string strDay = to_string(day);

    if((day > 9) && (day < 20))
    {
        strDay += "th";
    }
    else
    {
        if(day%10 == 1)
        strDay += "st";
        else if(day%10 == 2)
        strDay += "nd";
        else if(day%10 == 3)
        strDay += "rd";
        else
        strDay += "th";
    }
*/

    /** Load config from config.cfg **/
    config.LoadConfig(this);

    /** Load save from savereader **/
    savereader.LoadSave(config);
    /** "Alpha release" text **/

    f_font.loadFromFile("resources/fonts/p4kakupop-gothic.ttf");

    t_debug.setFont(f_font);
    t_debug.setCharacterSize(24);
    t_debug.setFillColor(sf::Color::White);
    t_debug.setString(config.strRepo.GetString(L"demo_string"));//+strDay+" "+months[month]+" "+to_string(year)+".");
    t_debug.setOrigin(t_debug.getGlobalBounds().width/2,t_debug.getGlobalBounds().height/2);

    /** Initialize main menu **/

    mainMenu.Initialise(&config,&keyMap,this);
    menus.push_back(&mainMenu);
    config.configDebugID = 10;

    /// If this is a new save (no previous save data) we load up the new game menu
    newGameMenu.Initialise(&config,&keyMap,this);
    menus.push_back(&newGameMenu);
    if(savereader.isNewSave){
        mainMenu.Hide();
    } else {
        newGameMenu.Hide();
    }
}

void V4Core::Init()
{
    srand(time(NULL));

    sf::RenderWindow window;

    if(config.GetInt("enableFullscreen"))
        window.create(sf::VideoMode(config.GetInt("resX"), config.GetInt("resY")), "Patafour", sf::Style::Fullscreen);
    else if(config.GetInt("enableBorderlessWindow"))
        window.create(sf::VideoMode(config.GetInt("resX"), config.GetInt("resY")), "Patafour", sf::Style::None);
    else
        window.create(sf::VideoMode(config.GetInt("resX"), config.GetInt("resY")), "Patafour", sf::Style::Titlebar | sf::Style::Close);

    window.setFramerateLimit(config.GetInt("framerateLimit"));
    window.setKeyRepeatEnabled(false);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed)
			{
			    if(config.GetInt("showKeyboardPressCode")==1){
                    ///keyMap[event.key.code] = true/false??? would that do the trick?
                    cout << "[DEBUG] Key pressed: " << event.key.code << endl;
			    }

			    keyMap[event.key.code] = true;
			    keyMapHeld[event.key.code] = true;

			    //if (!inMission){
                    //inMission=true;
                    //currentController.StartMission();
			    //} else if(event.key.code==59) {
                //    cout<<"Returning to main menu...";
                //    inMission=false;
			    //}
			}

			if(event.type == sf::Event::KeyReleased)
            {
                if(config.GetInt("showKeyboardPressCode")==1){
                    cout << "[DEBUG] Key released: " << event.key.code << endl;
                }
                keyMapHeld[event.key.code] = false;
            }

            /**
            Joystick map (x360, ds4 is turned off by default, unsure about custom joysticks)
            0 - square
            1 - cross
            2 - circle
            3 - triangle
            4 - L1
            5 - R1
            6 - L2
            7 - R2
            8 - share (start)
            9 - options (select)
            10 - L3 (analog)
            11 - R3 (analog)
            12 - PS/XB button
            13 - DS4 touchpad
            **/

            if (event.type == sf::Event::JoystickButtonPressed)
            {
                if(event.joystickButton.joystickId == 0)
                {
                    std::cout << "[DEBUG] Joystick (" << event.joystickButton.joystickId << ") key pressed: " << event.joystickButton.button << std::endl;

                    /// TEMPORARY SOLUTION! Make joysticks mappable in future, just like keyboard. //
                    switch(event.joystickButton.button)
                    {
                        case 0:
                        {
                            keyMap[config.GetInt("keybindPata")] = true;
                            keyMapHeld[config.GetInt("keybindPata")] = true;
                            break;
                        }

                        case 1:
                        {
                            keyMap[config.GetInt("keybindDon")] = true;
                            keyMapHeld[config.GetInt("keybindDon")] = true;
                            break;
                        }

                        case 2:
                        {
                            keyMap[config.GetInt("keybindPon")] = true;
                            keyMapHeld[config.GetInt("keybindPon")] = true;
                            break;
                        }

                        case 3:
                        {
                            keyMap[config.GetInt("keybindChaka")] = true;
                            keyMapHeld[config.GetInt("keybindChaka")] = true;
                            break;
                        }

                        case 4:
                        {
                            keyMap[sf::Keyboard::Q] = true;
                            keyMapHeld[sf::Keyboard::Q] = true;
                            break;
                        }

                        case 5:
                        {
                            keyMap[sf::Keyboard::E] = true;
                            keyMapHeld[sf::Keyboard::E] = true;
                            break;
                        }
                    }
                }
            }

            if (event.type == sf::Event::JoystickButtonReleased)
            {
                if(event.joystickButton.joystickId == 0)
                {
                    std::cout << "[DEBUG] Joystick (" << event.joystickButton.joystickId << ") key released: " << event.joystickButton.button << std::endl;

                    /// TEMPORARY SOLUTION! Make joysticks mappable in future, just like keyboard. //
                    switch(event.joystickButton.button)
                    {
                        case 0:
                        {
                            keyMapHeld[config.GetInt("keybindPata")] = false;
                            break;
                        }

                        case 1:
                        {
                            keyMapHeld[config.GetInt("keybindDon")] = false;
                            break;
                        }

                        case 2:
                        {
                            keyMapHeld[config.GetInt("keybindPon")] = false;
                            break;
                        }

                        case 3:
                        {
                            keyMapHeld[config.GetInt("keybindChaka")] = false;
                            break;
                        }

                        case 4:
                        {
                            keyMapHeld[sf::Keyboard::Q] = false;
                            break;
                        }

                        case 5:
                        {
                            keyMapHeld[sf::Keyboard::E] = false;
                            break;
                        }
                    }
                }
            }

            if (event.type == sf::Event::JoystickMoved)
            {
                if(event.joystickMove.joystickId == 0)
                {
                    if (event.joystickMove.axis == sf::Joystick::PovX)
                    {
                        if(event.joystickMove.position == -100)
                        {
                            cout << "left" << endl;
                        }

                        if(event.joystickMove.position == 100)
                        {
                            cout << "right" << endl;
                        }
                    }

                    if (event.joystickMove.axis == sf::Joystick::PovY)
                    {
                        if(event.joystickMove.position == -100)
                        {
                            cout << "down" << endl;
                        }

                        if(event.joystickMove.position == 100)
                        {
                            cout << "up" << endl;
                        }
                    }
                }
            }

            if (savereader.isNewSave){
                newGameMenu.EventFired(event);
            }
            mainMenu.EventFired(event);
        }

        fps = float(1000000) / fpsclock.getElapsedTime().asMicroseconds();
        fpsclock.restart();

        //cout << fps << endl;

        window.clear();
        // Something important goes here ok cool thanks
        //if(inMission){
            //currentController.Update(window,fps);

        //} else {
        if (savereader.isNewSave){
            newGameMenu.Update(window,fps,&keyMapHeld);
        } else {
            mainMenu.Update(window,fps,&keyMapHeld);
        }
        //}

        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        t_debug.setPosition(window.getSize().x/2,window.getSize().y-20);
        window.draw(t_debug);
        window.display();

        window.setView(lastView);

        keyMap.clear();
        if(closeWindow){
            window.close();
        }
    }
}
