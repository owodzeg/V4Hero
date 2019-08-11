#include "Scene.h"
#include "iostream"
#include "../V4Core.h"
Scene::Scene()
{
    //ctor
}
void Scene::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent){
    thisConfig = thisConfigs;

    v4core = parent;
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
