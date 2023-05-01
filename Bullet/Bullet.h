#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#define PLAYER_BULLET_SPEED 110.f
#define PLAYER_FAST_BULLET_SPEED 220.f
#define ENEMY_BULLET_SPEED 90.f
#define ENEMY_FAST_BULLET_SPEED 180.f

#include "../IGameObject/IGameObject.h"
#include "../Tank/Tank.h"

class Bullet : public IGameObject {
protected:
    float speed;

   std::shared_ptr<IGameObject> owner;

    Destination bulletDestination;
public:
    Bullet(float x, float y, float speed, Destination dest, std::shared_ptr<IGameObject> owner);
    ~Bullet() override = default;
    void update(float time) override;
    void render(sf::RenderWindow &window) override;
    bool checkBounds();
    void setIsDestroyed() override;
    std::shared_ptr<IGameObject> getOwner() const;
};

class PlayerBullet : public Bullet // пуля игрока
{
public:
    PlayerBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner) : Bullet(x, y, PLAYER_BULLET_SPEED, dest, std::move(owner)) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class PlayerFastBullet : public Bullet {
public:
    PlayerFastBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner) : Bullet(x, y, PLAYER_FAST_BULLET_SPEED, dest, std::move(owner)) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class PlayerPowerfulBullet : public Bullet {
public:
    PlayerPowerfulBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner) : Bullet(x, y, PLAYER_BULLET_SPEED, dest, std::move(owner)) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class EnemyBullet : public Bullet // пуля врага
{
public:
    EnemyBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner) : Bullet(x, y, ENEMY_BULLET_SPEED, dest, std::move(owner)) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

class EnemyFastBullet : public Bullet // пуля врага
{
public:
    EnemyFastBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner) : Bullet(x, y, ENEMY_FAST_BULLET_SPEED, dest, std::move(owner)) {}

    void handleCollision(IVisitor *visitor) override {
        visitor->visit(*this);
    }
};

#endif
