#include "Tank.h"
#include "../Bullet/Bullet.h"
#include <iostream>

#include <random>
#include <chrono>

#define TANK_SPEED 45.f
#define ENEMY_TANK_SPEED 35.f
#define CARRIER_SPEED 90.f

Tank::Tank(double x, double y, double speed, int health, std::vector<std::shared_ptr<IGameObject>> &allBullets)
        : IGameObject(x, y), health(health),
          allBullets(allBullets) {
    animation = 0;
    tankDestination = UP;
    this->speed = speed * FACTOR;
    canShoot = true;
}

void Tank::enableShooting() {
    canShoot = true;
}

bool Tank::isShootingEnable() const {
    return canShoot;
}

Destination Tank::getDestination() {
    return tankDestination;
}

void Tank::render(sf::RenderWindow &window) {
    window.draw(sprite);
}

PlayerTank::PlayerTank(double x, double y, std::vector<std::shared_ptr<IGameObject>> &allBullets) : Tank(x, y,
                                                                                                         TANK_SPEED, 3,
                                                                                                         allBullets),
                                                                                                    stars(0) {
    isSpacePressed = false;
    sprite.setTextureRect(sf::IntRect(0, 16 * stars, 16, 16));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

void PlayerTank::reset() {
    health--;
    if (health == 0)
        isDestroyed = true;
    dx = 64 * FACTOR;
    dy = 192 * FACTOR;
    stars = 0;
    sprite.setTextureRect(sf::IntRect(0, 16 * stars, 16, 16));
    sprite.setPosition(dx, dy);
    sprite.setScale(FACTOR, FACTOR);
    animation = 0;
    tankDestination = UP;
    canShoot = true;
    isSpacePressed = false;
}

void PlayerTank::update(double time) {
    this->time = time;
    double distance = speed * time;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canShoot && !isSpacePressed) {
        this->shoot();
        isSpacePressed = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        isSpacePressed = true;
    else
        isSpacePressed = false;

    this->move(distance);
}

void PlayerTank::shoot() {
    switch (stars) {
        case 0:
            bullet = std::make_shared<PlayerBullet>(dx, dy, tankDestination, this);
            break;
        case 1:
            bullet = std::make_shared<PlayerFastBullet>(dx, dy, tankDestination, this);
            break;
        case 2:
            // очередь
        case 3:
            bullet = std::make_shared<PlayerPowerfulBullet>(dx, dy, tankDestination, this);
            break;
    }

    if (bullet) {
        allBullets.emplace_back(std::move(bullet));
        canShoot = false;
    }
}


void PlayerTank::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

void PlayerTank::move(double distance) {
    if (animation == 2) // обработка движения
        animation = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        sprite.setTextureRect(sf::IntRect(32 + (16 * animation), stars * 16, 16, 16));
        dx -= distance;
        if (sprite.getPosition().x < 0) // проверка на выезд за предел экрана
        {
            sprite.setPosition(0, sprite.getPosition().y);
            dx = 0;
        }
        if (previousButton != LEFT) {
            dx = round(dx / (8 * FACTOR)) * (8 * FACTOR);
            dy = round(dy / (8 * FACTOR)) * (8 * FACTOR);
        }
        sprite.setPosition(dx, dy);
        previousButton = LEFT;
        tankDestination = LEFT;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        sprite.setTextureRect(sf::IntRect(96 + (16 * animation), stars * 16, 16, 16));
        dx += distance;
        if (sprite.getPosition().x > 192 * FACTOR) // проверка на выезд за предел экрана
        {
            sprite.setPosition(192 * FACTOR, sprite.getPosition().y);
            dx = 192 * FACTOR;
        }
        if (previousButton != RIGHT) {
            dx = round(dx / (8 * FACTOR)) * (8 * FACTOR);
            dy = round(dy / (8 * FACTOR)) * (8 * FACTOR);
        }
        sprite.setPosition(dx, dy);
        previousButton = RIGHT;
        tankDestination = RIGHT;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        sprite.setTextureRect(sf::IntRect(64 + (16 * animation), stars * 16, 16, 16));
        dy += distance;
        if (sprite.getPosition().y > 192 * FACTOR) // проверка на выезд за предел экрана
        {
            sprite.setPosition(sprite.getPosition().x, 192 * FACTOR);
            dy = 192 * FACTOR;
        }
        if (previousButton != DOWN) {
            dx = round(dx / (8 * FACTOR)) * (8 * FACTOR);
            dy = round(dy / (8 * FACTOR)) * (8 * FACTOR);
        }
        sprite.setPosition(dx, dy);
        previousButton = RIGHT;
        tankDestination = DOWN;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        sprite.setTextureRect(
                sf::IntRect(0 + (16 * animation), stars * 16, 16, 16)); // по строкам в зависимости от звезд
        dy -= distance;
        if (sprite.getPosition().y < 0) // проверка на выезд за предел экрана
        {
            sprite.setPosition(sprite.getPosition().x, 0);
            dy = 0;
        }
        if (previousButton != UP) {
            dx = round(dx / (8 * FACTOR)) * (8 * FACTOR);
            dy = round(dy / (8 * FACTOR)) * (8 * FACTOR);
        }
        sprite.setPosition(dx, dy);
        previousButton = UP;
        tankDestination = UP;
    }
    previousButton = tankDestination;
    animation++;
}

