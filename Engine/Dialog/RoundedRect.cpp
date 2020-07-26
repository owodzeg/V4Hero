#include "RoundedRect.h"

RoundedRect::RoundedRect()
{

}

void RoundedRect::Create(float nwidth, float nheight, float nedge_size, float resRatio)
{
    for(int i=0; i<4; i++)
    {
        edges[i].setRadius(nedge_size*resRatio);
        edges[i].setFillColor(sf::Color(255,237,210,255));
        connections[i].setFillColor(sf::Color(255,237,210,255));
    }

    connections[0].setSize(sf::Vector2f(nwidth*resRatio, nedge_size*2*resRatio)); ///top
    connections[1].setSize(sf::Vector2f(nwidth*resRatio, nedge_size*2*resRatio)); ///bottom
    connections[2].setSize(sf::Vector2f(nedge_size*2*resRatio, nheight*resRatio)); ///left
    connections[3].setSize(sf::Vector2f(nedge_size*2*resRatio, nheight*resRatio)); ///right

    contents.setSize(sf::Vector2f((nwidth-nedge_size*2)*resRatio, (nheight-nedge_size*2)*resRatio));
    contents.setFillColor(sf::Color(255,237,210,255));

    width = nwidth;
    height = nheight;
    edge_size = nedge_size;
}

void RoundedRect::setOrigin(sf::Vector2f origin)
{
    orx = origin.x;
    ory = origin.y;
}

void RoundedRect::Draw(sf::RenderWindow& window)
{
    float resRatio = window.getSize().x / float(1280);

    edges[0].setPosition((x-orx)*resRatio,(y-ory)*resRatio);
    edges[1].setPosition((x-orx+width)*resRatio,(y-ory)*resRatio);
    edges[2].setPosition((x-orx)*resRatio,(y-ory+height)*resRatio);
    edges[3].setPosition((x-orx+width)*resRatio,(y-ory+height)*resRatio);

    connections[0].setPosition((x-orx+edge_size)*resRatio,(y-ory)*resRatio);
    connections[1].setPosition((x-orx+edge_size)*resRatio,(y-ory+height)*resRatio);
    connections[2].setPosition((x-orx)*resRatio,(y-ory+edge_size)*resRatio);
    connections[3].setPosition((x-orx+width)*resRatio,(y-ory+edge_size)*resRatio);

    contents.setPosition((x-orx+edge_size*2)*resRatio, (y-ory+edge_size*2)*resRatio);

    for(int i=0; i<4; i++)
    {
        window.draw(edges[i]);
        window.draw(connections[i]);
    }

    window.draw(contents);
}
