#include "../headers/Tank.h"
#include "../headers/Bullet.h"

#include <random>
#include <chrono>

Tank::Tank(float x, float y, float speed, int health, std::vector<std::shared_ptr<IGameObject>> &allBullets)
        : IGameObject(x, y), health(health),
          allBullets(allBullets) {
    time = 0;
    animation = 0;
    tankDestination = UP;
    this->speed = speed * FACTOR;
    canShoot = true;
}
void Tank::enableShooting() {
    canShoot = true;
}
void Tank::render(sf::RenderWindow &window) {
    window.draw(sprite);
}
float Tank::getTime() const {
    return time;
}
float Tank::getSpeed() const {
    return speed;
}
int Tank::getHealth() const {
    return health;
}
void Tank::decrementAnimation() { animation--; }
