#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#include "../IGameObject/IGameObject.h"

extern double FACTOR;

class Bullet : public IGameObject {
protected:
    double speed;
    IGameObject *owner;
    Destination bulletDestination;
public:
    Bullet(double x, double y, double speed, Destination dest, IGameObject *owner);
    ~Bullet() override = default;
    void update(double time) override;
    void render(sf::RenderWindow &window) override;
    bool checkBounds();
    IGameObject* getOwner() const;
};

class PlayerBullet : public Bullet // пуля игрока
{
public:
    PlayerBullet(double x, double y, Destination dest, IGameObject *owner) : Bullet(x, y, 110.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class PlayerFastBullet : public Bullet {
public:
    PlayerFastBullet(double x, double y, Destination dest, IGameObject *owner) : Bullet(x, y, 220.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class PlayerPowerfulBullet : public Bullet {
public:
    PlayerPowerfulBullet(double x, double y, Destination dest, IGameObject *owner) : Bullet(x, y, 110.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class EnemyBullet : public Bullet // пуля врага
{
public:
    EnemyBullet(double x, double y, Destination dest, IGameObject *owner) : Bullet(x, y, 110.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class EnemyFastBullet : public Bullet // пуля врага
{
public:
    EnemyFastBullet(double x, double y, Destination dest, IGameObject *owner) : Bullet(x, y, 220.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

#endif
