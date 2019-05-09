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

    /** Make Patapon walk (temporary) **/

    if(walk)
    {
        followobject_x += (2 * 60) / fps;
    }

    /** Temporary movement code **/

    if(camera_x > followobject_x + 500)
    {
        camera_xspeed = -(camera_x - followobject_x - 400) / 50;
    }
    else if(camera_x < followobject_x + 400)
    {
        camera_xspeed = -(camera_x - followobject_x - 400) / 50;
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

    /** Apply zoom **/

    zoom += ((dest_zoom - zoom) / 20);
    if((zoom > 0.9999) && (zoom < 1.0001))
    zoom = 1;
    cout << "zoom: " << zoom << endl;
    cout << "dest_zoom: " << dest_zoom << endl;

    zoomViewAt(sf::Vector2i(window.mapCoordsToPixel(sf::Vector2f(followobject_x,610)).x,610),window,zoom,fps);

    /** Apply camera position **/

    sf::View view{window.getView()};
    view.setCenter(camera_x+zoom_x,camera_y+zoom_y);
    window.setView(view);
}
