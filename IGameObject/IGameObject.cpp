#include "IGameObject.h"

sf::Texture IGameObject::texture;

IGameObject :: IGameObject(double x, double y) : dx(x), dy(y) {
    sprite.setTexture((IGameObject::texture));
    isDestroyed = false;
}

sf::Sprite &IGameObject::getSprite() {
    return sprite;
}

bool IGameObject ::  getIsDestroyed()
{
    return isDestroyed;
}
void IGameObject :: setIsDestroyed()
{
    isDestroyed = true;
}