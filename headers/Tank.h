#ifndef GAME_TANK_H
#define GAME_TANK_H

#include "../headers/IGameObject.h"

#include <iostream>

#define TANK_SPEED 45.f
#define ENEMY_TANK_SPEED 45.f
#define CARRIER_SPEED 400.f
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
    std::vector<std::shared_ptr<IGameObject>> &allBullets;

public:
    Tank(float x, float y, float speed, int health, std::vector<std::shared_ptr<IGameObject>> &allBullets);
    ~Tank() override = default;
    void render(sf::RenderWindow &window) override;
    virtual void move(float distance) = 0;
    virtual void shoot() = 0;
    virtual void enableShooting();
    float getTime() const;
    float getSpeed() const;
    int getHealths() const;
    void decrementAnimation();
};

class PlayerTank : public Tank {
protected:
    bool isInvulnerable;
    Destination previousButton;
    bool isShootButtonPressed;
    int stars;
    sf::Clock invulnerableTime;
    bool isAllyTank;
public:
    PlayerTank(float x, float y, std::vector<std::shared_ptr<IGameObject>> &allBullets, bool isAllyTank);
    ~PlayerTank() override = default;
    void update(float time) override;
    void move(float distance) override;
    void shoot() override;
    void addStar();
    void reset();
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
    bool getIsInvulnerable() const;
    void addHealth();
    void setIsInvulnerable();

    void setX(float x);
    void setY(float y);
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
    void setIsColliding();
    void handleCollision(IVisitor *visitor) override;
    void enableShooting() override;
    bool getIsBonusTank() const;
    int getType() const;
};

#endif
