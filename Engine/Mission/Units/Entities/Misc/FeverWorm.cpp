#include "FeverWorm.h"
#include "../../../../Func.h"
#include "math.h"
#include <fstream>
#include <iostream>
#include <sstream>

FeverWorm::FeverWorm()
{
    tex_number[2].loadFromFile("resources/graphics/rhythm/worm/2.png");
    tex_number[3].loadFromFile("resources/graphics/rhythm/worm/3.png");
    tex_number[4].loadFromFile("resources/graphics/rhythm/worm/4.png");
    tex_number[5].loadFromFile("resources/graphics/rhythm/worm/5.png");
    tex_number[6].loadFromFile("resources/graphics/rhythm/worm/6.png");
    tex_number[7].loadFromFile("resources/graphics/rhythm/worm/7.png");
    tex_number[8].loadFromFile("resources/graphics/rhythm/worm/8.png");
    tex_number[9].loadFromFile("resources/graphics/rhythm/worm/9.png");

    tex_c.loadFromFile("resources/graphics/rhythm/worm/c.png");
    tex_o1.loadFromFile("resources/graphics/rhythm/worm/o1.png");
    tex_m.loadFromFile("resources/graphics/rhythm/worm/m.png");
    tex_b.loadFromFile("resources/graphics/rhythm/worm/b.png");
    tex_o2.loadFromFile("resources/graphics/rhythm/worm/o2.png");
    tex_exc.loadFromFile("resources/graphics/rhythm/worm/exc.png");

    let_c.setTexture(tex_c);
    let_o1.setTexture(tex_o1);
    let_m.setTexture(tex_m);
    let_b.setTexture(tex_b);
    let_o2.setTexture(tex_o2);
    let_exc.setTexture(tex_exc);

    let_c.setOrigin(let_c.getGlobalBounds().width / 2, let_c.getGlobalBounds().height);
    let_o1.setOrigin(let_o1.getGlobalBounds().width / 2, let_o1.getGlobalBounds().height);
    let_m.setOrigin(let_b.getGlobalBounds().width / 2, let_m.getGlobalBounds().height);
    let_b.setOrigin(let_m.getGlobalBounds().width / 2, let_b.getGlobalBounds().height);
    let_o2.setOrigin(let_o2.getGlobalBounds().width / 2, let_o2.getGlobalBounds().height);
    let_exc.setOrigin(let_exc.getGlobalBounds().width / 2, let_exc.getGlobalBounds().height);
}

void FeverWorm::LoadConfig()
{
    AnimatedObject::LoadConfig("resources/units/entity/fever_worm.p4a");
}

void FeverWorm::doRhythm(std::string current_song, std::string current_drum, int combo, int realcombo, bool advanced_prefever, float beatBounce, float satisfaction)
{
    if (combo >= 11)
    {
        if (getAnimationSegment() == "fever")
        {
            scaleX = 1 + beatBounce;
            scaleY = 1 + beatBounce;
        }
    }

    if (combo < 2)
    {
        global_x = -310;
        next_x = -310;
        speed = 120;
    }

    f_combo = realcombo;

    if (f_combo != old_combo)
    {
        old_combo = f_combo;

        if (realcombo < 2)
        {
            global_x = -310;
            next_x = -310;
            speed = 120;
        }

        if (realcombo == 2)
        {
            next_x = 20;
            speed = 400;
        }

        if ((realcombo > 2) && (combo < 11))
        {
            if (advanced_prefever)
                next_x = 40 + (satisfaction / 5.5) + ((realcombo - 2) * 8);
            else
                next_x = 20 + ((realcombo - 2) * 8);

            speed = 40;
        }

        if (combo < 11)
        {
            fever_achieved = false;

            if (advanced_prefever)
            {
                setAnimationSegment("fast");
            } else
            {
                setAnimationSegment("slow");
            }
        }

        if (combo == 11)
        {
            if (!fever_achieved)
            {
                worm_fever = true;
                fever_achieved = true;
            }

            setAnimationSegment("transform");
            setLoop(false);
        }

        if (combo >= 12)
        {
            setAnimationSegment("fever");
            setLoop(true);
        }
    }
}

void FeverWorm::Draw(sf::RenderWindow& window)
{
    if (AnimatedObject::getAnimationSegment() != "transform")
    {
        if (worm_fever)
        {
            global_x = -260;
            next_x = 120;
            speed = 450;

            worm_fever = false;
        }

        if (global_x > next_x)
            global_x -= speed / fps;

        if (global_x < next_x)
            global_x += speed / fps;
    }

    auto view = window.getView();
    window.setView(window.getDefaultView());

    /// call the parent function to draw the animations
    AnimatedObject::Draw(window);

    if ((AnimatedObject::getAnimationSegment() != "fever") && (AnimatedObject::getAnimationSegment() != "transform") && ((f_combo >= 2) && (f_combo <= 9)))
    {
        number.setTexture(tex_number[f_combo]);
        number.setOrigin(number.getGlobalBounds().width / 2, number.getGlobalBounds().height / 2);
        number.setPosition(80, AnimatedObject::getGlobalPosition().y - 20);

        let_c.setPosition(number.getPosition().x + 50, number.getPosition().y + 30);
        let_o2.setPosition(let_c.getPosition().x + 26, let_c.getPosition().y);
        let_m.setPosition(let_o2.getPosition().x + 26, let_o2.getPosition().y);
        let_b.setPosition(let_m.getPosition().x + 26, let_m.getPosition().y);
        let_o1.setPosition(let_b.getPosition().x + 26, let_b.getPosition().y);
        let_exc.setPosition(let_o1.getPosition().x + 20, let_o1.getPosition().y);

        number.draw(window);

        let_c.draw(window);
        let_o1.draw(window);
        let_m.draw(window);
        let_b.draw(window);
        let_o2.draw(window);
        let_exc.draw(window);
    }

    if (AnimatedObject::getAnimationSegment() == "transform")
    {
        if (AnimatedObject::getAnimationPos() >= 0.30)
        {
            global_x -= 1000.0 / fps;
        }
    }

    window.setView(view);
}
