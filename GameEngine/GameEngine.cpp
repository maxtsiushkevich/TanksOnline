#include "GameEngine.h"
#include "../Tank/Tank.h"
#include "../Bullet/Bullet.h"
#include "../Map/Map.h"
#include "../MapObject/MapObject.h"

#include <iostream>

#define PLAYER_SPAWN_X 64
#define PLAYER_SPAWN_Y 192

void GameEngine :: init()
{
    mainTexture.loadFromFile("SpriteList.png");

    IGameObject::texture = mainTexture;

    levelNum = 1;
    Map::loadMap(map, levelNum);

    // рандомно заполняем вектор с танками врага

    playerTank = std::make_shared<PlayerTank>(PLAYER_SPAWN_X * FACTOR, PLAYER_SPAWN_Y * FACTOR, allBullets);

    std::shared_ptr<IGameObject> enemy = std::make_shared<EnemyTank>(20 * FACTOR, 20 * FACTOR, allBullets, 0);
    enemyTanks.push_back(enemy);

}

void GameEngine::togglePause() {
    isPaused = !isPaused;
}

void GameEngine::update()
{
    if (!isPaused)
    {

        if (enemyTanks.empty())
        {
            levelNum++;
            if (levelNum == 31)
                levelNum = 1;
            Map::loadMap(map, levelNum);
            // инициализируем вектор врагов
            return;
        }


        sf::Time time = clock.restart();
        double seconds = time.asSeconds();

        playerTank->update(seconds);

        Map::update(map);

        for (auto it = enemyTanks.begin(); it != enemyTanks.end(); ) {
            if ((*it)->getIsDestroyed()) {
                it = enemyTanks.erase(it);
            } else {
                (*it)->update(seconds);
                ++it;
            }
        }

        allBullets.erase(std::remove_if(allBullets.begin(), allBullets.end(),
                                        [seconds](const auto &gameObject)
                                        {
                                            if (auto *bullet = dynamic_cast<Bullet *>(gameObject.get())) {
                                                if (bullet->getIsDestroyed())
                                                {
                                                    if (auto *tank = dynamic_cast<Tank *>(bullet->getOwner()))
                                                        tank->enableShooting();
                                                    return true;
                                                }
                                                else
                                                    bullet->update(seconds);

                                            }
                                            return false;
                                        }), allBullets.end());
    }
    else
        clock.restart();
}

