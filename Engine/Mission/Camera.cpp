#include "Camera.h"
#include <iostream>

using namespace std;

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}

Camera::Camera()
{
    //cameraView.setSize(1280, 720);
    //cameraView.setCenter(0, 610);
}

void Camera::Work(sf::RenderWindow& window)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    {
        zoom += 0.00001;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        zoom -= 0.00001;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        cameraView.setCenter(0,cameraView.getCenter().y+1);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        cameraView.setCenter(0,cameraView.getCenter().y-1);
    }

    cout << (cameraView.getSize().x / float(1280)) << " " << (cameraView.getSize().y / float(720)) << endl;

    zoomViewAt(sf::Vector2i(640,610),window,zoom);
    //cameraView.move(0, (cameraView.getSize().x / float(1280)));
    //cameraView.setCenter(640,cameraView.getSize().y/2);
    //window.setView(cameraView);
}
