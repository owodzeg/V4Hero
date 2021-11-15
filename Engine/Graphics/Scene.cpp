#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE 
#include "Scene.h"
#include "../V4Core.h"
#include <iostream>
Scene::Scene()
{
    //ctor
}
void Scene::Initialise(Config* thisConfigs, V4Core* parent)
{
    SPDLOG_TRACE("Initializing a new Scene...");
    thisConfig = thisConfigs;

    v4Core = parent;
    SPDLOG_TRACE("Scene initialized successfully.");
}

void Scene::EventFired(sf::Event event)
{
}

void Scene::Update(sf::RenderWindow& window, float fps)
{
}

void Scene::OnExit()
{
}

Scene::~Scene()
{
    //dtor
}
