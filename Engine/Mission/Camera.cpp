#include "Camera.h"
#include <iostream>

using namespace std;


Camera::Camera()
{
    //cameraView.setSize(1280, 720);
    //cameraView.setCenter(0, 610);
    camera_x = 480;
}
void Camera::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom,float fps)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	offsetCoords.x = (camera_xspeed*60/fps);
	view.move(offsetCoords);
	window.setView(view);
}


void Camera::Work(sf::RenderWindow& window,float fps)
{
    zoom = 1;

    /** Debug controls **/

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    {
        zoom = 1.002;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        zoom = 0.998;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
    {
        camera_x -= 5;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        camera_x += 5;
    }

    if(walk)
    {
        followobject_x += (2 * 60) / fps;
    }

    /** Move camera **/

    sf::View view{window.getView()};
    view.setCenter(camera_x,view.getCenter().y);
    window.setView(view);
}
