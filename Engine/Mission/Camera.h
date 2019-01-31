#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera
{
    public:
    float zoom = 1;
    float camera_x = 0;
    float camera_xspeed = 0;
    float followobject_x = 400; ///Point at which camera should follow
    bool walk = false;
    sf::View cameraView;

    Camera();
    void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom);
    void Work(sf::RenderWindow& window);
};

#endif // CAMERA_H
