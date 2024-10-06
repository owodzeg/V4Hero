#include "LoadingTip.h"
#include "../ResourceManager.h"
#include "../CoreManager.h"

LoadingTip::LoadingTip(int mode)
{
    tipMode = mode;
    startAlpha = 128;
    endAlpha = 255;
    timer.restart();
    targetTime = timer.getElapsedTime() + sf::seconds(1.0f);

    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
    std::string font = strRepo->GetFontNameForLanguage(strRepo->GetCurrentLanguage());

    if (tipMode == 0)
    {
        TipsUtil* tipsUtil = CoreManager::getInstance().getTipsUtil();

        float resRatioX = window->getSize().x / float(3840);
        float resRatioY = window->getSize().y / float(2160);

        box_1.setSize(sf::Vector2f(3840 * resRatioX, 240 * resRatioY));
        box_2.setSize(sf::Vector2f(3840 * resRatioX, 1542 * resRatioY));

        tip_logo.load("resources/graphics/ui/tips/tip-logo.png");

        loading_head.load("resources/graphics/ui/tips/loading_head.png");
        loading_eye1.load("resources/graphics/ui/tips/loading_eye.png");
        loading_eye2.load("resources/graphics/ui/tips/loading_eye.png");

        loading_eye1.setOrigin(loading_eye1.getGlobalBounds().width * 0.85, loading_eye1.getGlobalBounds().height * 0.85);
        loading_eye2.setOrigin(loading_eye2.getGlobalBounds().width * 0.85, loading_eye2.getGlobalBounds().height * 0.85);

        box_1.setPosition(0, 60 * resRatioY);
        box_2.setPosition(0, 360 * resRatioY);

        box_1.setFillColor(sf::Color(0, 0, 0, 192));
        box_2.setFillColor(sf::Color(0, 0, 0, 192));

        int tipBackground = rand() % tipsUtil->backgroundFileNames.size();
        int tipIcon = rand() % tipsUtil->iconFileNames.size();
        int tipText = (rand() % tipsUtil->tipAmount) + 1;

        string title_key = "tip" + to_string(tipText) + "_title";
        string desc_key = "tip" + to_string(tipText) + "_desc";

        string wtitle_key(title_key.begin(), title_key.end());
        string wdesc_key(desc_key.begin(), desc_key.end());

        bg_key = "resources/graphics/ui/tips/" + tipsUtil->backgroundFileNames[tipBackground];
        icon_key = "resources/graphics/ui/tips/" + tipsUtil->iconFileNames[tipIcon];

        s_bg.load(bg_key);
        s_icon.load(icon_key);
        s_icon.setOrigin(s_icon.getGlobalBounds().width / 2, s_icon.getGlobalBounds().height / 2);

        sf::String str_tipText = Func::ConvertToUtf8String(strRepo->GetString(wdesc_key));


        t_tipTitle.setFont(font);
        t_tipTitle.setCharacterSize(48);
        t_tipTitle.setColor(sf::Color::White);
        t_tipTitle.setStringKey(title_key);

        t_tipText.setFont(font);
        t_tipText.setCharacterSize(32);
        t_tipText.setColor(sf::Color::White);
        t_tipText.setStringKey(desc_key);

        t_pressAnyKey.setFont(font);
        t_pressAnyKey.setCharacterSize(48);
        t_pressAnyKey.setColor(sf::Color::White);
        t_pressAnyKey.setStringKey("tips_anykey");

        t_nowLoading.setFont(font);
        t_nowLoading.setCharacterSize(48);
        t_nowLoading.setColor(sf::Color::White);
        t_nowLoading.setStringKey("tips_loading");
    } else if (tipMode == 1)
    {
        float resRatioX = window->getSize().x / float(3840);
        float resRatioY = window->getSize().y / float(2160);

        box_1.setSize(sf::Vector2f(3840 * resRatioX, 240 * resRatioY));
        box_2.setSize(sf::Vector2f(2160 * resRatioX, 1542 * resRatioY));

        loading_head.load("resources/graphics/ui/tips/loading_head.png");
        loading_eye1.load("resources/graphics/ui/tips/loading_eye.png");
        loading_eye2.load("resources/graphics/ui/tips/loading_eye.png");

        loading_eye1.setOrigin(loading_eye1.getGlobalBounds().width * 0.85, loading_eye1.getGlobalBounds().height * 0.85);
        loading_eye2.setOrigin(loading_eye1.getGlobalBounds().width * 0.85, loading_eye1.getGlobalBounds().height * 0.85);

        box_1.setPosition(0, 60 * resRatioY);
        box_2.setPosition(0, 360 * resRatioY);

        box_1.setFillColor(sf::Color(0, 0, 0, 192));
        box_2.setFillColor(sf::Color(0, 0, 0, 192));

        t_nowLoading.setFont(font);
        t_nowLoading.setCharacterSize(48);
        t_nowLoading.setColor(sf::Color::White);
        t_nowLoading.setStringKey("tips_loading");
    }
}

