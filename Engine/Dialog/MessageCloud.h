#ifndef MESSAGECLOUD_H
#define MESSAGECLOUD_H

#include <SFML/Graphics.hpp>

#include "../Graphics/PSprite.h"
#include "../Graphics/PText.h"
#include "../Input/InputController.h"

class MessageCloud
{
public:
    PSprite cloud[4];
    PSprite cross, cross_highlight, cross_arrow;
    sf::ConvexShape triangle;

    std::vector<PText> ptext;    ///text used for determining display sizes
    std::vector<PText> showtext; ///text that is being shown on screen

    sf::Font font;

    std::vector<sf::String> loaded_text; ///text used to compare with what's displayed
    std::vector<sf::String> viewed_text; ///text that is being shown on screen

    int cur_char = 0;
    int timeout = 50; ///milliseconds
    int regular_timeout = timeout;
    int speed_timeout = 5;

    bool speedup = false;

    sf::Clock text_timeout;
    sf::Clock arrow_timeout;

    float x = 0, y = 0;

    bool active = false;
    std::vector<bool> next_dialog;

    int cur_dialog = 0;

    float old_xsize = 0, old_ysize = 0;
    float xsize = 0, ysize = 0;
    float dest_xsize = 0, dest_ysize = 0;
    float arrow_y = 0;

    int quality = 0;

    bool ready = false;
    bool done = false;
    bool canwrite = false;
    bool firstrender = false;
    bool speedable = false;

    sf::Vector2f startpos;

    sf::Color cur_color;

    int msgcloud_ID = 0;

    MessageCloud();
    void setSize(float new_x, float new_y);
    void Create(int speed, sf::Vector2f start_pos, sf::Color color, bool can_speedup, int q, std::string font_path);
    void AddDialog(sf::String text, bool nextdialog);
    void NextDialog();
    void SpeedUp();
    void Show();
    void Hide();
    void End();
    void Draw(sf::RenderWindow& window, float fps, InputController& inputCtrl);
};

#endif // MESSAGECLOUD_H
