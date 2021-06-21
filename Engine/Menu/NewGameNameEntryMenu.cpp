#include "NewGameNameEntryMenu.h"
#include "../V4Core.h"
#include "ButtonList.h"
#include "ButtonLists/NameEntryButtonList.h"
#include "MenuButton.h"
#include "iostream"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
NewGameNameEntryMenu::NewGameNameEntryMenu()
{
    //ctor
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    t_promptText.setFont(f_font);
    t_promptText.setCharacterSize(35);
    t_promptText.setFillColor(sf::Color::Red);

    t_title.setFont(f_font);
    t_title.setCharacterSize(55);
    t_title.setFillColor(sf::Color::White);

    t_enteredtext.setFont(f_font);
    t_enteredtext.setCharacterSize(35);
    t_enteredtext.setFillColor(sf::Color::White);
    t_enteredtext.setString("");
    t_enteredtext.setOrigin(t_enteredtext.getGlobalBounds().width / 2, t_enteredtext.getGlobalBounds().height / 2);

    //t_pressToContinue.setFont(f_font);
    //t_pressToContinue.setCharacterSize(32);
    //t_pressToContinue.setColor(sf::Color::White);
    //t_pressToContinue.setFillColor(sf::Color::White);
    //t_pressToContinue.setString("Press any key to continue...");
    //t_pressToContinue.setOrigin(t_pressToContinue.getGlobalBounds().width/2,t_pressToContinue.getGlobalBounds().height/2);

    mm_bigBox.setSize(sf::Vector2f(100, 10));
    mm_bigBox.setFillColor(sf::Color(4, 0, 90));

    mm_titleBox.setSize(sf::Vector2f(100, 10));
    mm_titleBox.setFillColor(sf::Color::Red);
}
void NewGameNameEntryMenu::Initialise(Config* _thisConfig, V4Core* parent, Menu* parentMenu)
{
    Scene::Initialise(_thisConfig, parent);
    v4Core->menus.push_back(&optionsMenu);
    savefilecreated.Initialise(_thisConfig, parent, this);
    savefilecreated.Hide();
    buttonList.Initialise(&f_font, *thisConfig, &(v4Core->currentController), this);

    f_font.loadFromFile(_thisConfig->fontPath);

    t_title.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_what_is_your_name")));
    t_title.setOrigin(t_title.getGlobalBounds().width / 2, t_title.getGlobalBounds().height / 2);

    t_promptText.setString(Func::ConvertToUtf8String(thisConfig->strRepo.GetUnicodeString(L"newgame_name_error_prompt")));
    t_promptText.setOrigin(t_promptText.getGlobalBounds().width / 2, t_promptText.getGlobalBounds().height / 2);
}
string NewGameNameEntryMenu::GetEnteredString()
{
    sf::String currentText = t_enteredtext.getString();
    return currentText.toAnsiString();
}
void NewGameNameEntryMenu::EventFired(sf::Event event)
{
    if (savefilecreated.is_active)
    {
        savefilecreated.EventFired(event);
    } else if (is_active)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Enter)
            {
                buttonList.SelectButton(0);
            }
            if (event.key.code == sf::Keyboard::Backspace)
            {
                sf::String currentText = t_enteredtext.getString();
                if (currentText.getSize() > 0)
                {
                    currentText.erase(t_enteredtext.getString().getSize() - 1, 1);
                    t_enteredtext.setString(currentText);
                    t_enteredtext.setOrigin(t_enteredtext.getGlobalBounds().width / 2, t_enteredtext.getGlobalBounds().height / 2);
                }
            }
        }
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode != '\b' && event.text.unicode != '\n' && event.text.unicode != '\r' && event.text.unicode != '\r\n')
            {
                t_enteredtext.setString(t_enteredtext.getString() + event.text.unicode);
                t_enteredtext.setOrigin(t_enteredtext.getGlobalBounds().width / 2, t_enteredtext.getGlobalBounds().height / 2);
            }
        } else if (event.type == sf::Event::MouseButtonReleased)
        {
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
            buttonList.MouseReleasedEvent(event);
        }
    }
}
void NewGameNameEntryMenu::Update(sf::RenderWindow& window, float fps)
{
    if (savefilecreated.is_active)
    {
        savefilecreated.Update(window, fps);
    } else if (is_active)
    {

        mm_bigBox.setSize(sf::Vector2f(window.getSize().x, window.getSize().y - 200));
        //mm_smallerBox.setSize(sf::Vector2f(100,10));
        //mm_titleBox.setSize(sf::Vector2f(100,10));

        mm_bigBox.setPosition(0, 85);
        //mm_smallerBox.setPosition(100,10);
        //mm_titleBox.setPosition(100,10);


        window.draw(mm_bigBox);
        //window.draw(mm_smallerBox);
        //window.draw(mm_titleBox);

        t_title.setPosition(window.getSize().x / 2, 200);
        window.draw(t_title);
        if (showPromptText)
        {
            t_promptText.setPosition(window.getSize().x / 2, 400);
            window.draw(t_promptText);
        }

        t_enteredtext.setPosition(window.getSize().x / 2, 350);
        window.draw(t_enteredtext);

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        auto lastView = window.getView();
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, lastView);

        //t_pressToContinue.setPosition(window.getSize().x/2,300);
        //window.draw(t_pressToContinue);
        buttonList.Update(window, fps, &worldPos);
        window.setView(window.getDefaultView());

    } else
    {
        if (v4Core->currentController.isInitialized)
        {
            //v4core->currentController.Update(window, fps, keyMap,keyMapHeld);
        }
    }
}
void NewGameNameEntryMenu::UpdateButtons()
{
    /// this should update the text on all the buttons
    buttonList.UpdateButtons();
}
void NewGameNameEntryMenu::OnExit()
{
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
NewGameNameEntryMenu::~NewGameNameEntryMenu()
{
    //dtor
}
