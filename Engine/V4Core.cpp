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

    struct tm *st = localtime(&t);

    int day = st->tm_mday;
    int month = st->tm_mon;
    int year = st->tm_year + 1900;

    vector<string> months = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    string strDay = to_string(day);

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

    string dbgString = "This is an Alpha release. Game is not finished yet. Test debug build from "+strDay+" "+months[month]+" "+to_string(year)+".";

    /** Load config from config.cfg **/
    config.LoadConfig();
    /** "Alpha release" text **/

    f_font.loadFromFile("resources/fonts/patapon.ttf");

    t_debug.setFont(f_font);
    t_debug.setCharacterSize(24);
    t_debug.setFillColor(sf::Color::White);
    t_debug.setString(dbgString);
    t_debug.setOrigin(t_debug.getGlobalBounds().width/2,t_debug.getGlobalBounds().height/2);

    /** Initialize main menu **/

    mainMenu.Initialise(&config,&keyMap,this);
    config.configDebugID = 10;
}

void V4Core::Init()
{
    srand(time(NULL));

    sf::RenderWindow window;

    if(config.GetInt("enableFullscreen"))
    window.create(sf::VideoMode(config.GetInt("resX"), config.GetInt("resY")), "Patafour", sf::Style::Fullscreen);
    else
    window.create(sf::VideoMode(config.GetInt("resX"), config.GetInt("resY")), "Patafour");

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
			    ///keyMap[event.key.code] = true/false??? would that do the trick?
			    keyMap[event.key.code] = true;

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
                keyMap[event.key.code] = false;
            }
        }
        mainMenu.EventFired(event);

        fps = float(1000000) / fpsclock.getElapsedTime().asMicroseconds();
        fpsclock.restart();

        //cout << fps << endl;

        window.clear();
        // Something important goes here ok cool thanks
        //if(inMission){
            //currentController.Update(window,fps);

        //} else {
            mainMenu.Update(window,fps);
        //}

        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        t_debug.setPosition(window.getSize().x/2,window.getSize().y-20);
        window.draw(t_debug);
        window.display();

        window.setView(lastView);

        keyMap.clear();
    }
}
