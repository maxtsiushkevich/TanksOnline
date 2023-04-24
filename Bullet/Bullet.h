#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#include "../IGameObject/IGameObject.h"

extern float FACTOR;

class Bullet : public IGameObject {
protected:
    float speed;
    bool isDestroyed;
    IGameObject *owner;
    Destination bulletDestination;
public:
    Bullet(float x, float y, float speed, Destination dest, IGameObject *owner);
    ~Bullet() override = default;
    void update(float time) override;
    void render(sf::RenderWindow &window) override;
    bool getIsDestroyed() const;
    bool checkBounds();
    IGameObject* getOwner() const;
};

class PlayerBullet : public Bullet // пуля игрока
{
public:
    PlayerBullet(float x, float y, Destination dest, IGameObject *owner) : Bullet(x, y, 110.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class PlayerFastBullet : public Bullet {
public:
    PlayerFastBullet(float x, float y, Destination dest, IGameObject *owner) : Bullet(x, y, 220.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class PlayerPowerfulBullet : public Bullet {
public:
    PlayerPowerfulBullet(float x, float y, Destination dest, IGameObject *owner) : Bullet(x, y, 110.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class EnemyBullet : public Bullet // пуля врага
{
public:
    EnemyBullet(float x, float y, Destination dest, IGameObject *owner) : Bullet(x, y, 110.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class EnemySpeedBullet : public Bullet // пуля врага
{
public:
    EnemySpeedBullet(float x, float y, Destination dest, IGameObject *owner) : Bullet(x, y, 220.f, dest, owner) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

#endif
