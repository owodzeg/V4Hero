#include "../CoreManager.h"
#include "Credits.h"
#include <math.h>

Credits::Credits()
{
}

Credits::~Credits()
{
    delete s_outro;
    delete s_credits;
}

void Credits::addHeaderText(sf::String text)
{
    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    PataText p;
    //TO-DO: PataText rework
    /*
    p.defaultStyleSetFont(font);
    p.defaultStyleSetCharSize(36);
    p.setColor(sf::Color::White);
    p.append(Func::GetStrFromKey(text);
    p.setPosition(640, 800 + (36 * credits_text.size()));
    */
    credits_text.push_back(p);
}

void Credits::addRegularText(sf::String text, int font)
{
    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font_str = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    PataText p;
    /*
    if (font == 0)
    {
        p.defaultStyleSetFont(font_str);
        p.defaultStyleSetCharSize(24);
        p.setColor(sf::Color::White);
        p.setString(text);
    }
    else if (font == 1)
    {
        p.defaultStyleSetFont("kaku-cn");
        p.defaultStyleSetCharSize(24);
        p.setColor(sf::Color::White);
        p.setString(text);
    }

    p.setPosition(640, 800 + (36 * credits_text.size()));
    */
    credits_text.push_back(p);
}

void Credits::Initialise(Config* thisConfig, V4Core* parent)
{
    config = thisConfig;
    quality = thisConfig->GetInt("textureQuality");

    r_black.setSize(sf::Vector2f(thisConfig->GetInt("resX"), thisConfig->GetInt("resY")));
    r_black.setFillColor(sf::Color::Black);

    sb_outro.loadFromFile("resources/sfx/fun/outro.ogg");
    sb_credits.loadFromFile("resources/sfx/fun/patafour4.ogg");

    s_outro = new sf::Sound(sb_outro);
    s_credits = new sf::Sound(sb_credits);

    s_outro->setVolume(float(thisConfig->GetInt("masterVolume")) * (float(thisConfig->GetInt("bgmVolume")) / 100.f));
    s_credits->setVolume(float(thisConfig->GetInt("masterVolume")) * (float(thisConfig->GetInt("bgmVolume")) / 100.f));

    auto strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    /*
    outro_text.defaultStyleSetFont(font);
    outro_text.defaultStyleSetCharSize(36);
    outro_text.setColor(sf::Color::White);
    outro_text.setOutlineThickness(2);
    outro_text.setOutlineColor(sf::Color::Black);

    anykey.defaultStyleSetFont(font);
    anykey.defaultStyleSetCharSize(36);
    anykey.setColor(sf::Color::White);
    */

    teaser_1.loadFromFile("resources/graphics/ui/credits/teaser_1.png", thisConfig->GetInt("textureQuality"));
    teaser_2.loadFromFile("resources/graphics/ui/credits/teaser_2.png", thisConfig->GetInt("textureQuality"));
    teaser_3.loadFromFile("resources/graphics/ui/credits/teaser_3.png", thisConfig->GetInt("textureQuality"));

    addHeaderText("credits_creator");
    addRegularText("Owocek");
    addRegularText("");
    addHeaderText("credits_programmers");
    addRegularText("Owocek");
    addRegularText("RugnirTheViking");
    addRegularText("Golem1903");
    addRegularText("MightyTheJa");
    addRegularText("");
    addHeaderText("credits_artists");
    addRegularText("Shockturtle");
    addRegularText("Fabierex");
    addRegularText("Xandis");
    addRegularText("Supercharger");
    addRegularText("Maxi");
    addRegularText("Iracy");
    addRegularText("Patahero");
    addRegularText("Maciek1704");
    addRegularText("");
    addHeaderText("credits_animators");
    addRegularText("DriftStar");
    addRegularText("Topaz-The-CrossCat");
    addRegularText("");
    addHeaderText("credits_musicians");
    addRegularText("Zephyr5D");
    addRegularText("Kotklopot");
    addRegularText("");
    addHeaderText("credits_gameplay");
    addRegularText("Magam");
    addRegularText("");
    addHeaderText("credits_writers");
    addRegularText("Arciel");
    addRegularText("Supercharger");
    addRegularText("");
    addHeaderText("credits_community");
    addRegularText("SymphonyBlue");
    addRegularText("Arciel");
    addRegularText("Alyssa");
    addRegularText("0Modd0");
    addRegularText("");
    addHeaderText("credits_translators");
    addRegularText("Kyron Finder");
    addRegularText("Amonimus");
    addRegularText("Fables Ash");
    addRegularText("");
    addRegularText("x225franc");
    addRegularText("TooFat4You");
    addRegularText("");
    addRegularText("Iracy");
    addRegularText("");
    addRegularText("Trinix");
    addRegularText("");
    addRegularText("SackBoy (SBG)");
    addRegularText("Maglor el Mago");
    addRegularText("NGy14");
    addRegularText("");
    ///add different font parameter maybe? would be cool!
    addRegularText("纯洁的菲那", 1);
    addRegularText("7君", 1);
    addRegularText("永远的盾拉杰", 1);
    addRegularText("短信蛾", 1);
    addRegularText("LanT");
    addRegularText("冥蓝幽风", 1);
    addRegularText("Mr.F");
    addRegularText("Hammer Princess Meden");
    addRegularText("");
    addRegularText("HarryR");
    addRegularText("");
    addRegularText("Artur Płatkowski");
    addRegularText("");
    addRegularText("TBAdriel");
    addRegularText("");
    addRegularText("WDayro =D");
    addRegularText("");
    addRegularText("Gagu2000");
    addRegularText("");
    addHeaderText("credits_supporters");
    addRegularText("Rugnir");
    addRegularText("Abu_Ali");
    addRegularText("Adri-chan2");
    addRegularText("Amonimus");
    addRegularText("Arciel");
    addRegularText("Blu'");
    addRegularText("Bluewarriorz");
    addRegularText("Charlie Feik");
    addRegularText("cozybutsad");
    addRegularText("Death");
    addRegularText("electricflash");
    addRegularText("Fabierex");
    addRegularText("Forever'Tate.Razay");
    addRegularText("Ha-paya on yo ass");
    addRegularText("Harmonia");
    addRegularText("harrazel");
    addRegularText("Iracy");
    addRegularText("jason (jason)");
    addRegularText("kotklopot");
    addRegularText("Lizzyisme");
    addRegularText("matikd");
    addRegularText("Maxigattan");
    addRegularText("MegaPixlz");
    addRegularText("Mighty Dettankarmen");
    addRegularText("Nieton");
    addRegularText("onion");
    addRegularText("Orbacle");
    addRegularText("Patahero");
    addRegularText("puatek");
    addRegularText("radix_1");
    addRegularText("Rah");
    addRegularText("SadB");
    addRegularText("SeriousGranade");
    addRegularText("shockturtle");
    addRegularText("Sonicjan");
    addRegularText("tank_facts");
    addRegularText("Tommy");
    addRegularText("Topaz-The-CrossCat");
    addRegularText("tosutosu");
    addRegularText("Twoshimaetiyoshiko");
    addRegularText("Vengeance");
    addRegularText("washio hands or catch plague");
    addRegularText("Xibral");
    addRegularText("MrNuckMAn");
    addRegularText("樹皮");
    addRegularText("The Maxilimit");
    addRegularText("bigbrainben");
    addRegularText("Chersi Sutalinov");
    addRegularText("Hiroyuki Kotani");
    addRegularText("");
    addHeaderText("credits_special");
    addRegularText("Mitsuki");
    addRegularText("WondaOxigen");
    addRegularText("");
    addRegularText("");
    addRegularText("");
    addRegularText("All my great friends who always supported me <3");
    addRegularText("");
    addRegularText("");
    addRegularText("");
    addRegularText("My family for all the support!");
    addRegularText("");
    addRegularText("");
    addRegularText("");
    addRegularText("credits_all");
    addRegularText("");
    addRegularText("");
    addRegularText("");
    addRegularText("");
    addRegularText("");
    addRegularText("and to YOU, for playing the game! :)");
}

