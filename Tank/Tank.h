#ifndef GAME_TANK_H
#define GAME_TANK_H

#include "../IGameObject/IGameObject.h"

extern float FACTOR;

class Bullet;

class Tank : public IGameObject
{
protected:
    Destination tankDestination;
    int health;
    int animation;
    bool canShoot;
    float speed;
    Bullet *bullet;
    std::vector<IGameObject*>& allBullets;
public:
    Tank(float x, float y, float speed, int health, std::vector<IGameObject*>& allBullets);
    ~Tank() override;
    void render(sf::RenderWindow &window) override;
    virtual void move(float distance) = 0;
    virtual void shoot() = 0;
};

class PlayerTank : public Tank
{
protected:
    int stars;
public:
    PlayerTank(float x, float y, std::vector<IGameObject*>& allBullets);
    void update(float time) override;
    void move(float distance) override;
    void shoot() override;
    void addStar();
    void checkCollision(IVisitor *visitor) override;
};

#endif
