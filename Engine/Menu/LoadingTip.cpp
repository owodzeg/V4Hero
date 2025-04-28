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

        box_1.setSize(sf::Vector2f(3840 * CoreManager::getInstance().getCore()->resRatio, 240 * CoreManager::getInstance().getCore()->resRatio));
        box_2.setSize(sf::Vector2f(3840 * CoreManager::getInstance().getCore()->resRatio, 1542 * CoreManager::getInstance().getCore()->resRatio));

        tip_logo.load("resources/graphics/ui/tips/tip-logo.png");

        loading_head.load("resources/graphics/ui/tips/loading_head.png");
        loading_eye1.load("resources/graphics/ui/tips/loading_eye.png");
        loading_eye2.load("resources/graphics/ui/tips/loading_eye.png");

        loading_eye1.setOrigin(loading_eye1.getGlobalBounds().size.x * 0.85, loading_eye1.getGlobalBounds().size.y * 0.85);
        loading_eye2.setOrigin(loading_eye2.getGlobalBounds().size.x * 0.85, loading_eye2.getGlobalBounds().size.y * 0.85);

        box_1.setPosition(sf::Vector2f(0, 60 * CoreManager::getInstance().getCore()->resRatio));
        box_2.setPosition(sf::Vector2f(0, 360 * CoreManager::getInstance().getCore()->resRatio));

        box_1.setFillColor(sf::Color(0, 0, 0, 192));
        box_2.setFillColor(sf::Color(0, 0, 0, 192));

        int tipBackground = rand() % tipsUtil->backgroundFileNames.size();
        int tipIcon = rand() % tipsUtil->iconFileNames.size();
        int tipText = (rand() % tipsUtil->tipAmount) + 1;

        std::string title_key = "tip" + std::to_string(tipText) + "_title";
        std::string desc_key = "tip" + std::to_string(tipText) + "_desc";

        std::string wtitle_key(title_key.begin(), title_key.end());
        std::string wdesc_key(desc_key.begin(), desc_key.end());

        bg_key = "resources/graphics/ui/tips/" + tipsUtil->backgroundFileNames[tipBackground];
        icon_key = "resources/graphics/ui/tips/" + tipsUtil->iconFileNames[tipIcon];

        s_bg.load(bg_key);
        s_icon.load(icon_key);
        s_icon.setOrigin(s_icon.getGlobalBounds().size.x / 2, s_icon.getGlobalBounds().size.y / 2);

        sf::String str_tipText = Func::ConvertToUtf8String(strRepo->GetString(wdesc_key));


        t_tipTitle.defaultStyleSetFont(font);
        t_tipTitle.defaultStyleSetCharSize(144);
        t_tipTitle.defaultStyleSetColor(sf::Color::White);
        t_tipTitle.append(Func::GetStrFromKey(title_key));

        t_tipText.defaultStyleSetFont(font);
        t_tipText.defaultStyleSetCharSize(96);
        t_tipText.defaultStyleSetColor(sf::Color::White);
        t_tipText.append(Func::GetStrFromKey(desc_key));

        t_pressAnyKey.defaultStyleSetFont(font);
        t_pressAnyKey.defaultStyleSetCharSize(144);
        t_pressAnyKey.defaultStyleSetColor(sf::Color::White);
        t_pressAnyKey.append(Func::GetStrFromKey("tips_anykey"));

        t_nowLoading.defaultStyleSetFont(font);
        t_nowLoading.defaultStyleSetCharSize(144);
        t_nowLoading.defaultStyleSetColor(sf::Color::White);
        t_nowLoading.append(Func::GetStrFromKey("tips_loading"));

    } else if (tipMode == 1)
    {
        box_1.setSize(sf::Vector2f(3840 * CoreManager::getInstance().getCore()->resRatio, 240 * CoreManager::getInstance().getCore()->resRatio));
        box_2.setSize(sf::Vector2f(2160 * CoreManager::getInstance().getCore()->resRatio, 1542 * CoreManager::getInstance().getCore()->resRatio));

        loading_head.load("resources/graphics/ui/tips/loading_head.png");
        loading_eye1.load("resources/graphics/ui/tips/loading_eye.png");
        loading_eye2.load("resources/graphics/ui/tips/loading_eye.png");

        loading_eye1.setOrigin(loading_eye1.getGlobalBounds().size.x * 0.85, loading_eye1.getGlobalBounds().size.y * 0.85);
        loading_eye2.setOrigin(loading_eye1.getGlobalBounds().size.x * 0.85, loading_eye1.getGlobalBounds().size.y * 0.85);

        box_1.setPosition(sf::Vector2f(0, 60 * CoreManager::getInstance().getCore()->resRatio));
        box_2.setPosition(sf::Vector2f(0, 360 * CoreManager::getInstance().getCore()->resRatio));

        box_1.setFillColor(sf::Color(0, 0, 0, 192));
        box_2.setFillColor(sf::Color(0, 0, 0, 192));

        t_nowLoading.defaultStyleSetFont(font);
        t_nowLoading.defaultStyleSetCharSize(144);
        t_nowLoading.defaultStyleSetColor(sf::Color::White);
        t_nowLoading.append(Func::GetStrFromKey("tips_loading"));
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

        t_tipTitle.setGlobalPosition(72, 96);
        t_tipTitle.draw();

        t_tipText.setGlobalPosition(72, 390);
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


            //t_pressAnyKey.setColor(sf::Color(255, 255, 255, alpha));




            t_pressAnyKey.setGlobalOrigin(t_pressAnyKey.getGlobalBounds().size.x, t_pressAnyKey.getGlobalBounds().size.y / 2);
            t_pressAnyKey.setGlobalPosition(3744, 2037);
            t_pressAnyKey.draw();

            if (inputCtrl->isAnyKeyPressed())
            {
                tipFinished = true;
                inputCtrl->Flush();
            }
        } else
        {
            t_nowLoading.setGlobalOrigin(t_nowLoading.getGlobalBounds().size.x, t_nowLoading.getGlobalBounds().size.y / 2);
            t_nowLoading.setGlobalPosition(3624, 2052);
            t_nowLoading.draw();

            loading_head.setPosition(t_nowLoading.getGlobalPosition().x - t_nowLoading.getGlobalBounds().size.x - 138, t_nowLoading.getGlobalPosition().y - 84);
            loading_eye1.setPosition(t_nowLoading.getGlobalPosition().x - t_nowLoading.getGlobalBounds().size.x - 81, t_nowLoading.getGlobalPosition().y + 45);
            loading_eye1.setRotation(angle_1);
            loading_head.draw();
            loading_eye1.draw();

            loading_head.setPosition(t_nowLoading.getGlobalPosition().x + 36, t_nowLoading.getGlobalPosition().y - 74);
            loading_eye2.setPosition(t_nowLoading.getGlobalPosition().x + 93, t_nowLoading.getGlobalPosition().y + 45);
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