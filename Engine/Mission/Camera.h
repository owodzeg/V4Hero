#ifndef CAMERA_H
#define CAMERA_H

#include "../Input/InputController.h"
#include <SFML/Graphics.hpp>

class Camera
{
public:
    float zoom = 1, dest_zoom = 1;
    float camera_x = 0, camera_y = 360;
    float zoom_x = 0, zoom_y = 0;
    float manual_x = 0, manual_y = 0, manual_x_dest = 0, manual_y_dest = 0;
    float debug_x = 0, debug_x_dest = 0, debug_x_speed = 5;
    float manual_xspeed = 0;
    float camera_xspeed = 0;
    float followobject_x = 341; ///Point at which camera should follow
    float pataSpeed = 0;
    bool walk = false;
    bool missionEnd = false;
    bool lockMovement = false;

    float zoom_point_x = 0;
    float zoom_point_y = 0;

    Camera();
    void zoomViewAt(sf::Vector2i pixel, float zoom);
    void Work(sf::View view, float = 1.0f);
};


#endif // CAMERA_H
