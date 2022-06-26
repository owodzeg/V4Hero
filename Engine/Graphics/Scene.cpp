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
    thisConfig = thisConfigs;
    v4Core = parent;
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
