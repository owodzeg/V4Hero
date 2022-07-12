#include "Camera.h"
#include <iostream>
#include <math.h> /* fabs */
#include "../CoreManager.h"

using namespace std;

Camera::Camera()
{
    camera_x = 480;
}

void Camera::zoomViewAt(sf::Vector2i pixel, float zoom)
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    const sf::Vector2f beforeCoord{window->mapPixelToCoords(pixel)};
    sf::View view{window->getView()};
    view.zoom(zoom);
    window->setView(view);
    sf::Vector2f afterCoord{window->mapPixelToCoords(pixel)};
    sf::Vector2f offsetCoords{beforeCoord - afterCoord};

    zoom_x += offsetCoords.x;
    zoom_y += offsetCoords.y;
}

void Camera::Work()
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    float resRatioX = window->getSize().x / float(1280);
    float resRatioY = window->getSize().y / float(720);

    camera_y = window->getSize().y / 2;

    dest_zoom = 1;
    manual_x_dest = 0;

    /** Debug controls **/

    if (!missionEnd)
    {
        if (inputCtrl->isKeyHeld(InputController::Keys::LTRIGGER))
        {
            manual_x_dest = -500;
        }

        if (inputCtrl->isKeyHeld(InputController::Keys::RTRIGGER))
        {
            manual_x_dest = 500;
        }
    }

    /** Move camera **/

    //camera_x += camera_xspeed / fps;
    camera_x = followobject_x + (600 * resRatioX);

    /** Manual camera movement (L/R in Patapon) **/
    manual_x += ((manual_x_dest - manual_x) * 5) / fps;

    /** Debug camera movement **/
    debug_x += ((debug_x_dest - debug_x) * 5) / fps;

    /** Apply zoom **/
    zoom += ((dest_zoom - zoom) * 5) / fps;

    if (dest_zoom == 1)
    {
        float zoomLower, zoomUpper;

        if (fps >= 120)
            zoomLower = 0.9995 + (0.03 / (fps));
        else
            zoomLower = 0.9995;

        if (fps >= 120)
            zoomUpper = 1.0005 - (0.03 / (fps));
        else
            zoomUpper = 1.0005;

        if ((zoom > zoomLower) && (zoom < zoomUpper))
            zoom = 1;
    }

    if (dest_zoom > 1)
    {
        if (zoom >= dest_zoom)
            zoom = dest_zoom;
    } else if (dest_zoom < 1)
    {
        if (zoom <= dest_zoom)
            zoom = dest_zoom;
    }

    zoomViewAt(sf::Vector2i(250, window->getSize().y - 110), zoom);

    /** Apply camera position **/

    sf::View view{window->getView()};
    view.setCenter(camera_x + zoom_x + manual_x + debug_x - 100, camera_y + zoom_y + manual_y);
    window->setView(view);
}
