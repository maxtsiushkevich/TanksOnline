#ifndef GAME_IGAMEOBJECT_H
#define GAME_IGAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "../IVisitor/IVisitor.h"

enum Destination {
    UP,
    LEFT,
    DOWN,
    RIGHT
};


class IGameObject {
protected:
    bool isDestroyed;
    sf::Sprite sprite;
    double dx, dy;
public:
    IGameObject(double x, double y);
    static sf::Texture texture;
    virtual ~IGameObject() = default;
    virtual void update(double time) = 0;
    virtual void render(sf::RenderWindow &window) = 0;
    virtual void handleCollision(IVisitor *visitor) = 0;
    bool getIsDestroyed();
    void setIsDestroyed();
    sf::Sprite &getSprite();
};


#endif
