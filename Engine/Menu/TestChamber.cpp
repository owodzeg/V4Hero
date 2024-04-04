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

    pngTest.Load("resources/units/unit/yaripon");

    initialized = true;
    SPDLOG_DEBUG("Welcome to the Test Chamber");
}

void TestChamber::Update()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    bg.setSize(sf::Vector2f(window->getSize().x,window->getSize().y));
    window->draw(bg);
}

TestChamber::~TestChamber()
{

}