#include "Bullet.h"

Bullet :: Bullet(float x, float y, float speed, Destination dest) : IGameObject(x, y), bulletDestination(dest)
{
    this->speed = speed * FACTOR;

    switch (bulletDestination)
    {
        case UP:
            dx = x + 4.f * FACTOR;
            dy = y - 8.f * FACTOR;
            sprite.setTextureRect(sf::IntRect(321, 100, 8, 8));
            break;
        case DOWN:
            dx = x+ 4.f * FACTOR;
            dy = y+ 16.f * FACTOR;
            sprite.setTextureRect(sf::IntRect(337, 100, 8, 8));
            break;
        case LEFT:
            dx = x- 8.f * FACTOR;
            dy = y+ 4.f * FACTOR;
            sprite.setTextureRect(sf::IntRect(329, 100, 8, 8));
            break;
        case RIGHT:
            dx = x+ 16.f * FACTOR;
            dy = y+ 4.f * FACTOR;
            sprite.setTextureRect(sf::IntRect(345, 100, 8, 8));
            break;
    }
    sprite.setPosition(dx, dy);
    sprite.setScale(FACTOR, FACTOR);

    isDestroyed = false;
}

bool Bullet :: checkBounds()
{
    if (dx < -1.f * FACTOR * 16 / 2 || dx > 208.f * FACTOR || dy < -1.f * FACTOR * 16 / 2 || dy > 208.f * FACTOR) // -1*FACTOR*16/2
    {
        isDestroyed = true;
        return true;
    }
    return false;

}

void Bullet :: update(float time)
{
    float distance = speed * time;

    if (checkBounds())
        return;

    switch (bulletDestination)
    {
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

bool Bullet :: getIsDestroyed() const
{
    return isDestroyed;
}

void Bullet :: render(sf::RenderWindow &window)
{
    window.draw(sprite);
}