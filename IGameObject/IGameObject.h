#ifndef GAME_IGAMEOBJECT_H
#define GAME_IGAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "../IVisitor/IVisitor.h"

#include <memory>

enum Destination {
    UP,
    LEFT,
    DOWN,
    RIGHT
};

class IGameObject : public std::enable_shared_from_this<IGameObject>
{
protected:
    bool isDestroyed;
    sf::Sprite sprite;
    float dx, dy;
public:
    static sf::Texture texture;
    IGameObject(float x, float y);
    virtual ~IGameObject() = default;
    virtual void update(float time) = 0;
    virtual void render(sf::RenderWindow &window) = 0;
    virtual void handleCollision(IVisitor *visitor) = 0;
    virtual void setIsDestroyed();
    bool getIsDestroyed();
    sf::Sprite &getSprite();
};


#endif
