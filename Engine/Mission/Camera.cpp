#include "Camera.h"
#include <iostream>

using namespace std;

Camera::Camera()
{
    cameraView.setSize(1280, 720);
    cameraView.setCenter(0, 360);
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

    cameraView.zoom(zoom);
    //cameraView.move(0, (cameraView.getSize().x / float(1280)));
    //cameraView.setCenter(640,cameraView.getSize().y/2);
    window.setView(cameraView);
}
