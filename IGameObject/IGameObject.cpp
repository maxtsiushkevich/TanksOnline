#include "IGameObject.h"

sf::Texture IGameObject::texture;

IGameObject :: IGameObject(float x, float y) : dx(x), dy(y) {
    sprite.setTexture((IGameObject::texture));
}

sf::Sprite &IGameObject::getSprite() {
    return sprite;
}