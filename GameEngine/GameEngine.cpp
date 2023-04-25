#include "GameEngine.h"
#include "../Tank/Tank.h"
#include "../Bullet/Bullet.h"
#include "../Map/Map.h"

#include <iostream>

void GameEngine::init() {
    mainTexture.loadFromFile("SpriteList.png");

    IGameObject::texture = mainTexture;

    levelNum = 1;
    Map::loadMap(map, levelNum);

    // рандомно заполняем вектор с танками врага

    playerTank = std::make_unique<PlayerTank>(64 * FACTOR, 192 * FACTOR, allBullets);
}

void GameEngine::togglePause() {
    isPaused = !isPaused;
}

void GameEngine::update()
{
    if (!isPaused)
    {
        sf::Time time = clock.restart();
        float seconds = time.asSeconds();

        playerTank->update(seconds);

        std::cout << allBullets.size() << std::endl;

        for (auto &gameObject : allBullets)
        {
            if (auto *bullet = dynamic_cast<Bullet *>(gameObject.get()))
            {
                if (bullet->getIsDestroyed())
                {
                    if (auto *tank = dynamic_cast<Tank *>(bullet->getOwner()))
                        tank->enableShooting();
                    gameObject.reset();
                }
                else
                {
                    bullet->update(seconds);
                }
            }
        }
        allBullets.erase(std::remove_if(allBullets.begin(), allBullets.end(), [](const auto &gameObject)
        { return gameObject == nullptr; }), allBullets.end()); // возможно удаляет все элементы после нулевого указателя
    }
    else
        clock.restart();
}

void GameEngine::handleCollisions() {

    /*std::vector<IGameObject *> allObjects;
     allObjects.reserve(1 + map.size() + enemyTanks.size() + allBullets.size());

     allObjects.emplace_back(playerTank);
     allObjects.insert(allObjects.end(), enemyTanks.begin(), enemyTanks.end());
     allObjects.insert(allObjects.end(), allBullets.begin(), allBullets.end());

     IVisitor *visitor;

     for (auto object: allObjects)
     {

         // проверка с танком игрока
         if (object->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds()) &&
             object != playerTank) {
             visitor = new CollisionWithTankVisitor();
             object->handleCollision(visitor);
             delete visitor;
         }

         for (auto enemy: enemyTanks) {
             if (object->getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()) &&
                 object != enemy) {
                 visitor = new CollisionWithTankVisitor();
                 object->handleCollision(visitor);
                 delete visitor;
             }

         }

         for (auto bullet: allBullets) {
             if (object->getSprite().getGlobalBounds().intersects(bullet->getSprite().getGlobalBounds()) &&
                 object != bullet) {
                 visitor = new CollisionWithBulletVisitor();
                 object->handleCollision(visitor);
                 delete visitor;
             }
         }

         for (auto mapObject: map) {
             if (object->getSprite().getGlobalBounds().intersects(mapObject->getSprite().getGlobalBounds()) &&
                 object != mapObject) {
                 visitor = new CollisionWithMapObjectVisitor();
                 object->handleCollision(visitor);
                 delete visitor;
             }
         }
     } */
}

void GameEngine::render()
{
    playerTank->render(window);

    for (auto& bullet : allBullets) {
        bullet->render(window);
    }

    Map::render(map, window);
}


void GameEngine :: end()
{
    // заставка мб
}