void GameEngine::handleCollisions()
{
    std::vector<std::shared_ptr<IGameObject>> allObjects;
    allObjects.insert(allObjects.end(), enemyTanks.begin(), enemyTanks.end());
    allObjects.insert(allObjects.end(), allBullets.begin(), allBullets.end());

    for (const auto &obj : allObjects) // обработка коллизий танков врага и всех пуль
    {
        if (obj->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds())) // обработка коллизии с танком игрока
        {

            auto visitor = std::make_shared<CollisionWithTankVisitor>();

            if ((dynamic_cast<EnemyBullet*>(obj.get()) || dynamic_cast<EnemyFastBullet*>(obj.get()) && !(dynamic_cast<Bullet*>(obj.get())->getIsDestroyed()))) // если это вражеские пули
            {
                obj->handleCollision(visitor.get());
                auto newVisitor = std::make_shared<CollisionWithBulletVisitor>();
                playerTank->handleCollision(newVisitor.get()); // обработка столкновения игрока с пулей
            }
            else if ((dynamic_cast<PlayerBullet*>(obj.get()) || dynamic_cast<PlayerFastBullet*>(obj.get()) // обработка пуль союзников при столкновении с игроком
            || dynamic_cast<PlayerPowerfulBullet*>(obj.get())) && !(dynamic_cast<Bullet*>(obj.get())->getIsDestroyed()))
                obj->handleCollision(visitor.get());

            if (dynamic_cast<Tank*>(obj.get())) // столкновение танка врага с танком игрока
            {
                obj->handleCollision(visitor.get());
                playerTank->handleCollision(visitor.get());
            }
        }

        for (const auto &bullet : allBullets)
        {
            if (obj->getSprite().getGlobalBounds().intersects(bullet->getSprite().getGlobalBounds()) && obj.get() != bullet.get())
            {
                if (dynamic_cast<Bullet*>(obj.get()) && !(dynamic_cast<Bullet*>(obj.get())->getIsDestroyed())) // пуля с пулей // и пуля не считается уничтоженной
                {
                    auto visitor = std::make_shared<CollisionWithBulletVisitor>();
                    obj->handleCollision(visitor.get());
                    bullet->handleCollision(visitor.get());
                    continue;
                }
                if (dynamic_cast<Tank*>(obj.get())) // пуля с танком врага
                {
                    if ((dynamic_cast<PlayerBullet*>(obj.get()) || dynamic_cast<PlayerFastBullet*>(obj.get()) // обработка пуль союзников при столкновении с игроком
                        || dynamic_cast<PlayerPowerfulBullet*>(obj.get())) && !(dynamic_cast<Bullet*>(obj.get())->getIsDestroyed()))
                    {
                        auto visitor1 = std::make_shared<CollisionWithBulletVisitor>();
                        auto visitor2 = std::make_shared<CollisionWithTankVisitor>();
                        obj->handleCollision(visitor1.get());
                        bullet->handleCollision(visitor2.get());
                        continue;
                    }
                    else
                    {
                        auto visitor = std::make_shared<CollisionWithTankVisitor>();
                        bullet->handleCollision(visitor.get());
                    }
                }

            }
        }

        for (const auto &enemy : enemyTanks)
        {
            if (obj->getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()) && obj.get() != enemy.get() && dynamic_cast<Tank*>(obj.get()))
            {
                auto visitor = std::make_shared<CollisionWithTankVisitor>();
                obj->handleCollision(visitor.get());
                enemy->handleCollision(visitor.get());
            }
        }

    }

    for (const auto &mapObj : map) // обработка столкновений танка игрока с объектами на карте
    {
        if (mapObj->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds()))
        {
            if (dynamic_cast<Brick*>(mapObj.get()) ||
            dynamic_cast<Metal *>(mapObj.get()) ||
            dynamic_cast<Water*>(mapObj.get())) // через траву можно ездить
            {
                auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                playerTank->handleCollision(visitor.get());
            }
        }

        for (const auto &bullet : allBullets)
        {
            if (mapObj->getSprite().getGlobalBounds().intersects(bullet->getSprite().getGlobalBounds()))
            {
                if (dynamic_cast<Grass*>(mapObj.get()) ||  dynamic_cast<Water*>(mapObj.get()))
                    continue;
                if (dynamic_cast<Metal*>(mapObj.get()))
                {
                    if (dynamic_cast<PlayerPowerfulBullet*>(bullet.get()) && !(dynamic_cast<Bullet*>(bullet.get())->getIsDestroyed()) ) // и пуля не считается уничтоженной
                    {
                        auto visitor1 = std::make_shared<CollisionWithBulletVisitor>();
                        auto visitor2 = std::make_shared<CollisionWithMapObjectVisitor>();
                        mapObj->handleCollision(visitor1.get());
                        bullet->handleCollision(visitor2.get());
                        continue;
                    }
                    else
                    {
                        auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                        bullet->handleCollision(visitor.get());
                        continue;
                    }
                }
                else
                {
                    auto visitor1 = std::make_shared<CollisionWithBulletVisitor>();
                    auto visitor2 = std::make_shared<CollisionWithMapObjectVisitor>();
                    mapObj->handleCollision(visitor1.get());
                    bullet->handleCollision(visitor2.get());
                }
            }
        }

        for (const auto &enemy : enemyTanks)
        {
            if (mapObj->getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()))
            {
                if (dynamic_cast<Brick*>(mapObj.get()) ||
                    dynamic_cast<Metal *>(mapObj.get()) ||
                    dynamic_cast<Water*>(mapObj.get())) // через траву можно ездить
                {
                    auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                    dynamic_cast<EnemyTank*>(enemy.get())->setIsCollidingWithMap();
                    enemy->handleCollision(visitor.get());
                }
            }
        }
    }
}


void GameEngine::render()
{
    for (auto &bullet: allBullets) {
        bullet->render(window);
    }

    playerTank->render(window);


    for (auto &enemyTank: enemyTanks) {
        enemyTank->render(window);
    }

    Map::render(map, window);

}

//void initEnemies()
//{
//    for (int i = 0; i < 10; i++)
//    {
//        std::shared_ptr<IGameObject> enemy = std::make_shared<EnemyTank>(PLAYER_SPAWN_X * FACTOR, PLAYER_SPAWN_Y * FACTOR, allBullets);
//    }
//}

void GameEngine::end() {
    // заставка мб
}
