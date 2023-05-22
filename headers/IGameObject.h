#ifndef GAME_IGAMEOBJECT_H
#define GAME_IGAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "IVisitor.h"
#include <sstream>
#include <memory>


#define PLAYER_SPAWN_X 64
#define PLAYER_SPAWN_Y 192
#define ALLY_SPAWN_X 128
#define ALLY_SPAWN_Y 192

enum Destination {
    UP,
    LEFT,
    DOWN,
    RIGHT
};



class IGameObject : public std::enable_shared_from_this<IGameObject>
{
    friend class GameState;

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

    sf::Sprite &getSprite();

    void setIsDestroyed();
    void setIsDestroyed(bool newState) { isDestroyed = newState; }
    bool getIsDestroyed();

    float getX() { return dx; };
    float getY() { return dy; };

    void setX(float x) {
        dx = x;
        sprite.setPosition(dx, dy);
    };
    void setY(float y) {
        dy = y;
        sprite.setPosition(dx, dy);
    };
};


#endif
