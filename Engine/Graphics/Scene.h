#ifndef SCENE_H
#define SCENE_H
#include <SFML/Graphics.hpp>
#include "../Config.h"

class V4Core;
class Scene
{
    public:
        V4Core *v4Core;

        Config *thisConfig;

        virtual void Initialise(Config *thisConfig, V4Core *parent);
        virtual void Update(sf::RenderWindow &window, float fps);
        virtual void EventFired(sf::Event event);
        virtual void OnExit();
        Scene();
        ~Scene();

};


#endif // SCENE_H
