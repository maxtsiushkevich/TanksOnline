#include "../headers/Tank.h"
#include "../headers/Bullet.h"
#include <cmath>

#define PLAYER_SPAWN_X 64
#define PLAYER_SPAWN_Y 192

#define ALLY_SPAWN_X 128
#define ALLY_SPAWN_Y 192


PlayerTank::PlayerTank(float x, float y, std::vector<std::shared_ptr<IGameObject>> &allBullets, bool isAllyTank) : Tank(x, y,
                                                                                                       TANK_SPEED, 3,
                                                                                                       allBullets),
                                                                                                  stars(0) {
    this->isAllyTank = isAllyTank;
    previousButton = UP;
    invulnerableTime.restart();
    isInvulnerable = true;
    isShootButtonPressed = false;
    sprite.setTextureRect(sf::IntRect(0, 16 * stars, 16, 16));

    if (isAllyTank)
        sprite.setColor(sf::Color(0, 255, 0, 230));

    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

void PlayerTank::reset() {
    isInvulnerable = true;
    invulnerableTime.restart();
    health--;
    if (health == 0)
        isDestroyed = true;
    if (isAllyTank)
    {
        dx = ALLY_SPAWN_X * FACTOR;
        dy = ALLY_SPAWN_Y * FACTOR;
    }
    else {
        dx = PLAYER_SPAWN_X * FACTOR;
        dy = PLAYER_SPAWN_Y * FACTOR;
    }
    stars = 0;
    sprite.setTextureRect(sf::IntRect(0, 16 * stars, 16, 16));
    sprite.setPosition(dx, dy);
    sprite.setScale(FACTOR, FACTOR);
    animation = 0;
    tankDestination = UP;
    canShoot = true;
    isShootButtonPressed = false;
}

void PlayerTank::render(sf::RenderWindow &window) {
    window.draw(sprite);

    if (isInvulnerable)
    {
        if (animation == 2)
            animation = 0;
        sf::Sprite effect;
        effect.setTexture(texture);
        effect.setTextureRect(sf::IntRect(256 + (16 * animation), 144, 16, 16));
        effect.setPosition(dx, dy);
        effect.setScale(FACTOR, FACTOR);
        window.draw(effect);
    }
}

void PlayerTank::update(float time)
{
    if (invulnerableTime.getElapsedTime().asSeconds() > INVULNERABLE_TIME)
        isInvulnerable = false;

    this->time = time;
    float distance = speed * time;
    if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isAllyTank) || (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) && isAllyTank)) && canShoot && !isShootButtonPressed) // добавить выстрел на разные кнопки
    {
        this->shoot();
        isShootButtonPressed = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isAllyTank) || (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) && isAllyTank))
        isShootButtonPressed = true;
    else
        isShootButtonPressed = false;

    this->move(distance);
}

void PlayerTank::shoot()
{
    std::shared_ptr<IGameObject> bullet;
    switch (stars) {
        case 0:
            bullet = std::make_shared<PlayerBullet>(dx, dy, tankDestination, shared_from_this());
            canShoot = false;
            break;
        case 1:
            bullet = std::make_shared<PlayerFastBullet>(dx, dy, tankDestination, shared_from_this());
            canShoot = false;
            break;
        case 2:
            //bullet = std::make_shared<PlayerBullet>(dx, dy, tankDestination, shared_from_this()); сделать максимум 2 пули одновременно

            break;
            // очередь
        case 3:
            bullet = std::make_shared<PlayerPowerfulBullet>(dx, dy, tankDestination, shared_from_this());
            canShoot = false;
            break;
    }

    if (bullet)
        allBullets.emplace_back(std::move(bullet));
}


void PlayerTank::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

void PlayerTank::move(float distance) {
    if (animation == 2) // обработка движения
        animation = 0;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !isAllyTank) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && isAllyTank)) {
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
        previousButton = LEFT;
        tankDestination = LEFT;
    } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !isAllyTank) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && isAllyTank))
    {
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
        previousButton = RIGHT;
        tankDestination = RIGHT;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !isAllyTank) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && isAllyTank))
    {
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
        previousButton = DOWN;
        tankDestination = DOWN;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isAllyTank) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && isAllyTank)) {
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
        previousButton = UP;
        tankDestination = UP;
    }

    //if (tankDestination == LEFT)
    //    dx -= distance;
    //else if (tankDestination == RIGHT)
    //    dx += distance;
    //else if (tankDestination == DOWN)
    //    dy += distance;
    //else if (tankDestination == UP)
    //    dy -= distance;

    sprite.setPosition(dx, dy);
    animation++;
}

void PlayerTank::addStar() {
    if (stars != 3)
        stars++;
}

void PlayerTank::setIsInvulnerable() {
    isInvulnerable = true;
    invulnerableTime.restart();
}

bool PlayerTank::getIsInvulnerable() const { return isInvulnerable; }

void PlayerTank::addHealth() { health++; }
