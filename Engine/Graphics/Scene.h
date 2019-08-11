#ifndef SCENE_H
#define SCENE_H
#include <SFML/Graphics.hpp>
#include "../Config.h"
#include "../Mission/MissionController.h"
class V4Core;
class Scene
{
    public:
        V4Core *v4core;

        Config *thisConfig;
        std::map<int,bool> *keyMap;

        virtual void Initialise(Config *thisConfig, std::map<int,bool> *keymap,V4Core *parent);
        virtual void Update(sf::RenderWindow &window, float fps);
        virtual void EventFired(sf::Event event);
        virtual void OnExit();
        Scene();
        ~Scene();

};


#endif // SCENE_H
