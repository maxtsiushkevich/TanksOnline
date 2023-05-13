#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "../IGameObject/IGameObject.h"

class Map {
public:
    Map() = default;
    virtual ~Map() = 0;
    static void loadMap(std::vector<std::shared_ptr<IGameObject>> &map, int levelNum);
    static void update(std::vector<std::shared_ptr<IGameObject>> &map);
    static void render(std::vector<std::shared_ptr<IGameObject>> &map, sf::RenderWindow &window);
};

/*void GameEngine::handleCollisions() {
    std::vector<std::shared_ptr<IGameObject>> allObjects;
    allObjects.insert(allObjects.end(), enemyTanks.begin(), enemyTanks.end());
    allObjects.insert(allObjects.end(), allBullets.begin(), allBullets.end());

    for (const auto &obj: allObjects) // идем по объединенному вектору танков врагов и всех пуль
    {
        if (obj->getSprite().getGlobalBounds().intersects(
                playerTank->getSprite().getGlobalBounds())) // если что-то пересекается с танком игрока
        {
            if (dynamic_cast<Tank *>(obj.get())) // если это танк врага
            {
                auto visitor = std::make_shared<CollisionWithTankVisitor>();
                playerTank->handleCollision(visitor.get()); // обработка столкновения для игрока
                obj->handleCollision(visitor.get()); // для вражеского танка
                continue;
            }
            if (dynamic_cast<PlayerBullet *>(obj.get()) || dynamic_cast<PlayerFastBullet *>(obj.get()) ||
                dynamic_cast<PlayerPowerfulBullet *>(obj.get())) // если это пуля союзников то танку игрока ничего не будет
            {
                auto visitor = std::make_shared<CollisionWithTankVisitor>();
                obj->handleCollision(visitor.get());
                continue;
            } else // иначе это пуля врага
            {
                auto visitor1 = std::make_shared<CollisionWithTankVisitor>();
                auto visitor2 = std::make_shared<CollisionWithBulletVisitor>();
                obj->handleCollision(visitor1.get());
                playerTank->handleCollision(visitor2.get());
                continue;
            }
        }

        for (const auto &bullet: allBullets) // проверяем пересечение объектов общего вектора и вектора пуль
        {
            if (obj->getSprite().getGlobalBounds().intersects(bullet->getSprite().getGlobalBounds()) && obj != bullet)
            {
                if (dynamic_cast<Bullet *>(obj.get()) && !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed()) &&
                    !(dynamic_cast<Bullet *>(bullet.get())->getIsDestroyed())) // если пуля с пулей, проверяем чтобы ни одна не была уже уничтожена
                {
                    auto visitor = std::make_shared<CollisionWithBulletVisitor>();
                    obj->handleCollision(visitor.get());
                    bullet->handleCollision(visitor.get());
                    break;
                } else if (dynamic_cast<Tank *>(obj.get()) && !(dynamic_cast<Tank *>(obj.get())->getIsDestroyed()) &&
                           !(dynamic_cast<Bullet *>(bullet.get())->getIsDestroyed())) {
                    auto visitor1 = std::make_shared<CollisionWithBulletVisitor>();
                    obj->handleCollision(visitor1.get());
                    auto visitor2 = std::make_shared<CollisionWithTankVisitor>();
                    bullet->handleCollision(visitor2.get());
                    break;
                }
            }
        }

        for (const auto &tank: enemyTanks)
        {
            if (obj->getSprite().getGlobalBounds().intersects(tank->getSprite().getGlobalBounds()) && obj != tank)
            {
                if (dynamic_cast<Bullet *>(obj.get()) && !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed()) &&
                    !(dynamic_cast<Tank *>(tank.get())->getIsDestroyed()))
                {
                    auto visitor1 = std::make_shared<CollisionWithBulletVisitor>();
                    tank->handleCollision(visitor1.get());
                    auto visitor2 = std::make_shared<CollisionWithTankVisitor>();
                    obj->handleCollision(visitor2.get());
                    break;
                } else if (dynamic_cast<Tank *>(obj.get()) && !(dynamic_cast<Tank *>(obj.get())->getIsDestroyed()) &&
                           !(dynamic_cast<Tank *>(tank.get())->getIsDestroyed()))
                {
                    auto visitor = std::make_shared<CollisionWithTankVisitor>();
                    tank->handleCollision(visitor.get());
                    obj->handleCollision(visitor.get());
                }
            }
        }
    }

    /// дописать коллизии с картой
} */

#endif
