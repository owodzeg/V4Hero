#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera
{
    public:
    float zoom = 1, dest_zoom = 1;
    float camera_x = 0, camera_y = 360;
    float zoom_x = 0, zoom_y = 0;
    float camera_xspeed = 0;
    float followobject_x = 400; ///Point at which camera should follow
    bool walk = false;

    Camera();
    void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom,float fps);
    void Work(sf::RenderWindow& window,float fps);
};


#endif // CAMERA_H
