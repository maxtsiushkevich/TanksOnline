#include "../headers/Bonus.h"

Bonus::Bonus(float x, float y, int type) : IGameObject(x, y) {
    isPicked = false;
    this->type = static_cast<BonusType>(type);
    sprite.setTextureRect(sf::IntRect(256 + (16 * type), 112, 16, 16));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}

bool Bonus::getIsPicked() const {
    return isPicked;
}

void Bonus::setIsPicked() {
    isPicked = true;
}

void update(float time) {
}

void Bonus::render(sf::RenderWindow &window) {
    window.draw(sprite);
}

void Bonus::handleCollision(IVisitor *visitor) {
    visitor->visit(*this);
}

BonusType Bonus::getBonusType() {
    return type;
}

void Bonus::update(float time) {}