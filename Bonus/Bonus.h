#ifndef GAME_BONUS_H
#define GAME_BONUS_H

#include "../IGameObject/IGameObject.h"

extern float FACTOR;

#include <iostream>

enum BonusType
{
    HELMET,
    CLOCK,
    SHOVEL,
    STAR,
    GRENADE,
    TANK
};

class Bonus : public IGameObject
{
protected:
    BonusType type;
    bool isPicked;
public:
    Bonus(float x, float y, int type);
    bool getIsPicked() const { return isPicked; }
    void setIsPicked() { isPicked = true; }
    void update(float time) override { }
    void render(sf::RenderWindow &window) override { window.draw(sprite); };
    void handleCollision(IVisitor *visitor) override {  visitor->visit(*this); };
    BonusType getBonusType() { return type; }
};


#endif //GAME_BONUS_H
