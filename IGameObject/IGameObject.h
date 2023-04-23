#ifndef GAME_IGAMEOBJECT_H
#define GAME_IGAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "../IVisitor/IVisitor.h"

enum Destination
{
    UP,
    LEFT,
    DOWN,
    RIGHT
};

class IGameObject
{
protected:

    sf::Sprite sprite;
    float dx, dy;
public:
    IGameObject(float x, float y) : dx(x), dy(y)
    {
        sprite.setTexture((IGameObject::texture));
    }
    static sf::Texture texture;
    virtual ~IGameObject() = default;
    virtual void update(float time) = 0;
    virtual void render(sf::RenderWindow &window) = 0;
    virtual void handleCollision(IVisitor *visitor) = 0;
};


#endif
