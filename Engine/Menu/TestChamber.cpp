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
    sf::Int64 cur=0, old=0;
    cur = loadSpeed.getElapsedTime().asMicroseconds();
    SPDLOG_INFO("Load timer: {}us", cur-old);
    old = cur;
    pngTest_1.Load("resources/units/unit/yaripon");
    cur = loadSpeed.getElapsedTime().asMicroseconds();
    SPDLOG_INFO("Loaded yaripon 1, timer: {}us", cur-old);
    old = cur;
    pngTest_2.Load("resources/units/unit/yaripon");
    cur = loadSpeed.getElapsedTime().asMicroseconds();
    SPDLOG_INFO("Loaded yaripon 2, timer: {}us", cur-old);
    old = cur;
    pngTest_3.Load("resources/units/unit/yaripon");
    cur = loadSpeed.getElapsedTime().asMicroseconds();
    SPDLOG_INFO("Loaded yaripon 3, timer: {}us", cur-old);

    initialized = true;
    SPDLOG_DEBUG("Welcome to the Test Chamber");
}

void TestChamber::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    bg.setSize(sf::Vector2f(window->getSize().x,window->getSize().y));
    window->draw(bg);

    pngTest_1.Draw();
}

TestChamber::~TestChamber()
{

}