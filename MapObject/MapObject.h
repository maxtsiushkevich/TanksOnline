#ifndef GAME_MAPOBJECT_H
#define GAME_MAPOBJECT_H

#include "../IGameObject/IGameObject.h"

extern double FACTOR;

class MapObject : public IGameObject {
protected:
public:
    MapObject(double x, double y);
};

class Brick : public MapObject {
public:
    Brick(double x, double y);
    void update(double time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;

};

class Metal : public MapObject {
protected:
public:
    Metal(double x, double y);
    void update(double time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Water : public MapObject {
public:
    Water(double x, double y);
    void update(double time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Grass : public MapObject {
public:
    Grass(double x, double y);
    void update(double time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

//class Eagle : public IGameObject
//{
//protected:
//    Eagle(float x, float y) : IGameObject(x, y)
//    {
//        //sprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
//        sprite.setPosition(x, y);
//        sprite.setScale(FACTOR, FACTOR);
//    }
//};
//
//class Flag : public IGameObject
//{
//protected:
//    Flag(float x, float y) : IGameObject(x, y)
//    {
//        //sprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
//        sprite.setPosition(x, y);
//        sprite.setScale(FACTOR, FACTOR);
//    }
//};

#endif //GAME_MAPOBJECT_H
