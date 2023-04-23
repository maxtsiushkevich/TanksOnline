#ifndef GAME_MAPOBJECT_H
#define GAME_MAPOBJECT_H

#include "../IGameObject/IGameObject.h"

extern float FACTOR;

class MapObject : public IGameObject
{
protected:
    bool isDestroyed;
public:
    MapObject(float x, float y) : IGameObject(x, y) { }
    void setIsDestroyed() { isDestroyed = true; }
    bool getIsDestroyed() { return isDestroyed; }
};

class Brick : public MapObject
{
public:
    Brick(float x, float y);
    void update(float time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;

};
class Metal : public MapObject
{
public:
    Metal(float x, float y);
    void update(float time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Water : public MapObject
{
public:
    Water(float x, float y);
    void update(float time) override {};
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
};

class Grass : public MapObject
{
public:
    Grass(float x, float y);
    void update(float time) override {};
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
