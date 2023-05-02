#include "Tank.h"
#include "../Bullet/Bullet.h"

PlayerTank::PlayerTank(float x, float y, std::vector<std::shared_ptr<IGameObject>> &allBullets) : Tank(x, y,
                                                                                                       TANK_SPEED, 3,
                                                                                                       allBullets),
                                                                                                  stars(0)
{
    previousButton = UP;
    invulnerableTime.restart();
    isInvulnerable = true;
    isSpacePressed = false;
    sprite.setTextureRect(sf::IntRect(0, 16 * stars, 16, 16));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

void PlayerTank::reset() {
    isInvulnerable = true;
    invulnerableTime.restart();
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

void PlayerTank :: render(sf::RenderWindow &window)
{
    window.draw(sprite);
    if (isInvulnerable)
    {
        if (animation == 2) // обработка движения
            animation = 0;
        sf::Sprite effect;
        effect.setTexture(texture);
        effect.setTextureRect(sf::IntRect(256 +(16*animation), 144, 16, 16));
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
            bullet = std::make_shared<PlayerBullet>(dx, dy, tankDestination, shared_from_this());
            break;
        case 1:
            bullet = std::make_shared<PlayerFastBullet>(dx, dy, tankDestination, shared_from_this());
            break;
        case 2:
            // очередь
        case 3:
            bullet = std::make_shared<PlayerPowerfulBullet>(dx, dy, tankDestination, shared_from_this());
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

void PlayerTank::move(float distance)
{
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

void PlayerTank::addStar()
{
    if (stars != 3)
        stars++;
}

void PlayerTank ::setIsInvulnerable()
{
    isInvulnerable = true;
    invulnerableTime.restart();
}