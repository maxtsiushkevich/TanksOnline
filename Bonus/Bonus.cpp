#include "Bonus.h"

Bonus :: Bonus(float x, float y, int type) : IGameObject(x, y)
{
    isPicked = false;
    this->type = static_cast<BonusType>(type);
    sprite.setTextureRect(sf::IntRect(256+(16*type), 112, 16, 16));
    sprite.setPosition(x, y);
    sprite.setScale(FACTOR, FACTOR);
}