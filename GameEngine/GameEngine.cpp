#include "GameEngine.h"
#include "../Tank/Tank.h"
#include "../Map/Map.h"

void GameEngine :: init()
{
    mainTexture.loadFromFile("SpriteList.png");

    IGameObject::texture = mainTexture;

    levelNum = 1;
    Map::loadMap(map, levelNum);
    playerTank = new PlayerTank(64 * FACTOR, 192 * FACTOR, allBullets);
    // рандомно заполняем вектор с танками врага
}

void GameEngine :: togglePause()
{
    isPaused = !isPaused;
}

void GameEngine :: update()
{
    if(!isPaused)
    {
        sf::Time time = clock.restart();
        float seconds = time.asSeconds();

        playerTank->update(seconds);

        // проверяем вектор врагов, если пуст какая-нибудь заставка и переход на новый уровень

    }
    else
        clock.restart();
}

void GameEngine :: handleCollisions()
{
    std::vector<IGameObject*> allObjects;
    allObjects.reserve(1 + map.size() + enemyTanks.size() + allBullets.size());

    allObjects.emplace_back(playerTank);
    allObjects.insert(allObjects.end(), enemyTanks.begin(), enemyTanks.end());
    allObjects.insert(allObjects.end(), allBullets.begin(), allBullets.end());

    IVisitor *visitor;

    for (auto object : allObjects)
    {
        visitor = new CollisionWithTankVisitor();

        // проверка с танком игрока
        //if () // И УКАЗАТЕЛИ НЕ РАВНЫ
        //    object->handleCollision(visitor);

        for (auto enemy : enemyTanks)
        {
            //if ()
            //    object->handleCollision(visitor);

        }

        delete visitor;
        visitor = new CollisionWithBulletVisitor();

        for (auto bullet : allBullets)
        {

            //if () // И УКАЗАТЕЛИ НЕ РАВНЫ
            //    object->handleCollision(visitor);
        }

        delete visitor;
        visitor = new CollisionWithMapObjectVisitor();

        for (auto mapObject : map)
        {
            //if () // И УКАЗАТЕЛИ НЕ РАВНЫ
            //    object->handleCollision();
        }

        delete visitor;
    }
}

void GameEngine :: render()
{
    playerTank->render(window);

    Map::render(map, window);
}


void GameEngine :: end()
{
    delete playerTank;

    //for (auto object : map) {
    //    delete object;
    //}
}
