#include "Camera.h"
#include <iostream>

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


void Camera::Work(sf::RenderWindow& window,float fps)
{
    dest_zoom = 1;
    manual_x_dest = 0;

    /** Debug controls **/

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    {
        dest_zoom = 1.01;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        dest_zoom = 0.99;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
    {
        camera_x -= 5;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        camera_x += 5;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        manual_x_dest = -500;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        manual_x_dest = 500;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
    {
        debug_x_dest -= debug_x_speed;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
    {
        debug_x_dest += debug_x_speed;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
    {
        debug_x_dest = 0;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
    {
        debug_x_speed -= 10;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
    {
        debug_x_speed += 10;
    }

    if(debug_x_speed < 10)
    debug_x_speed = 10;

    /** Make Patapon walk (temporary) **/

    if(walk)
    {
        followobject_x += (2 * 60) / fps;
    }

    /** Temporary movement code **/

    if(camera_x > followobject_x + 500)
    {
        camera_xspeed = -(camera_x - followobject_x - 400) / 20;
    }
    else if(camera_x < followobject_x + 400)
    {
        camera_xspeed = -(camera_x - followobject_x - 400) / 20;
    }
    else
    {
        camera_xspeed = 0;
    }

    if(camera_xspeed >= 2)
    {
        camera_xspeed = 2;
    }

    if(abs(camera_xspeed) <= 0.05)
    {
        camera_xspeed = 0;
    }

    /** Move camera **/

    camera_x += (camera_xspeed * 60) / fps;

    /** Manual camera movement (L/R in Patapon) **/
    manual_x += ((manual_x_dest - manual_x) / 20);

    /** Debug camera movement **/
    debug_x += ((debug_x_dest - debug_x) / 20);

    /** Apply zoom **/

    zoom += ((dest_zoom - zoom) / 20);

    if(dest_zoom == 1)
    {
        if((zoom > 0.9995) && (zoom < 1.0005))
        zoom = 1;
    }

    //cout << "zoom: " << zoom << endl;
    //cout << "dest_zoom: " << dest_zoom << endl;

    zoomViewAt(sf::Vector2i(250,610),window,zoom,fps);

    /** Apply camera position **/

    sf::View view{window.getView()};
    view.setCenter(camera_x+zoom_x+manual_x+debug_x,camera_y+zoom_y+manual_y);
    window.setView(view);
}
