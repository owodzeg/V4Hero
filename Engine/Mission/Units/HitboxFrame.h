#ifndef HITBOXFRAME_H
#define HITBOXFRAME_H

#include <SFML/Graphics.hpp>

///Class for managing object hitboxes defined by V4Anim format

class HitboxFrame
{
    private:
        ///timestamp
        ///global position, so the hitbox can be moved around as a whole


        ///hitboxes' rect that react with other collidables
        std::vector<sf::Vector2f> vertices;

        //float x;
        //float y;
        //float width;
        //float height;
    public:
        ///timestamp
        float time;
        ///global position, so the hitbox can be moved around as a whole
        float g_x=0;
        float g_y=0;
        ///rotation (in radians)
        float rotation=0;
        float scaleX=1;
        float scaleY=1;

        void clearVertices();
        void addVertex(float relX, float relY);
        std::vector<sf::Vector2f> getCurrentVertices();
        std::vector<sf::Vector2f>* getBaseVerticiesDontUseThisUnlessYouKnowWhy();
        float minProjection(float axisAngle);
        float maxProjection(float axisAngle);

        HitboxFrame();
        ~HitboxFrame();
};

#endif // HITBOXFRAME_H
