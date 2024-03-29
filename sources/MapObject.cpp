#include "../headers/MapObject.h"
#include <iostream>

MapObject::MapObject(float x, float y) : IGameObject(x, y) {
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

Brick::Brick(float x, float y) : MapObject(x, y) {
    sprite.setTextureRect(sf::IntRect(256, 64, 8, 8));
}

Metal::Metal(float x, float y) : MapObject(x, y) {
    sprite.setTextureRect(sf::IntRect(256, 72, 8, 8));
}

Water::Water(float x, float y) : MapObject(x, y) {
    sprite.setTextureRect(sf::IntRect(256, 80, 8, 8));
}

Grass::Grass(float x, float y) : MapObject(x, y) {
    sprite.setTextureRect(sf::IntRect(264, 72, 8, 8));
}

Eagle::Eagle(float x, float y) : MapObject(x, y) {
    isFallen = false;
    isInvulnerable = false;
    invulnerableTime.restart();
    sprite.setTextureRect(sf::IntRect(304, 32, 16, 16));
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

void Eagle::render(sf::RenderWindow &window) {
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

void Eagle::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

void Eagle::setIsFallen() {
    isFallen = true;
    sprite.setTextureRect(sf::IntRect(320, 32, 16, 16));
}

void Brick::update(float time) {}

void Metal::update(float time) {}

void Water::update(float time) {}

void Grass::update(float time) {}

void Eagle::update(float time) {
    if (invulnerableTime.getElapsedTime().asSeconds() > INVULNERABLE_TIME_FOR_BASE)
        isInvulnerable = false;
}

void Eagle::setIsInvulnerable() {
    isInvulnerable = true;
    invulnerableTime.restart();
}

bool Eagle::getIsInvulnerable() const {
    return isInvulnerable;
}