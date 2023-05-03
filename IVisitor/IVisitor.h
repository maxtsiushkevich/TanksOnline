#ifndef GAME_IVISITOR_H
#define GAME_IVISITOR_H

class PlayerTank;
class EnemyTank;

class PlayerBullet;
class PlayerFastBullet;
class PlayerPowerfulBullet;
class EnemyBullet;
class EnemyFastBullet;

class Brick;
class Metal;
class Water;
class Grass;
class Eagle;

class Bonus;

class IVisitor {
public:
    virtual void visit(PlayerTank &tank) = 0;
    virtual void visit(EnemyTank &tank) = 0;

    virtual void visit(PlayerBullet &bullet) = 0;
    virtual void visit(PlayerFastBullet &bullet) = 0;
    virtual void visit(PlayerPowerfulBullet &bullet) = 0;
    virtual void visit(EnemyBullet &bullet) = 0;
    virtual void visit(EnemyFastBullet &bullet) = 0;

    virtual void visit(Brick &object) = 0;
    virtual void visit(Metal &object) = 0;
    virtual void visit(Water &object) = 0;
    virtual void visit(Grass &object) = 0;
    virtual void visit(Eagle &object) = 0;

    virtual void visit(Bonus &bonus) = 0;

    virtual ~IVisitor() = default;
};

class CollisionWithTankVisitor : public IVisitor {
public:
    void visit(PlayerTank &tank) override;
    void visit(EnemyTank &tank) override;

    void visit(PlayerBullet &bullet) override;
    void visit(PlayerFastBullet &bullet) override;
    void visit(PlayerPowerfulBullet &bullet) override;
    void visit(EnemyBullet &bullet) override;
    void visit(EnemyFastBullet &bullet) override;

    void visit(Brick &object) override {};
    void visit(Metal &object) override {};
    void visit(Water &object) override {};
    void visit(Grass &object) override {};
    void visit(Eagle &object) override {};

    void visit(Bonus &bonus) override;
};

class CollisionWithBulletVisitor : public IVisitor {
    void visit(PlayerTank &tank) override;
    void visit(EnemyTank &tank) override;

    void visit(PlayerBullet &bullet) override;
    void visit(PlayerFastBullet &bullet) override;
    void visit(PlayerPowerfulBullet &bullet) override;
    void visit(EnemyBullet &bullet) override;
    void visit(EnemyFastBullet &bullet) override;

    void visit(Brick &object) override;
    void visit(Metal &object) override;
    void visit(Water &object) override;
    void visit(Grass &object) override;
    void visit(Eagle &object) override;

    void visit(Bonus &object) override {};

};

class CollisionWithMapObjectVisitor : public IVisitor
{
    void visit(PlayerTank &tank) override;
    void visit(EnemyTank &tank) override;

    void visit(PlayerBullet &bullet) override;
    void visit(PlayerFastBullet &bullet) override;
    void visit(PlayerPowerfulBullet &bullet) override;
    void visit(EnemyBullet &bullet) override;
    void visit(EnemyFastBullet &bullet) override;

    void visit(Brick &object) override {};
    void visit(Metal &object) override {};
    void visit(Water &object) override {};
    void visit(Grass &object) override {};
    void visit(Eagle &object) override {};

    void visit(Bonus &bonus) override {};
};

#endif
