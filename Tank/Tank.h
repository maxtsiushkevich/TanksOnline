#ifndef GAME_TANK_H
#define GAME_TANK_H

#include "../IGameObject/IGameObject.h"

extern double FACTOR;

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
    double speed;
    double time;

    std::shared_ptr<IGameObject> bullet;
    std::vector<std::shared_ptr<IGameObject>> &allBullets;

public:
    Tank(double x, double y, double speed, int health, std::vector<std::shared_ptr<IGameObject>> &allBullets);
    ~Tank() override = default;
    void render(sf::RenderWindow &window) override;
    virtual void move(double distance) = 0;
    virtual void shoot() = 0;
    void enableShooting();
    bool isShootingEnable() const;
    Destination getDestination();
    double getTime() const { return time; }
    double getSpeed() const { return speed; }
    void decrementAnimation() { animation--; }
    int getHealths() { return health; }
};

class EnemyTank : public Tank
{
protected:
    bool isCollidingWithMap;
    EnemyType type;
    int randDest;
public:
    EnemyTank(double x, double y, std::vector<std::shared_ptr<IGameObject>> &allBullets, int type);
    void update(double time) override;
    void move(double distance) override;
    void shoot() override;
    void decrementHealth();
    void setIsCollidingWithMap() { isCollidingWithMap = true;}
    void handleCollision(IVisitor *visitor) override;
};

class PlayerTank : public Tank {
protected:
    Destination previousButton;
    bool isSpacePressed;
    int stars;
public:
    PlayerTank(double x, double y, std::vector<std::shared_ptr<IGameObject>> &allBullets);
    void update(double time) override;
    void move(double distance) override;
    void shoot() override;
    void addStar();
    void reset();
    void handleCollision(IVisitor *visitor) override;
};

#endif