void Credits::restart()
{
    mode = 0;
    playOutro = false;
    startTimer.restart();

    for (int i = 0; i < credits_text.size(); i++)
    {
        //credits_text[i].setPosition(640, 800 + (36 * i));
    }

    //outro_text.setString("");
}

void Credits::draw(sf::RenderWindow& window, float fps, InputController& inputCtrl)
{
    if (mode == 0)
    {
        window.draw(r_black);

        if (!playOutro)
        {
            if (startTimer.getElapsedTime().asSeconds() > 1)
            {
                s_outro->stop();
                s_outro->setBuffer(sb_outro);
                s_outro->play();

                playOutro = true;
            }
        }

        /*
        if (startTimer.getElapsedTime().asSeconds() > 3.2)
        {
            outro_text.setString("outro_cutscene_1");
            outro_text.setPosition(640, 360);
            outro_text.setOrigin(outro_text.getLocalBounds().size.x / 2, outro_text.getLocalBounds().size.y / 2);
        }

        if (startTimer.getElapsedTime().asSeconds() > 7)
        {
            outro_text.setString("outro_cutscene_2");
            outro_text.setPosition(640, 360);
            outro_text.setOrigin(outro_text.getLocalBounds().size.x / 2, outro_text.getLocalBounds().size.y / 2);
        }

        if (startTimer.getElapsedTime().asSeconds() > 9)
        {
            outro_text.setString("outro_cutscene_3");
            outro_text.setPosition(640, 360);
            outro_text.setOrigin(outro_text.getLocalBounds().size.x / 2, outro_text.getLocalBounds().size.y / 2);
        }

        if (startTimer.getElapsedTime().asSeconds() > 12.8)
        {
            outro_text.setString("outro_cutscene_4");
            outro_text.setPosition(640, 680);
            outro_text.setOrigin(outro_text.getLocalBounds().size.x / 2, outro_text.getLocalBounds().size.y);

            teaser_1.draw();
        }

        if (startTimer.getElapsedTime().asSeconds() > 16.6)
        {
            outro_text.setString("outro_cutscene_5");
            outro_text.setPosition(640, 700);
            outro_text.setOrigin(outro_text.getLocalBounds().size.x / 2, outro_text.getLocalBounds().size.y);

            teaser_2.draw();
        }

        if (startTimer.getElapsedTime().asSeconds() > 20.5)
        {
            outro_text.setString("outro_cutscene_6");
            outro_text.setPosition(640, 700);
            outro_text.setOrigin(outro_text.getLocalBounds().size.x / 2, outro_text.getLocalBounds().size.y);

            teaser_3.draw();
        }

        if (startTimer.getElapsedTime().asSeconds() > 24.2)
        {
            outro_text.setString("outro_cutscene_7");
            outro_text.setPosition(640, 360);
            outro_text.setOrigin(outro_text.getLocalBounds().size.x / 2, outro_text.getLocalBounds().size.y / 2);
            window.draw(r_black);
        }

        if (startTimer.getElapsedTime().asSeconds() > 26.2)
        {
            outro_text.setString("outro_cutscene_8");
            outro_text.setPosition(640, 360);
            outro_text.setOrigin(outro_text.getLocalBounds().size.x / 2, outro_text.getLocalBounds().size.y / 2);
            window.draw(r_black);
        }

        if (startTimer.getElapsedTime().asSeconds() > 29.4)
        {
            outro_text.setString("outro_end");
            outro_text.setPosition(640, 360);
            outro_text.setOrigin(outro_text.getLocalBounds().size.x / 2, outro_text.getLocalBounds().size.y / 2);
            window.draw(r_black);
        }

        if (startTimer.getElapsedTime().asSeconds() > 35)
        {
            mode = 1;

            s_credits->stop();
            s_credits->setBuffer(sb_credits);
            s_credits->setLooping(true);
            s_credits->play();
        }

        outro_text.draw();
        */
    }

    if (mode == 1)
    {
        window.draw(r_black);

        for (int i = 0; i < credits_text.size(); i++)
        {
            //credits_text[i].ly -= 50.0 / fps;
            //credits_text[i].setOrigin(credits_text[i].getLocalBounds().size.x / 2, credits_text[i].getLocalBounds().size.y / 2);
            credits_text[i].draw();
        }

        //if (credits_text[credits_text.size() - 1].ly < -120)
        {
            flash_x += 1.0 / fps;

            //anykey.append(Func::GetStrFromKey("tips_anykey");
            //anykey.setOrigin(anykey.getLocalBounds().size.x / 2, anykey.getLocalBounds().size.y / 2);
            //anykey.setColor(sf::Color(255, 255, 255, 127 + (cos(flash_x) * 127)));
            //anykey.setPosition(640, 360);
            anykey.draw();

            if (inputCtrl.isAnyKeyPressed())
            {
                s_credits->stop();
                is_active = false;
            }
        }
    }

    if (inputCtrl.isKeyPressed(Input::Keys::START))
    {
        s_credits->stop();
        s_outro->stop();
        is_active = false;
    }
}
