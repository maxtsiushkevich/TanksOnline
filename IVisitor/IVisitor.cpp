#include "IVisitor.h"
#include <iostream>
void TankCollisionVisitor::visit(PlayerTank &tank)
{ }

void TankCollisionVisitor::visit(PlayerBullet &bullet)
{
    std::cout << "Танк столкунлся с "
}
void TankCollisionVisitor::visit(PlayerFastBullet &bullet)
{ }
void TankCollisionVisitor::visit(PlayerPowerfulBullet &bullet)
{ }
void TankCollisionVisitor::visit(EnemyBullet &bullet)
{ }
void TankCollisionVisitor::visit(EnemySpeedBullet &bullet)
{ }


//void BulletCollisionVisitor :: visit(PlayerTank &tank)
//{ }
//
//void BulletCollisionVisitor::visit(PlayerBullet &bullet)
//{ }
//void BulletCollisionVisitor::visit(PlayerFastBullet &bullet)
//{ }
//void BulletCollisionVisitor::visit(PlayerPowerfulBullet &bullet)
//{ }
//void BulletCollisionVisitor::visit(EnemyBullet &bullet)
//{ }
//void BulletCollisionVisitor::visit(EnemySpeedBullet &bullet)
//{ }

//void TankCollisionVisitor::visit(Brick &bullet)
//{}
//void TankCollisionVisitor::visit(Metal &bullet)
//{}
//void TankCollisionVisitor::visit(Water &bullet)
//{}
//void TankCollisionVisitor::visit(Grass &bullet)
//{}
//
//void BulletCollisionVisitor::visit(Brick &bullet)
//{}
//void BulletCollisionVisitor::visit(Metal &bullet)
//{}
//void BulletCollisionVisitor::visit(Water &bullet)
//{}
//void BulletCollisionVisitor::visit(Grass &bullet)
//{}