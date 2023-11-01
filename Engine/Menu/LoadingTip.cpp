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

    if (tipMode == 0)
    {
        TipsUtil* tipsUtil = CoreManager::getInstance().getTipsUtil();
        sf::RenderWindow* window = CoreManager::getInstance().getWindow();
        StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
        Config* config = CoreManager::getInstance().getConfig();

        float resRatioX = window->getSize().x / float(1280);
        float resRatioY = window->getSize().y / float(720);

        box_1.setSize(sf::Vector2f(1280 * resRatioX, 80 * resRatioY));
        box_2.setSize(sf::Vector2f(1280 * resRatioX, 514 * resRatioY));

        tip_logo.load("resources/graphics/ui/tips/tip-logo.png");

        loading_head.load("resources/graphics/ui/tips/loading_head.png");
        loading_eye1.load("resources/graphics/ui/tips/loading_eye.png");
        loading_eye2.load("resources/graphics/ui/tips/loading_eye.png");

        loading_eye1.setOrigin(loading_eye1.getGlobalBounds().width * 0.85, loading_eye1.getGlobalBounds().height * 0.85);
        loading_eye2.setOrigin(loading_eye2.getGlobalBounds().width * 0.85, loading_eye2.getGlobalBounds().height * 0.85);

        box_1.setPosition(0, 20 * resRatioY);
        box_2.setPosition(0, 120 * resRatioY);

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

        f_font.loadFromFile(config->fontPath);

        sf::String str_tipText = Func::ConvertToUtf8String(strRepo->GetString(wdesc_key));

        t_tipTitle.createText(f_font, 48, sf::Color(255, 255, 255, 255), Func::ConvertToUtf8String(strRepo->GetString(wtitle_key)), config->GetInt("textureQuality"), 1);
        t_tipText.createText(f_font, 32, sf::Color(255, 255, 255, 255), str_tipText, config->GetInt("textureQuality"), 1);
        t_pressAnyKey.createText(f_font, 46, sf::Color(255, 255, 255, 255), Func::ConvertToUtf8String(strRepo->GetString("tips_anykey")), config->GetInt("textureQuality"), 1);
        t_nowLoading.createText(f_font, 46, sf::Color(255, 255, 255, 255), Func::ConvertToUtf8String(strRepo->GetString("tips_loading")), config->GetInt("textureQuality"), 1);
    } else if (tipMode == 1)
    {
        sf::RenderWindow* window = CoreManager::getInstance().getWindow();
        StringRepository* strRepo = CoreManager::getInstance().getStrRepo();
        Config* config = CoreManager::getInstance().getConfig();

        float resRatioX = window->getSize().x / float(1280);
        float resRatioY = window->getSize().y / float(720);

        box_1.setSize(sf::Vector2f(1280 * resRatioX, 80 * resRatioY));
        box_2.setSize(sf::Vector2f(1280 * resRatioX, 514 * resRatioY));

        loading_head.load("resources/graphics/ui/tips/loading_head.png");
        loading_eye1.load("resources/graphics/ui/tips/loading_eye.png");
        loading_eye2.load("resources/graphics/ui/tips/loading_eye.png");

        loading_eye1.setOrigin(loading_eye1.getGlobalBounds().width * 0.85, loading_eye1.getGlobalBounds().height * 0.85);
        loading_eye2.setOrigin(loading_eye1.getGlobalBounds().width * 0.85, loading_eye1.getGlobalBounds().height * 0.85);

        box_1.setPosition(0, 20 * resRatioY);
        box_2.setPosition(0, 120 * resRatioY);

        box_1.setFillColor(sf::Color(0, 0, 0, 192));
        box_2.setFillColor(sf::Color(0, 0, 0, 192));

        f_font.loadFromFile(config->fontPath);

        t_nowLoading.createText(f_font, 46, sf::Color(255, 255, 255, 255), Func::ConvertToUtf8String(strRepo->GetString("tips_loading")), config->GetInt("textureQuality"), 1);
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

        tip_logo.setPosition(1060, 20);
        tip_logo.draw();

        s_icon.setPosition(1040, 380);
        s_icon.draw();

        t_tipTitle.setPosition(24, 32);
        t_tipTitle.draw(window);

        t_tipText.setPosition(24, 130);
        t_tipText.draw(window);

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
            t_pressAnyKey.setPosition(722 + 526, 658 + 21);
            t_pressAnyKey.draw(window);

            if (inputCtrl->isAnyKeyPressed())
            {
                tipFinished = true;
            }
        } else
        {
            t_nowLoading.setOrigin(t_nowLoading.getLocalBounds().width, t_nowLoading.getLocalBounds().height / 2);
            t_nowLoading.setPosition(722 + 230 + 256, 658 + 26);
            t_nowLoading.draw(window);

            loading_head.setPosition(t_nowLoading.getPosition().x - t_nowLoading.getLocalBounds().width - 46, t_nowLoading.getPosition().y - 28);
            loading_eye1.setPosition(t_nowLoading.getPosition().x - t_nowLoading.getLocalBounds().width + 19 - 46, t_nowLoading.getPosition().y + 43 - 28);
            loading_eye1.setRotation(angle_1);
            loading_head.draw();
            loading_eye1.draw();

            loading_head.setPosition(t_nowLoading.getPosition().x + 12, t_nowLoading.getPosition().y - 28);
            loading_eye2.setPosition(t_nowLoading.getPosition().x + 19 + 12, t_nowLoading.getPosition().y + 43 - 28);
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
            loading_head.setPosition(1220, 650);
            loading_eye2.setPosition(1239, 693);
            loading_eye2.setRotation(angle_2);
            loading_head.draw();
            loading_eye2.draw();

            angle_2 -= 2.f / fps;
        }
    }

    window->setView(lastView);
}