#ifndef GAME_TANK_H
#define GAME_TANK_H

#include "../headers/IGameObject.h"
#include <iostream>

#define TANK_SPEED 45
#define ENEMY_TANK_SPEED 45
#define CARRIER_SPEED 90
#define INVULNERABLE_TIME 5
#define DELAY_BEFORE_SHOOT 1

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
    friend class GameState;
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
    ~Tank() = default;
    void render(sf::RenderWindow &window) override;
    virtual void move(float distance) = 0;
    virtual void shoot() = 0;
    virtual void enableShooting();
    float getTime() const;
    void setTime(float newTime) { time = newTime; }
    float getSpeed() const;
    int getHealth() const;
    void setHealth(int newHealth) { health = newHealth; }
    void decrementAnimation();

    int getDestination() { return tankDestination; }
    void setDestination(int newDest) { tankDestination = static_cast<Destination>(newDest); }
};

class PlayerTank : public Tank
        {
    friend class GameState;
    friend class CollisionWithMapObjectVisitor;
protected:
    bool isInvulnerable;
    Destination previousButton;
    bool isShootButtonPressed;
    int stars;
    sf::Clock invulnerableTime;
    bool isAllyTank;
public:
    PlayerTank(float x, float y, std::vector<std::shared_ptr<IGameObject>> &allBullets, bool isAllyTank);
    ~PlayerTank() = default;
    void update(float time) override;
    void move(float distance) override;
    void shoot() override;
    void addStar();
    void reset();
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
    void addHealth();

    bool getIsInvulnerable() const;
    void setIsInvulnerable();
    void setIsInvulnerable(bool newState) { isInvulnerable = newState; };

    void setStars(int newStars) { stars = newStars; }
    int getStars() { return stars; }

    void setAnimation(int animation) { this->animation = animation; }
    int getAnimation() { return stars; }

    bool getCanShoot() { return canShoot; }
    void setCanShoot(bool newState) { canShoot = newState; }

    void setSprite() {
        if (tankDestination == LEFT)
            sprite.setTextureRect(sf::IntRect(32 + (16 * animation), stars * 16, 16, 16));
        else if (tankDestination == RIGHT)
            sprite.setTextureRect(sf::IntRect(96 + (16 * animation), stars * 16, 16, 16));
        else if (tankDestination == DOWN)
            sprite.setTextureRect(sf::IntRect(64 + (16 * animation), stars * 16, 16, 16));
        else if (tankDestination == UP)
            sprite.setTextureRect(sf::IntRect(0 + (16 * animation), stars * 16, 16, 16));
    }
};

class EnemyTank : public Tank
{
    friend class GameState;
    friend class CollisionWithMapObjectVisitor;
protected:
    bool isColliding;
    EnemyType type;
    int randDest;
    bool isBonusTank;
    sf::Clock delayBeforeShoot;
public:
    EnemyTank(float x, float y, std::vector<std::shared_ptr<IGameObject>> &allBullets, int type, bool isBonusTank);
    ~EnemyTank() = default;
    void update(float time) override;
    void move(float distance) override;
    void shoot() override;
    void decrementHealth();
    void setIsColliding();
    void handleCollision(IVisitor *visitor) override;
    void enableShooting() override;
    bool getIsBonusTank() const;

    void setType(int newType) { type = static_cast<EnemyType>(newType); }
    int getType() const;

    void setIsBonusTank(bool newState) { isBonusTank = newState; }

    void setSprite() {
        if (tankDestination == LEFT)
            sprite.setTextureRect(sf::IntRect(160 + (16 * animation), 64 + (static_cast<int>(type) * 16), 16, 16));
        else if (tankDestination == RIGHT)
            sprite.setTextureRect(sf::IntRect(224 + (16 * animation), 64 + (static_cast<int>(type) * 16), 16, 16));
        else if (tankDestination == DOWN)
            sprite.setTextureRect(sf::IntRect(192 + (16 * animation), 64 + (static_cast<int>(type) * 16), 16, 16));
        else if (tankDestination == UP)
            sprite.setTextureRect(sf::IntRect(128 + (16 * animation), 64 + (static_cast<int>(type) * 16), 16, 16));
    }
};

#endif
