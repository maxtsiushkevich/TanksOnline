#ifndef GAME_BULLET_H
#define GAME_BULLET_H

#define PLAYER_BULLET_SPEED 110
#define PLAYER_FAST_BULLET_SPEED 220
#define ENEMY_BULLET_SPEED 90
#define ENEMY_FAST_BULLET_SPEED 180

#include "../headers/IGameObject.h"
#include "../headers/Tank.h"

class Bullet : public IGameObject {
    friend class GameState;
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
    std::shared_ptr<IGameObject> getOwner() const;

    int getDestination() { return bulletDestination; }
    void setDestination(int newDestination) { bulletDestination = static_cast<Destination>(newDestination); }
};

class PlayerBullet : public Bullet // пуля игрока
{
    friend class GameState;
public:
    PlayerBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;
};

class PlayerFastBullet : public Bullet {
    friend class GameState;
public:
    PlayerFastBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;
};

class PlayerPowerfulBullet : public Bullet {
public:
    PlayerPowerfulBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;
};

class EnemyBullet : public Bullet // пуля врага
{
    friend class GameState;
public:
    EnemyBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;
};

class EnemyFastBullet : public Bullet // пуля врага
{
    friend class GameState;
public:
    EnemyFastBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner);
    void handleCollision(IVisitor *visitor) override;
};

#endif
