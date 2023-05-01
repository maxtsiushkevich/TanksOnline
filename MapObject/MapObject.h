#ifndef GAME_MAPOBJECT_H
#define GAME_MAPOBJECT_H

#include "../IGameObject/IGameObject.h"

extern float FACTOR;

class MapObject : public IGameObject {
protected:
public:
    MapObject(float x, float y);
};

class Brick : public MapObject {
public:
    Brick(float x, float y);
    void update(float time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;

};

class Metal : public MapObject {
protected:
public:
    Metal(float x, float y);
    void update(float time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Water : public MapObject {
public:
    Water(float x, float y);
    void update(float time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Grass : public MapObject {
public:
    Grass(float x, float y);
    void update(float time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Eagle : public MapObject
{
protected:
    bool isFallen;
public:
    Eagle(float x, float y);
    void update(float time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
    void setIsFallen();
    bool getIsFallen() { return isFallen; }
};

#endif //GAME_MAPOBJECT_H
