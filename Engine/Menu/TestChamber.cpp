#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "TestChamber.h"
#include "CoreManager.h"
#include <spdlog/spdlog.h>

TestChamber::TestChamber()
{
    Initialize();
}

void TestChamber::Initialize()
{
    bg.setFillColor(sf::Color::White);

    sf::Clock loadSpeed;
    int64_t cur=0, old=0;
    int64_t bench1,bench2,bench3;
    cur = loadSpeed.getElapsedTime().asMicroseconds();
    SPDLOG_INFO("Load timer: {}us", cur-old);
    old = cur;
    yaripon.LoadConfig("resources/units/unit/yaripon.zip");
    cur = loadSpeed.getElapsedTime().asMicroseconds();
    bench1 = cur-old;
    old = cur;
    hatapon.LoadConfig("resources/units/unit/hatapon.zip");
    cur = loadSpeed.getElapsedTime().asMicroseconds();
    bench2 = cur-old;
    old = cur;
    dropped_item.LoadConfig("resources/units/unit/dropped_item.zip");
    cur = loadSpeed.getElapsedTime().asMicroseconds();
    bench3 = cur-old;
    SPDLOG_INFO("Loading benchmark:");
    SPDLOG_INFO("Yaripon 1: {}s ({}us)", bench1 / 1000000.f, bench1);
    SPDLOG_INFO("Yaripon 2: {}s ({}us)", bench2 / 1000000.f, bench2);
    SPDLOG_INFO("Yaripon 3: {}s ({}us)", bench3 / 1000000.f, bench3);

    SPDLOG_DEBUG("Welcome to the Test Chamber");

    ptext.append("{speed 0}{color 255 0 0}{size 90}Hello, {size 150}VAVAVAVAVAVA! lllllllllllll{n}{bold}V4Hero Client v1.2 is real!{n}{size 30}This {size 45}is {size 120}{color 0 255 128}a test {color 0 0 0}{size 60}of the {italic}PataText{italic} system.{bold}{n}{size 180}{color 255 0 0}R{color 255 128 0}A{color 255 255 0}I{color 128 255 0}N{color 0 255 0}B{color 0 255 128}O{color 0 255 255}W{color 0 0 0}{size 90}{n}{n}Let's test kerning!{n}VAVAVAVAVAVAVA{n}{font fallback}VAVAVAVAVAVAVA{n}{font kaku-uni}The quick brown fox jumps over a lazy dog.{n}{font kaku-cn}The quick brown fox jumps over a lazy dog.{n}{font fallback}The quick brown fox jumps over a lazy dog.{font kaku-uni}{n}{size 90}{speed 10}{wait 3000}Yo... I heard that these... {wait 500}{speed 5}{shake 6}{color 255 0 0}{bold}KIRAJINS{speed 10}{bold}{color 0 0 0}{shake 0} are some real trouble...{n}{wait 1000}{hwave 12 10}I'm dizzy.{wait 1000}{hwave 0 0}{n}{n}{wait 1000}{vwave 18 10}heyyyyyy sweetie <3{vwave 0 0}{n}{n}{wait 1000}{shake 15}{hwave 18 10}{vwave 21 10}oh{color 100 0 0}hh hh{color 0 0 0}h {font kaku-cn}my{size 150}y{color 0 200 0}yy{font kaku-uni} gg{color 0 0 0}o{size 60}oo{font fallback}oo{color 255 0 255}o{size 60}ddd{color 0 0 0}{size 90}dd{font kaku-uni}ddd {color 255 0 0}{shake 30}{speed 7}AAAAA{shake 45}{speed 5}{size 150}AAAAA{shake 60}{size 210}{speed 3}AAAAAA");

    initialized = true;
}

void TestChamber::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    bg.setSize(sf::Vector2f(static_cast<float>(window->getSize().x),static_cast<float>(window->getSize().y)));
    window->draw(bg);

    yaripon.pos_global = sf::Vector2f(400, 200);
    //yaripon.Draw();

    hatapon.pos_global = sf::Vector2f(100, 200);
    //hatapon.Draw();

    dropped_item.pos_global = sf::Vector2f(100, 500);
    //dropped_item.Draw();

    ptext.draw();
}

TestChamber::~TestChamber()
{

}