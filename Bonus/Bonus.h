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
    bool getIsPicked() const;
    void setIsPicked();
    void update(float time) override;
    void render(sf::RenderWindow &window) override;
    void handleCollision(IVisitor *visitor) override;
    BonusType getBonusType();
};


#endif //GAME_BONUS_H
