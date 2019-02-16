#include <iostream>
#include <cstdlib>
#include <time.h>

#include "V4Core.h"

using namespace std;

V4Core::V4Core()
{
    config.LoadConfig();


    f_font.loadFromFile("resources/fonts/patapon.ttf");

    t_debug.setFont(f_font);
    t_debug.setCharacterSize(24);
    t_debug.setFillColor(sf::Color::White);
    t_debug.setString("This is an Alpha release. Game is not finished yet. Test debug build from 3rd February 2019.");
    t_debug.setOrigin(t_debug.getGlobalBounds().width/2,t_debug.getGlobalBounds().height/2);
    //inMission=false;
    mainMenu.Initialise(config,&keyMap,this);
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
			    mainMenu.KeyPressedEvent(event);
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

        fps = float(1000000) / fpsclock.getElapsedTime().asMicroseconds();
        fpsclock.restart();

        //cout << fps << endl;

        window.clear();
        // Something important goes here ok cool thanks
        //if(inMission){
            //currentController.Update(window,fps);

        //} else {
            mainMenu.Update(window,fps);
            auto lastView = window.getView();
            window.setView(window.getDefaultView());

            window.setView(lastView);
        //}
        sf::Vector2i pixelPos = sf::Vector2i(window.getSize().x/2,window.getSize().y-20);//sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos,lastView);
        t_debug.setPosition(worldPos);
        window.draw(t_debug);
        window.display();

        keyMap.clear();
    }
}
