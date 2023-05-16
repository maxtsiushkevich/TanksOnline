#ifndef GAME_MAPOBJECT_H
#define GAME_MAPOBJECT_H

#include "../headers/IGameObject.h"

extern float FACTOR;

class MapObject : public IGameObject {
    friend class GameState;
public:
    MapObject(float x, float y);
};

class Brick : public MapObject {
    friend class GameState;
public:
    Brick(float x, float y);
    void update(float time) override;
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;

};

class Metal : public MapObject {
    friend class GameState;
public:
    Metal(float x, float y);
    void update(float time) override;
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Water : public MapObject {
    friend class GameState;
public:
    Water(float x, float y);
    void update(float time) override;
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Grass : public MapObject {
    friend class GameState;
public:
    Grass(float x, float y);
    void update(float time) override;
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Eagle : public MapObject
{
    friend class GameState;
protected:
    bool isFallen;
public:
    Eagle(float x, float y);
    void update(float time) override;
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
    void setIsFallen();
    bool getIsFallen();
};

#endif //GAME_MAPOBJECT_H
