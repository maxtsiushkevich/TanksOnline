#ifndef GAME_TANK_H
#define GAME_TANK_H

#include "../IGameObject/IGameObject.h"

extern float FACTOR;

class Bullet;

class Tank : public IGameObject {
protected:
    Destination tankDestination;
    int health;
    int animation;
    bool canShoot;
    float speed;
    std::unique_ptr<IGameObject> bullet;
    std::vector<std::unique_ptr<IGameObject>> &allBullets;
public:
    Tank(float x, float y, float speed, int health, std::vector<std::unique_ptr<IGameObject>> &allBullets);
    ~Tank() override = default;
    void render(sf::RenderWindow &window) override;
    virtual void move(float distance) = 0;
    virtual void shoot() = 0;
    void enableShooting();
    bool isShootingEnable() const;
};

class PlayerTank : public Tank {
protected:
    int stars;
public:
    PlayerTank(float x, float y, std::vector<std::unique_ptr<IGameObject>> &allBullets);

    void update(float time) override;

    void move(float distance) override;

    void shoot() override;

    void addStar();

    void handleCollision(IVisitor *visitor) override;
};

#endif
