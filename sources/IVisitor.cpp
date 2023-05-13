#include "../headers/IVisitor.h"
#include <iostream>
#include "../headers/Tank.h"
#include "../headers/Bullet.h"
#include "../headers/MapObject.h"
#include "../headers/Bonus.h"

void CollisionWithTankVisitor::visit(PlayerTank &tank)
{
    float distance = tank.getTime() * tank.getSpeed();
    tank.decrementAnimation();
    tank.move(-distance);
}

void CollisionWithTankVisitor :: visit(EnemyTank &tank)
{
    float distance = tank.getTime() * tank.getSpeed();
    tank.decrementAnimation();
    tank.move(-distance);
}

void CollisionWithTankVisitor::visit(PlayerBullet &bullet) // столкновение пули с танком
{
    bullet.setIsDestroyed();
}

void CollisionWithTankVisitor::visit(PlayerFastBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithTankVisitor::visit(PlayerPowerfulBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithTankVisitor::visit(EnemyBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithTankVisitor::visit(EnemyFastBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithTankVisitor :: visit(Bonus &bonus)
{
    bonus.setIsPicked();
}

void CollisionWithBulletVisitor::visit(PlayerTank &tank) // столкновение танка с пулей, вражеские помечаются как удаленные
{
    if (!tank.getIsInvulnerable())
        tank.reset();
}

void CollisionWithBulletVisitor::visit(EnemyTank &tank)
{
    tank.decrementHealth();
}

void CollisionWithBulletVisitor::visit(PlayerBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithBulletVisitor::visit(PlayerFastBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithBulletVisitor::visit(PlayerPowerfulBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithBulletVisitor::visit(EnemyBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithBulletVisitor::visit(EnemyFastBullet &bullet) {
    bullet.setIsDestroyed();
}

void CollisionWithBulletVisitor::visit(Brick &object) // вызов зависит от того, c какой пулей столкнулось
{
    object.setIsDestroyed();
}

void CollisionWithBulletVisitor::visit(Metal &object)
{
    object.setIsDestroyed();
}

void CollisionWithBulletVisitor::visit(Water &object)
{
}

void CollisionWithBulletVisitor::visit(Grass &object)
{
}

void CollisionWithBulletVisitor::visit(Eagle &object)
{
    object.setIsFallen();
}

void CollisionWithMapObjectVisitor::visit(PlayerTank &tank)
{
    float distance = tank.getTime() * tank.getSpeed();
    tank.decrementAnimation();
    tank.move(-distance);
}

void CollisionWithMapObjectVisitor::visit(EnemyTank &tank) // надо отодвинуть танк
{
    float distance = tank.getTime() * tank.getSpeed();
    tank.decrementAnimation();
    tank.move(-distance);
}

void CollisionWithMapObjectVisitor::visit(PlayerBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithMapObjectVisitor::visit(PlayerFastBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithMapObjectVisitor::visit(PlayerPowerfulBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithMapObjectVisitor::visit(EnemyBullet &bullet)
{
    bullet.setIsDestroyed();
}

void CollisionWithMapObjectVisitor::visit(EnemyFastBullet &bullet)
{
    bullet.setIsDestroyed();
}