void PlayerTank::addStar() {
    stars = (stars == 3) ? stars : stars++;
}

EnemyTank::EnemyTank(double x, double y, std::vector<std::shared_ptr<IGameObject>> &allBullets, int type) : Tank(x, y,
                                                                                                                 ENEMY_TANK_SPEED,
                                                                                                                 1,
                                                                                                                 allBullets) {
    isCollidingWithMap = true;
    this->type = static_cast<EnemyType>(type);
    if (this->type == EnemyCarrier)
        speed = CARRIER_SPEED;
    if (this->type == EnemyHeavyTank)
        health = 4;
    tankDestination = DOWN;
    sprite.setTextureRect(sf::IntRect(192, 64 + (16 * type), 16, 16));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

void EnemyTank::update(double time) {
    this->time = time;
    double distance = speed * time;
    if (canShoot) {
        std::cout << "пиу пиу" << std::endl;
        this->shoot();
    }

    this->move(distance);
}

void EnemyTank::shoot() {
    if (type != EnemyShootingTank)
        bullet = std::make_shared<EnemyBullet>(dx, dy, tankDestination, this);
    else bullet = std::make_shared<EnemyFastBullet>(dx, dy, tankDestination, this);

    if (bullet) {
        allBullets.emplace_back(std::move(bullet));
        canShoot = false;
    }
}

void EnemyTank::move(double distance)
{
//    if (animation == 2) // обработка движения
//        animation = 0;
//
//    if (isCollidingWithMap) {
//        isCollidingWithMap = false;
//
//        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//        std::default_random_engine generator(seed);
//        std::uniform_int_distribution<int> distribution(0, 4);
//
//        // Генерируем случайное число
//        randDest = distribution(generator);
//        return;
//    }
//
//    if (randDest == 0) // влево
//    {
//        sprite.setTextureRect(sf::IntRect(160 + (16 * animation), type * 16, 16, 16));
//        dx -= distance;
//        if (sprite.getPosition().x < 0) // проверка на выезд за предел экрана
//        {
//            sprite.setPosition(0, sprite.getPosition().y);
//            dx = 0;
//            isCollidingWithMap = true;
//        }
//        sprite.setPosition(dx, dy);
//        tankDestination = LEFT;
//    } else if (randDest == 1) // вправо
//    {
//        sprite.setTextureRect(sf::IntRect(224 + (16 * animation), type * 16, 16, 16));
//        dx += distance;
//        if (sprite.getPosition().x > 192 * FACTOR) // проверка на выезд за предел экрана
//        {
//            sprite.setPosition(192 * FACTOR, sprite.getPosition().y);
//            dx = 192 * FACTOR;
//            isCollidingWithMap = true;
//        }
//        sprite.setPosition(dx, dy);
//        tankDestination = RIGHT;
//    } else if (randDest == 2) // вниз
//    {
//        sprite.setTextureRect(sf::IntRect(192 + (16 * animation), type * 16, 16, 16));
//        dy += distance;
//        if (sprite.getPosition().y > 192 * FACTOR) // проверка на выезд за предел экрана
//        {
//            sprite.setPosition(sprite.getPosition().x, 192 * FACTOR);
//            dy = 192 * FACTOR;
//            isCollidingWithMap = true;
//        }
//        sprite.setPosition(dx, dy);
//        tankDestination = DOWN;
//    } else if (randDest == 3) // вверх
//    {
//        sprite.setTextureRect(
//                sf::IntRect(128 + (16 * animation), type * 16, 16, 16)); // по строкам в зависимости от звезд
//        dy -= distance;
//        if (sprite.getPosition().y < 0) // проверка на выезд за предел экрана
//        {
//            sprite.setPosition(sprite.getPosition().x, 0);
//            dy = 0;
//            isCollidingWithMap = true;
//        }
//        sprite.setPosition(dx, dy);
//        tankDestination = UP;
//    }
//    animation++;
//    //}
}

void EnemyTank::decrementHealth() {
    health--;
    if (health == 0)
        isDestroyed = true;
}

void EnemyTank::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}