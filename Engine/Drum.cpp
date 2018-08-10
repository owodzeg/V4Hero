#include "Drum.h"
#include <iostream>

using namespace std;

Drum::Drum()
{

}

void Drum::Load(string drum, sf::RenderWindow& window)
{
    if(drum == "pata")
    {
        t_drum.loadFromFile("resources/graphics/rhythm/drums/pata.png");

        ///Describe the pattern of the drums being placed on the screen
        pattern_X.push_back(135); ///Drum 2
        pattern_Y.push_back(295);
        pattern_Angle.push_back(-20);

        pattern_X.push_back(100); ///Drum 4
        pattern_Y.push_back(410);
        pattern_Angle.push_back(-20);


        pattern_X.push_back(80); ///Drum 3
        pattern_Y.push_back(340);
        pattern_Angle.push_back(3);

        pattern_X.push_back(95); ///Drum 1
        pattern_Y.push_back(220);
        pattern_Angle.push_back(-7);


        pattern_X.push_back(100); ///Drum 4
        pattern_Y.push_back(410);
        pattern_Angle.push_back(-15);

        pattern_X.push_back(135); ///Drum 2
        pattern_Y.push_back(295);
        pattern_Angle.push_back(-23);


        pattern_X.push_back(80); ///Drum 3
        pattern_Y.push_back(340);
        pattern_Angle.push_back(3);

        pattern_X.push_back(95); ///Drum 1
        pattern_Y.push_back(220);
        pattern_Angle.push_back(-7);
    }

    if(drum == "pon")
    {
        t_drum.loadFromFile("resources/graphics/rhythm/drums/pon.png");

        ///Describe the pattern of the drums being placed on the screen
        pattern_X.push_back(1140); ///Drum 2
        pattern_Y.push_back(290);
        pattern_Angle.push_back(0);

        pattern_X.push_back(1165); ///Drum 4
        pattern_Y.push_back(410);
        pattern_Angle.push_back(20);


        pattern_X.push_back(1110); ///Drum 3
        pattern_Y.push_back(340);
        pattern_Angle.push_back(10);

        pattern_X.push_back(1135); ///Drum 1
        pattern_Y.push_back(220);
        pattern_Angle.push_back(10);


        pattern_X.push_back(1165); ///Drum 4
        pattern_Y.push_back(410);
        pattern_Angle.push_back(20);

        pattern_X.push_back(1140); ///Drum 2
        pattern_Y.push_back(290);
        pattern_Angle.push_back(0);


        pattern_X.push_back(1110); ///Drum 3
        pattern_Y.push_back(340);
        pattern_Angle.push_back(10);

        pattern_X.push_back(1135); ///Drum 1
        pattern_Y.push_back(220);
        pattern_Angle.push_back(10);
    }

    if(drum == "don")
    {
        t_drum.loadFromFile("resources/graphics/rhythm/drums/don.png");

        ///Describe the pattern of the drums being placed on the screen
        pattern_X.push_back(690); ///Drum 3
        pattern_Y.push_back(650);
        pattern_Angle.push_back(0);

        pattern_X.push_back(515); ///Drum 1
        pattern_Y.push_back(660);
        pattern_Angle.push_back(10);


        pattern_X.push_back(605); ///Drum 2
        pattern_Y.push_back(665);
        pattern_Angle.push_back(-10);

        pattern_X.push_back(780); ///Drum 4
        pattern_Y.push_back(670);
        pattern_Angle.push_back(-10);


        pattern_X.push_back(515); ///Drum 1
        pattern_Y.push_back(660);
        pattern_Angle.push_back(10);

        pattern_X.push_back(690); ///Drum 3
        pattern_Y.push_back(650);
        pattern_Angle.push_back(0);


        pattern_X.push_back(605); ///Drum 2
        pattern_Y.push_back(665);
        pattern_Angle.push_back(-10);

        pattern_X.push_back(780); ///Drum 4
        pattern_Y.push_back(670);
        pattern_Angle.push_back(-10);
    }

    if(drum == "chaka")
    {
        t_drum.loadFromFile("resources/graphics/rhythm/drums/chaka.png");

        ///Describe the pattern of the drums being placed on the screen
        pattern_X.push_back(635); ///Drum 3
        pattern_Y.push_back(105);
        pattern_Angle.push_back(0);

        pattern_X.push_back(460); ///Drum 1
        pattern_Y.push_back(70);
        pattern_Angle.push_back(10);


        pattern_X.push_back(550); ///Drum 2
        pattern_Y.push_back(75);
        pattern_Angle.push_back(-10);

        pattern_X.push_back(715); ///Drum 4
        pattern_Y.push_back(85);
        pattern_Angle.push_back(-10);


        pattern_X.push_back(460); ///Drum 1
        pattern_Y.push_back(70);
        pattern_Angle.push_back(10);

        pattern_X.push_back(635); ///Drum 3
        pattern_Y.push_back(105);
        pattern_Angle.push_back(0);


        pattern_X.push_back(550); ///Drum 2
        pattern_Y.push_back(75);
        pattern_Angle.push_back(-10);

        pattern_X.push_back(715); ///Drum 4
        pattern_Y.push_back(85);
        pattern_Angle.push_back(-10);
    }

    t_drum.setSmooth(true);

    drumClock.restart();
}

void Drum::Draw(sf::RenderWindow& window)
{
    float ratio_X = window.getSize().x / float(1280);
    float ratio_Y = window.getSize().y / float(720);

    if(drumClock.getElapsedTime().asSeconds() > 0.5)
    {
        alpha -= float(510) / fps;

        if(alpha <= 0)
        alpha = 0;
    }

    s_drum.setTexture(t_drum);
    s_drum.setScale(ratio_X,ratio_Y);
    s_drum.setColor(sf::Color(255,255,255,alpha));
    s_drum.setRotation(pattern_Angle[pattern]);

    s_drum.setOrigin(s_drum.getLocalBounds().width/2,s_drum.getLocalBounds().height/2);

    s_drum.setPosition(pattern_X[pattern]*ratio_X,pattern_Y[pattern]*ratio_Y);

    window.draw(s_drum);
}