void LoadingTip::Draw()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    auto lastView = window->getView();
    window->setView(window->getDefaultView());

    if (tipMode == 0)
    {
        s_bg.draw();

        window->draw(box_1);
        window->draw(box_2);

        tip_logo.setPosition(3180, 60);
        tip_logo.draw();

        s_icon.setPosition(3120, 1140);
        s_icon.draw();

        t_tipTitle.setPosition(72, 96);
        t_tipTitle.draw();

        t_tipText.setPosition(72, 390);
        t_tipText.draw();

        // drawing some text
        if (pressAnyKey)
        {
            float timeRemaining = targetTime.asSeconds() - timer.getElapsedTime().asSeconds();
            if (timeRemaining <= 0)
            {
                // Reset target time for the next cycle
                targetTime = sf::seconds(1.0f); // Reset to a fixed duration
                timer.restart(); // Restart the timer for the new cycle
                fadein = !fadein; // Toggle the fade direction
                timeRemaining = targetTime.asSeconds() - timer.getElapsedTime().asSeconds();
            }
            float progress = timeRemaining / targetTime.asSeconds();
            float alpha = startAlpha;
            if (fadein) {
                // Quadratic ease-out
                progress = 1 - progress;
                alpha = startAlpha + (endAlpha - startAlpha) * progress * progress;
            } else {
                // Quadratic ease-in
                alpha = startAlpha + (endAlpha - startAlpha) * (progress * progress);
            }

            alpha = std::max(0, std::min(255, static_cast<int>(alpha)));


            t_pressAnyKey.setColor(sf::Color(255, 255, 255, alpha));




            t_pressAnyKey.setOrigin(t_pressAnyKey.getLocalBounds().width, t_pressAnyKey.getLocalBounds().height / 2);
            t_pressAnyKey.setPosition(3744, 2037);
            t_pressAnyKey.draw();

            if (inputCtrl->isAnyKeyPressed())
            {
                tipFinished = true;
                inputCtrl->Flush();
            }
        } else
        {
            t_nowLoading.setOrigin(t_nowLoading.getLocalBounds().width, t_nowLoading.getLocalBounds().height / 2);
            t_nowLoading.setPosition(3624, 2052);
            t_nowLoading.draw();

            loading_head.setPosition(t_nowLoading.getPosition().x - t_nowLoading.getLocalBounds().width - 138, t_nowLoading.getPosition().y - 84);
            loading_eye1.setPosition(t_nowLoading.getPosition().x - t_nowLoading.getLocalBounds().width - 81, t_nowLoading.getPosition().y + 45);
            loading_eye1.setRotation(angle_1);
            loading_head.draw();
            loading_eye1.draw();

            loading_head.setPosition(t_nowLoading.getPosition().x + 36, t_nowLoading.getPosition().y - 74);
            loading_eye2.setPosition(t_nowLoading.getPosition().x + 93, t_nowLoading.getPosition().y + 45);
            loading_eye2.setRotation(angle_2);
            loading_head.draw();
            loading_eye2.draw();

            angle_1 += 2.f / fps;
            angle_2 -= 2.f / fps;
        }
    } else if (tipMode == 1)
    {
        // drawing some text
        if (pressAnyKey)
        {
            tipFinished = true;
        } else
        {
            loading_head.setPosition(3660, 1950);
            loading_eye2.setPosition(3717, 2079);
            loading_eye2.setRotation(angle_2);
            loading_head.draw();
            loading_eye2.draw();

            angle_2 -= 2.f / fps;
        }
    }

    window->setView(lastView);
}