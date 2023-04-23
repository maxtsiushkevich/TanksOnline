#include "Tank.h"
#include "../Bullet/Bullet.h"

Tank :: Tank(float x, float y, float speed, int health, std::vector<IGameObject*>& allBullets) : IGameObject(x, y), health(health), allBullets(allBullets)
{
    animation = 0;
    tankDestination = UP;
    this->speed = speed * FACTOR;
    canShoot = true;
    bullet = nullptr;
}

Tank :: ~Tank()
{
    if (!bullet)
        delete bullet;
}

PlayerTank :: PlayerTank(float x, float y, std::vector<IGameObject*>& allBullets) : Tank(x, y, 45.f, 3, allBullets), stars(0)
{
    sprite.setTextureRect(sf::IntRect(0, 16 * stars, 16, 16));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

void PlayerTank :: update(float time)
{

    float distance = speed * time;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canShoot)
       this->shoot();

    this->move(distance);

    if (bullet != nullptr)
    {
        bullet->update(time);
        if (bullet->getIsDestroyed())
        {
            canShoot = true;
            delete bullet;
            bullet = nullptr;
        }
    }
}

void PlayerTank :: shoot()
{
    switch (stars)
    {
        case 0:
            bullet = new PlayerBullet(dx, dy, tankDestination);
            break;
        case 1:
            bullet = new PlayerFastBullet(dx, dy, tankDestination);
            break;
        case 2:
            // очередь
        case 3:
            bullet = new PlayerPowerfulBullet(dx, dy, tankDestination);
            break;
    }
    allBullets.emplace_back(bullet);
    canShoot = false;
}


void PlayerTank :: handleCollision(IVisitor *visitor)
{
    visitor->visit(*this);
}

void PlayerTank :: move(float distance)
{
    if (animation == 2) // обработка движения
        animation = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        sprite.move(0, -distance);
        sprite.setTextureRect(sf::IntRect(0 + (16 * animation), stars * 16, 16, 16)); // по строкам в зависимости от звезд
        dy -= distance;
        if(sprite.getPosition().y < 0) // проверка на выезд за предел экрана
        {
            sprite.setPosition(sprite.getPosition().x, 0);
            dy = 0;
        }
        tankDestination = UP;
    }

    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        sprite.move(0, distance);
        sprite.setTextureRect(sf::IntRect(64+(16 * animation), stars * 16, 16, 16));
        dy += distance;
        if(sprite.getPosition().y > 192 * FACTOR) // проверка на выезд за предел экрана
        {
            sprite.setPosition(sprite.getPosition().x, 192 * FACTOR);
            dy = 192 * FACTOR;
        }
        tankDestination = DOWN;
    }

    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        sprite.move(-distance, 0);
        sprite.setTextureRect(sf::IntRect(32+(16 * animation), stars * 16, 16, 16));
        dx -= distance;
        if(sprite.getPosition().x < 0) // проверка на выезд за предел экрана
        {
            sprite.setPosition(0, sprite.getPosition().y);
            dx = 0;
        }
        tankDestination = LEFT;
    }

    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        sprite.move(distance, 0);
        sprite.setTextureRect(sf::IntRect(96+(16 * animation), stars * 16, 16, 16));
        dx += distance;
        if(sprite.getPosition().x > 192 * FACTOR) // проверка на выезд за предел экрана
        {
            sprite.setPosition(192 * FACTOR, sprite.getPosition().y);
            dx = 192 * FACTOR;
        }
        tankDestination = RIGHT;
    }
    animation++;
}

void PlayerTank :: addStar()
{
    stars = (stars == 3) ? stars : stars++;
}

void Tank :: render(sf::RenderWindow &window)
{
    window.draw(sprite);
    if (bullet != nullptr)
        bullet->render(window);
}