#ifndef GAME_IVISITOR_H
#define GAME_IVISITOR_H

class PlayerTank;
class PlayerBullet;
class PlayerFastBullet;
class PlayerPowerfulBullet;
class EnemyBullet;
class EnemySpeedBullet;

class Brick;
class Metal;
class Water;
class Grass;

class IVisitor
{
public:
    virtual void visit(PlayerTank &tank) = 0;

    virtual void visit(PlayerBullet &bullet) = 0;
    virtual void visit(PlayerFastBullet &bullet) = 0;
    virtual void visit(PlayerPowerfulBullet &bullet) = 0;
    virtual void visit(EnemyBullet &bullet) = 0;
    virtual void visit(EnemySpeedBullet &bullet) = 0;

//    virtual void visit(Brick &bullet) = 0;
//    virtual void visit(Metal &bullet) = 0;
//    virtual void visit(Water &bullet) = 0;
//    virtual void visit(Grass &bullet) = 0;

    virtual ~IVisitor() = default;
};

class TankCollisionVisitor : public IVisitor // обрабатывает столкновение танка с чем-то, изменяет поведение танка
{
public:
    void visit(PlayerTank &tank) override; // танк с танком

    void visit(PlayerBullet &bullet) override; // танк с пулей союзника (ничего не будет, пуля исчезнет)
    void visit(PlayerFastBullet &bullet) override; // ничего не будет
    void visit(PlayerPowerfulBullet &bullet) override; // ничего не будет
    void visit(EnemyBullet &bullet) override; // минус жизнь, респаун
    void visit(EnemySpeedBullet &bullet) override;  // минус жизнь

//    void visit(Brick &bullet) override;
//    void visit(Metal &bullet) override;
//    void visit(Water &bullet) override;
//    void visit(Grass &bullet) override;
};

//class BulletCollisionVisitor : public IVisitor // обрабатывает столкновение пули с чем-то
//{
//public:
//    void visit(PlayerTank &tank) override;

//    void visit(PlayerBullet &bullet) override; // пули исчезают
//    void visit(PlayerFastBullet &bullet) override;
//    void visit(PlayerPowerfulBullet &bullet) override;
//    void visit(EnemyBullet &bullet) override;
//    void visit(EnemySpeedBullet &bullet) override;

//    void visit(Brick &bullet) override;
//    void visit(Metal &bullet) override;
//    void visit(Water &bullet) override;
//    void visit(Grass &bullet) override;
//};


#endif
