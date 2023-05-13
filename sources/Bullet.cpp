#include "../headers/Bullet.h"

#include <iostream>

Bullet::Bullet(float x, float y, float speed, Destination dest, std::shared_ptr<IGameObject> owner) : IGameObject(x, y),
                                                                                                      bulletDestination(
                                                                                                              dest),
                                                                                                      owner(std::move(
                                                                                                              owner)) {
    this->speed = speed * FACTOR;

    switch (bulletDestination) {
        case UP:
            dx = x + 5.f * FACTOR;
            dy = y - 6.f * FACTOR;
            sprite.setTextureRect(sf::IntRect(322, 101, 5, 6));
            break;
        case DOWN:
            dx = x + 5.f * FACTOR; ////
            dy = y + 15.f * FACTOR;
            sprite.setTextureRect(sf::IntRect(338, 101, 5, 6));
            break;
        case LEFT:
            dx = x - 6.f * FACTOR;
            dy = y + 5.f * FACTOR;
            sprite.setTextureRect(sf::IntRect(329, 101, 6, 5));
            break;
        case RIGHT:
            dx = x + 15.f * FACTOR;
            dy = y + 5.f * FACTOR; /////
            sprite.setTextureRect(sf::IntRect(345, 101, 6, 5));
            break;
    }
    sprite.setPosition(dx, dy);
    sprite.setScale(FACTOR, FACTOR);
}

bool Bullet::checkBounds() {
    if (dx < -1.f * FACTOR * 16 / 2 || dx > 208.f * FACTOR || dy < -1.f * FACTOR * 16 / 2 ||
        dy > 208.f * FACTOR) // -1*FACTOR*16/2
    {
        isDestroyed = true;
        return true;
    }
    return false;

}

void Bullet::update(float time) {
    float distance = speed * time;

    if (checkBounds())
        return;

    switch (bulletDestination) {
        case UP:
            sprite.move(0, -distance);
            dy -= distance;
            break;
        case DOWN:
            sprite.move(0, distance);
            dy += distance;
            break;
        case LEFT:
            sprite.move(-distance, 0);
            dx -= distance;
            break;
        case RIGHT:
            sprite.move(distance, 0);
            dx += distance;
            break;
    }
    sprite.setPosition(dx, dy);
}

void Bullet::render(sf::RenderWindow &window) {
    window.draw(sprite);
}

std::shared_ptr<IGameObject> Bullet::getOwner() const {
    return owner;
}

void Bullet::setIsDestroyed() {
    isDestroyed = true;
}

PlayerBullet::PlayerBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner) : Bullet(x, y,
                                                                                                            PLAYER_BULLET_SPEED,
                                                                                                            dest,
                                                                                                            std::move(
                                                                                                                    owner)) {}

void PlayerBullet::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

PlayerFastBullet::PlayerFastBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner) : Bullet(x,
                                                                                                                    y,
                                                                                                                    PLAYER_FAST_BULLET_SPEED,
                                                                                                                    dest,
                                                                                                                    std::move(
                                                                                                                            owner)) {}

void PlayerFastBullet::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

PlayerPowerfulBullet::PlayerPowerfulBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner)
        : Bullet(x, y, PLAYER_BULLET_SPEED, dest, std::move(owner)) {}

void PlayerPowerfulBullet::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}


EnemyBullet::EnemyBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner) : Bullet(x, y,
                                                                                                          ENEMY_BULLET_SPEED,
                                                                                                          dest,
                                                                                                          std::move(
                                                                                                                  owner)) {}

void EnemyBullet::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

EnemyFastBullet::EnemyFastBullet(float x, float y, Destination dest, std::shared_ptr<IGameObject> owner) : Bullet(x, y,
                                                                                                                  ENEMY_FAST_BULLET_SPEED,
                                                                                                                  dest,
                                                                                                                  std::move(
                                                                                                                          owner)) {}

void EnemyFastBullet::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}