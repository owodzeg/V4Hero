#include "Camera.h"
#include <iostream>
#include <math.h>       /* fabs */

using namespace std;


Camera::Camera()
{
    camera_x = 480;
}
void Camera::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom,float fps)
{
	const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel)};
	sf::View view{window.getView()};
	view.zoom(zoom);
	window.setView(view);
	sf::Vector2f afterCoord{window.mapPixelToCoords(pixel)};
	sf::Vector2f offsetCoords{beforeCoord - afterCoord};

	zoom_x += offsetCoords.x;
	zoom_y += offsetCoords.y;
}


void Camera::Work(sf::RenderWindow& window,float fps, std::map<int,bool> *keyMap)
{
    camera_y = window.getSize().y/2;

    std::map<int,bool> cameraKeyMap = *keyMap;

    dest_zoom = 1;
    manual_x_dest = 0;

    /** Debug controls **/

    if(cameraKeyMap[sf::Keyboard::O])
    {
        if(fps >= 120)
        dest_zoom = 1.01 - (0.01 / (fps / float(60)));
        else
        dest_zoom = 1.01;
    }

    if(cameraKeyMap[sf::Keyboard::P])
    {
        if(fps >= 120)
        dest_zoom = 0.99 + (0.01 / (fps / float(60)));
        else
        dest_zoom = 0.99;
    }

    if(cameraKeyMap[sf::Keyboard::K])
    {
        camera_x -= 5;
    }

    if(cameraKeyMap[sf::Keyboard::L])
    {
        camera_x += 5;
    }

    if(cameraKeyMap[sf::Keyboard::Q])
    {
        manual_x_dest = -500;
    }

    if(cameraKeyMap[sf::Keyboard::E])
    {
        manual_x_dest = 500;
    }

    if(cameraKeyMap[sf::Keyboard::Num4])
    {
        debug_x_dest -= debug_x_speed;
    }

    if(cameraKeyMap[sf::Keyboard::Num6])
    {
        debug_x_dest += debug_x_speed;
    }

    if(cameraKeyMap[sf::Keyboard::Num5])
    {
        debug_x_dest = 0;
    }

    if(cameraKeyMap[sf::Keyboard::Num9])
    {
        debug_x_speed -= 10;
    }

    if(cameraKeyMap[sf::Keyboard::Num0])
    {
        debug_x_speed += 10;
    }

    if(debug_x_speed < 10)
    debug_x_speed = 10;



    /** Temporary movement code **/

    if(camera_x > followobject_x + 500)
    {
        camera_xspeed = (-(camera_x - followobject_x - 400) / 20);
    }
    else if(camera_x < followobject_x + 400)
    {
        camera_xspeed = (-(camera_x - followobject_x - 400) / 20);
    }
    else
    {
        camera_xspeed = 0;
    }

    if(camera_xspeed >= 2)
    {
        camera_xspeed = 2;
    }

    if(fabs(camera_xspeed) <= 0.05)
    {
        camera_xspeed = 0;
    }

    /** Move camera **/

    camera_x += camera_xspeed / fps * float(60);

    /** Manual camera movement (L/R in Patapon) **/
    manual_x += ((manual_x_dest - manual_x) / 20) / fps * float(60);

    /** Debug camera movement **/
    debug_x += ((debug_x_dest - debug_x) / 20) / fps * float(60);

    /** Apply zoom **/
    zoom += ((dest_zoom - zoom) / 20) / fps * float(60);

    if(dest_zoom == 1)
    {
        float zoomLower, zoomUpper;

        if(fps >= 120)
        zoomLower = 0.9995 + (0.0005 / (fps / float(60)));
        else
        zoomLower = 0.9995;

        if(fps >= 120)
        zoomUpper = 1.0005 - (0.0005 / (fps / float(60)));
        else
        zoomUpper = 1.0005;

        if((zoom > zoomLower) && (zoom < zoomUpper))
        zoom = 1;
    }

    if(dest_zoom > 1)
    {
        if(zoom >= dest_zoom)
        zoom = dest_zoom;
    }
    else if(dest_zoom < 1)
    {
        if(zoom <= dest_zoom)
        zoom = dest_zoom;
    }

    zoomViewAt(sf::Vector2i(250,window.getSize().y-110),window,zoom,fps);

    /** Apply camera position **/

    sf::View view{window.getView()};
    view.setCenter(camera_x+zoom_x+manual_x+debug_x,camera_y+zoom_y+manual_y);
    window.setView(view);
}
