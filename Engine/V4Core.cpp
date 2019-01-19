#include <iostream>
#include <cstdlib>
#include <time.h>

#include "V4Core.h"

using namespace std;

V4Core::V4Core()
{
    config.LoadConfig();

    test_bg.Load(config.GetString("debugBackground"));
    rhythm.LoadTheme(config.GetString("debugTheme"));

    f_font.loadFromFile("resources/fonts/patapon.ttf");

    t_debug.setFont(f_font);
    t_debug.setCharacterSize(24);
    t_debug.setColor(sf::Color::White);
    t_debug.setString("This is an Alpha release. Game is not finished yet. Test debug build from 20th January 2019.");
    t_debug.setOrigin(t_debug.getGlobalBounds().width/2,t_debug.getGlobalBounds().height/2);
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
        if(rhythm.current_song == "patapata")
        {
            camera.walk = true;
        }
        else
        {
            camera.walk = false;
        }
        camera.Work(window);
        test_bg.setCamera(camera);
        test_bg.Draw(window);
        patapon.x = camera.followobject_x;
        patapon.fps = fps;

        if(rhythm.current_song == "patapata")
        {
            patapon.current_animation = "walk";
        }

        if((rhythm.rhythmController.current_drum == "pata") or (rhythm.rhythmController.current_drum == "pon") or (rhythm.rhythmController.current_drum == "chaka") or (rhythm.rhythmController.current_drum == "don"))
        {
            patapon.current_animation = rhythm.rhythmController.current_drum;
            patapon.current_frame = 0;
            rhythm.rhythmController.current_drum = "";
            rhythm.current_song = "";
        }

        patapon.Draw(window);
        rhythm.fps = fps;
        ///ugh this is a BAD solution i need to do it differently
        rhythm.rhythmController.keyMap = keyMap;
        rhythm.rhythmController.config = config;
        rhythm.config = config;
        rhythm.Draw(window);

        auto lastView = window.getView();
        window.setView(window.getDefaultView());

        t_debug.setPosition(window.getSize().x/2,window.getSize().y-20);
        window.draw(t_debug);

        window.setView(lastView);

        window.display();

        keyMap.clear();
    }
}
