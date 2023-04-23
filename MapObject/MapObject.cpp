#include "MapObject.h"

Brick :: Brick(float x, float y) : MapObject(x, y)
{
    sprite.setTextureRect(sf::IntRect(256 , 64, 8, 8));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

Metal :: Metal(float x, float y) : MapObject(x, y)
{
    sprite.setTextureRect(sf::IntRect(256, 72, 8, 8));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

Water :: Water(float x, float y) : MapObject(x, y)
{
    sprite.setTextureRect(sf::IntRect(256, 80, 8, 8));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

Grass :: Grass(float x, float y) : MapObject(x, y)
{
    sprite.setTextureRect(sf::IntRect(264, 72, 8, 8));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

void Brick :: render(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Metal :: render(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Water :: render(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Grass :: render(sf::RenderWindow &window)
{
    window.draw(sprite);
}


void Brick :: checkCollision(IVisitor *visitor)
{
    //visitor->visit(*this);
}
void Metal :: checkCollision(IVisitor *visitor)
{
    //visitor->visit(*this);
}
void Water :: checkCollision(IVisitor *visitor)
{
    //visitor->visit(*this);
}
void Grass :: checkCollision(IVisitor *visitor)
{
    //visitor->visit(*this);
}
