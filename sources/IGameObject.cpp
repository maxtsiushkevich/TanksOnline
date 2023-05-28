#include "../headers/IGameObject.h"

sf::Texture IGameObject::texture;

IGameObject::IGameObject(float x, float y) : dx(x), dy(y) {
    sprite.setTexture((IGameObject::texture));
    isDestroyed = false;
}

sf::Sprite &IGameObject::getSprite() {
    return sprite;
}

bool IGameObject::getIsDestroyed() {
    return isDestroyed;
}

void IGameObject::setIsDestroyed() {
    isDestroyed = true;
}