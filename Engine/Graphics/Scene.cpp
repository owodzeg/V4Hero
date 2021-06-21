#include "Scene.h"
#include "../V4Core.h"
#include <iostream>
Scene::Scene()
{
    //ctor
}
void Scene::Initialise(Config* thisConfigs, V4Core* parent)
{
    parent->saveToDebugLog("Initializing a new Scene...");

    std::cout << "[SCENE] Initializing...";
    thisConfig = thisConfigs;

    v4Core = parent;
    std::cout << " done" << std::endl;

    parent->saveToDebugLog("Scene initialized.");
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
