#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera
{
    public:
    float zoom = 1;
    sf::View cameraView;

    Camera();
    void Work(sf::RenderWindow& window);
};

#endif // CAMERA_H
