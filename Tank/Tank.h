#ifndef GAME_TANK_H
#define GAME_TANK_H

#include "../IGameObject/IGameObject.h"

#include <iostream>

extern float FACTOR;

enum EnemyType
{
    EnemyBaseTank,
    EnemyCarrier,
    EnemyShootingTank,
    EnemyHeavyTank
};

class Bullet;

class Tank : public IGameObject {
protected:
    Destination tankDestination;
    int health;
    int animation;
    bool canShoot;
    float speed;
    float time;

    std::shared_ptr<IGameObject> bullet;
    std::vector<std::shared_ptr<IGameObject>> &allBullets;

public:
    Tank(float x, float y, float speed, int health, std::vector<std::shared_ptr<IGameObject>> &allBullets);
    ~Tank() override = default;
    void render(sf::RenderWindow &window) override;
    virtual void move(float distance) = 0;
    virtual void shoot() = 0;
    virtual void enableShooting();
    float getTime() const { return time; }
    float getSpeed() const { return speed; }
    int getHealths() const { return health; }
    void decrementAnimation() { animation--; }
};

class PlayerTank : public Tank {
protected:
    int isInvulnerable;
    Destination previousButton;
    bool isSpacePressed;
    int stars;
    sf::Clock invulnerableTime;
public:
    PlayerTank(float x, float y, std::vector<std::shared_ptr<IGameObject>> &allBullets);
    void update(float time) override;
    void move(float distance) override;
    void shoot() override;
    void addStar();
    void reset();
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
    bool getIsInvulnerable() const { return isInvulnerable; }
};

class EnemyTank : public Tank
{
protected:
    bool isCollidingWithMap;
    EnemyType type;
    int randDest;
    bool isBonusTank;
    sf::Clock delayBeforeShoot;
public:
    EnemyTank(float x, float y, std::vector<std::shared_ptr<IGameObject>> &allBullets, int type, bool isBonusTank);
    ~EnemyTank() override = default;
    void update(float time) override;
    void move(float distance) override;
    void shoot() override;
    void decrementHealth();
    void setIsCollidingWithMap() { isCollidingWithMap = true;}
    void handleCollision(IVisitor *visitor) override;
    void enableShooting() override;
    bool getIsBonusTank() const { return isBonusTank; }
};

#endif
