#ifndef GAME_TANK_H
#define GAME_TANK_H

#include "../IGameObject/IGameObject.h"

#include <iostream>

#define TANK_SPEED 45.f
#define ENEMY_TANK_SPEED 35.f
#define CARRIER_SPEED 90.f
#define INVULNERABLE_TIME 5.f
#define DELAY_BEFORE_SHOOT 1.f

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
    bool isInvulnerable;
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
    void addHealth() { health++; }
    void setIsInvulnerable();
};

class EnemyTank : public Tank
{
protected:
    bool isColliding;
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
    void setIsColliding() { isColliding = true;}
    void handleCollision(IVisitor *visitor) override;
    void enableShooting() override;
    bool getIsBonusTank() const { return isBonusTank; }
    int getType() const { return type; }
};

#endif
