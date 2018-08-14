#include <iostream>
#include <cstdlib>
#include <time.h>

#include "V4Core.h"

using namespace std;

V4Core::V4Core()
{
    rhythm.LoadTheme("Donjalin Theme");
    config.LoadConfig();
    test_bg.Load("nanjarohills");
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
        camera.Work(window);
        test_bg.Draw(window);
        rhythm.fps = fps;
        ///ugh this is a BAD solution i need to do it differently
        rhythm.rhythmController.keyMap = keyMap;
        rhythm.rhythmController.config = config;
        rhythm.config = config;
        rhythm.Draw(window);
        window.display();

        keyMap.clear();
    }
}
