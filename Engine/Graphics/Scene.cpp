#include "Scene.h"
#include <iostream>
#include "../V4Core.h"
Scene::Scene()
{
    //ctor
}
void Scene::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent){
    parent->SaveToDebugLog("Initializing a new Scene...");

    std::cout << "[SCENE] Initializing...";
    thisConfig = thisConfigs;

    v4core = parent;
    std::cout << " done" << std::endl;

    parent->SaveToDebugLog("Scene initialized.");
}
void Scene::EventFired(sf::Event event){

}
void Scene::Update(sf::RenderWindow &window, float fps)
{

}
void Scene::OnExit(){

}

Scene::~Scene()
{
    //dtor
}
