#include "Tank.h"
#include "../Bullet/Bullet.h"

#include <random>
#include <chrono>

EnemyTank::EnemyTank(float x, float y, std::vector<std::shared_ptr<IGameObject>> &allBullets, int type, bool isBonusTank) : Tank(x, y,
                                                                                                                                 ENEMY_TANK_SPEED,
                                                                                                                                 1,
                                                                                                                                 allBullets)
{
    this->isBonusTank = isBonusTank;
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

void EnemyTank::update(float time) {
    this->time = time;
    float distance = speed * time;
    if (canShoot && (delayBeforeShoot.getElapsedTime().asSeconds() > DELAY_BEFORE_SHOOT))
    {
        this->shoot();
    }

    this->move(distance);
}

void EnemyTank::shoot()
{
    if (type != EnemyShootingTank)
        bullet = std::make_shared<EnemyBullet>(dx, dy, tankDestination, shared_from_this());
    else bullet = std::make_shared<EnemyFastBullet>(dx, dy, tankDestination, shared_from_this());

    if (bullet) {
        allBullets.emplace_back(std::move(bullet));
        canShoot = false;
    }
}

void EnemyTank::move(float distance)
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

void EnemyTank :: handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

void EnemyTank ::enableShooting()
{
    delayBeforeShoot.restart();
    canShoot = true;
}