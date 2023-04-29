#include "MapObject.h"

MapObject :: MapObject(double x, double y) : IGameObject(x, y)
{
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

Brick::Brick(double x, double y) : MapObject(x, y) {
    sprite.setTextureRect(sf::IntRect(256, 64, 8, 8));
}

Metal::Metal(double x, double y) : MapObject(x, y) {
    //isBulletPowerful = false;
    sprite.setTextureRect(sf::IntRect(256, 72, 8, 8));
}

Water::Water(double x, double y) : MapObject(x, y) {
    sprite.setTextureRect(sf::IntRect(256, 80, 8, 8));
}

Grass::Grass(double x, double y) : MapObject(x, y) {
    sprite.setTextureRect(sf::IntRect(264, 72, 8, 8));
}

void Brick::render(sf::RenderWindow &window) {
    window.draw(sprite);
}

void Metal::render(sf::RenderWindow &window) {
    window.draw(sprite);
}

void Water::render(sf::RenderWindow &window) {
    window.draw(sprite);
}

void Grass::render(sf::RenderWindow &window) {
    window.draw(sprite);
}


void Brick::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

void Metal::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

void Water::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

void Grass::handleCollision(IVisitor *visitor) {
      visitor->visit(*this);
}
