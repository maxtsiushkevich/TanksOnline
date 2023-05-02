#include "GameEngine.h"
#include "../Tank/Tank.h"
#include "../Bullet/Bullet.h"
#include "../Map/Map.h"
#include "../MapObject/MapObject.h"
#include "../Bonus/Bonus.h"

#include <unistd.h>

#include <random>
#include <chrono>

#define PLAYER_SPAWN_X 64
#define PLAYER_SPAWN_Y 192
#define TIME_BETWEEN_RENDER_ENEMY_TANK 90
#define MAX_LEVEL_NUM 30

void GameEngine::init() {
    isFlagFallen = false;
    mainTexture.loadFromFile("SpriteList.png");

    IGameObject::texture = mainTexture;

    levelNum = 1;
    Map::loadMap(map, levelNum);

    timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;

    playerTank = std::make_shared<PlayerTank>(PLAYER_SPAWN_X * FACTOR, PLAYER_SPAWN_Y * FACTOR, allBullets);

    remainingEnemies = 18;
    enemiesOnMap = 0;

    hud.setTexture(mainTexture);
    hud.setTextureRect(sf::IntRect(368, 0, 32, 224));
    hud.setPosition(13 * 16 * FACTOR, 0);
    hud.setScale(FACTOR, FACTOR);

}

void GameEngine::togglePause() {
    isPaused = !isPaused;
}

void GameEngine::update() {
    if (!isPaused)
    {
        //std::cout << isFlagFallen << std::endl;

        if (isFlagFallen) // еще какое-то окно гейм овер
        {
            sleep(3);
            clock.restart();
            restart();
        }

        bonusEffect();

        if (dynamic_cast<Tank *>(playerTank.get())->getHealths() == 0)
            restart();

        dropEnemies();

        sf::Time time = clock.restart();
        float seconds = time.asSeconds();

        playerTank->update(seconds);

        Map::update(map);

        for (auto it = enemyTanks.begin(); it != enemyTanks.end();) {
            if ((*it)->getIsDestroyed()) {
                if (auto *tank = dynamic_cast<EnemyTank *>((*it).get())) {
                    if (tank->getIsBonusTank())
                        dropBonus();
                }
                it = enemyTanks.erase(it);
                enemiesOnMap--;
            } else {
                (*it)->update(seconds);
                ++it;
            }
        }

        allBullets.erase(std::remove_if(allBullets.begin(), allBullets.end(),
                                        [seconds](const auto &gameObject) {
                                            if (auto *bullet = dynamic_cast<Bullet *>(gameObject.get())) {
                                                if (bullet->getIsDestroyed()) {
                                                    auto owner = bullet->getOwner(); // получаем shared_ptr на владельца пули
                                                    if (auto *tank = dynamic_cast<Tank *>(owner.get())) // проверяем, что owner не нулевой
                                                        tank->enableShooting(); // вызываем только если owner не нулевой
                                                    return true;
                                                } else
                                                    bullet->update(seconds);
                                            }
                                            return false;
                                        }), allBullets.end());

    } else
        clock.restart();
}

void GameEngine::handleCollisions() {
    std::vector<std::shared_ptr<IGameObject>> allObjects;
    allObjects.insert(allObjects.end(), enemyTanks.begin(), enemyTanks.end());
    allObjects.insert(allObjects.end(), allBullets.begin(), allBullets.end());

    for (const auto &obj: allObjects) // идем по объединенному вектору танков врагов и всех пуль
    {
        if (obj->getSprite().getGlobalBounds().intersects(
                playerTank->getSprite().getGlobalBounds())) // обработка коллизии танка врага или пули с танком игрока
        {

            auto visitor = std::make_shared<CollisionWithTankVisitor>();

            if ((dynamic_cast<EnemyBullet *>(obj.get()) || dynamic_cast<EnemyFastBullet *>(obj.get()) &&
                                                           !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed()))) // если вражеская пуля попала в игрока
            {
                obj->handleCollision(visitor.get());
                auto newVisitor = std::make_shared<CollisionWithBulletVisitor>();
                playerTank->handleCollision(newVisitor.get());
            } else if ((dynamic_cast<PlayerBullet *>(obj.get()) ||
                        dynamic_cast<PlayerFastBullet *>(obj.get()) //  если пуля союзника попала в игрока
                        || dynamic_cast<PlayerPowerfulBullet *>(obj.get())) &&
                       !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed())) {
                obj->handleCollision(visitor.get());
            }

            if (dynamic_cast<Tank *>(obj.get())) // столкновение танка врага с танком игрока
            {
                obj->handleCollision(visitor.get());
                playerTank->handleCollision(visitor.get());
                //continue;
            }
        }

        for (const auto &bullet: allBullets) // обработка столкновений всех пуль
        {
            if (obj->getSprite().getGlobalBounds().intersects(bullet->getSprite().getGlobalBounds()) &&
                obj.get() != bullet.get()) // проверяем чтобы не была одна и та же пуля
            {
                if (dynamic_cast<Bullet *>(obj.get()) &&
                    !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed())) // пуля с пулей и пуля не считается уничтоженной
                {
                    auto visitor = std::make_shared<CollisionWithBulletVisitor>();
                    obj->handleCollision(visitor.get());
                    bullet->handleCollision(visitor.get());
                    continue;
                }
                if (dynamic_cast<Tank *>(obj.get())) // пуля с танком врага
                {
                    if ((dynamic_cast<PlayerBullet *>(bullet.get()) ||
                         dynamic_cast<PlayerFastBullet *>(bullet.get()) // обработка пуль союзников при столкновении c танком врага
                         || dynamic_cast<PlayerPowerfulBullet *>(bullet.get())) &&
                        !(dynamic_cast<Bullet *>(bullet.get())->getIsDestroyed())) {
                        auto visitor1 = std::make_shared<CollisionWithBulletVisitor>();
                        auto visitor2 = std::make_shared<CollisionWithTankVisitor>();
                        obj->handleCollision(visitor1.get());
                        bullet->handleCollision(visitor2.get());
                        continue;
                    } else {
                        auto visitor = std::make_shared<CollisionWithTankVisitor>();
                        bullet->handleCollision(visitor.get());
                        continue;
                    }
                }

            }
        }

        for (const auto &enemy: enemyTanks) {
            if (obj->getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()) &&
                obj.get() != enemy.get() && dynamic_cast<Tank *>(obj.get())) {
                auto visitor = std::make_shared<CollisionWithTankVisitor>();
                obj->handleCollision(visitor.get());
                enemy->handleCollision(visitor.get());
                continue;
            }
        }

    }

    for (const auto &mapObj: map) // обработка столкновений танка игрока с объектами на карте
    {
        if (mapObj->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds())) {
            if (dynamic_cast<Brick *>(mapObj.get()) ||
                dynamic_cast<Metal *>(mapObj.get()) ||
                dynamic_cast<Water *>(mapObj.get()) || dynamic_cast<Eagle *>(mapObj.get())) // через траву можно ездить
            {
                auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                playerTank->handleCollision(visitor.get());
                //continue;
            }
        }

        for (const auto &bullet: allBullets) {
            if (mapObj->getSprite().getGlobalBounds().intersects(bullet->getSprite().getGlobalBounds())) {
                if (dynamic_cast<Grass *>(mapObj.get()) || dynamic_cast<Water *>(mapObj.get()))
                    continue;
                if (dynamic_cast<Metal *>(mapObj.get())) {
                    if (dynamic_cast<PlayerPowerfulBullet *>(bullet.get()) &&
                        !(dynamic_cast<Bullet *>(bullet.get())->getIsDestroyed())) // и пуля не считается уничтоженной
                    {
                        auto visitor1 = std::make_shared<CollisionWithBulletVisitor>();
                        auto visitor2 = std::make_shared<CollisionWithMapObjectVisitor>();
                        mapObj->handleCollision(visitor1.get());
                        bullet->handleCollision(visitor2.get());
                        continue;
                    } else {
                        auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                        bullet->handleCollision(visitor.get());
                        continue;
                    }
                }
                if (dynamic_cast<Eagle *>(mapObj.get())) {
                    isFlagFallen = true;
                    auto visitor1 = std::make_shared<CollisionWithBulletVisitor>();
                    auto visitor2 = std::make_shared<CollisionWithMapObjectVisitor>();
                    mapObj->handleCollision(visitor1.get());
                    bullet->handleCollision(visitor2.get());
                } else {
                    auto visitor1 = std::make_shared<CollisionWithBulletVisitor>();
                    auto visitor2 = std::make_shared<CollisionWithMapObjectVisitor>();
                    mapObj->handleCollision(visitor1.get());
                    bullet->handleCollision(visitor2.get());
                    continue;
                }
            }
        }

        for (const auto &enemy: enemyTanks) {
            if (mapObj->getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds())) {
                if (dynamic_cast<Brick *>(mapObj.get()) ||
                    dynamic_cast<Metal *>(mapObj.get()) ||
                    dynamic_cast<Water *>(mapObj.get())) // через траву можно ездить
                {
                    auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                    dynamic_cast<EnemyTank *>(enemy.get())->setIsCollidingWithMap();
                    enemy->handleCollision(visitor.get());
                    continue;
                }
            }
        }
    }

    if (bonus != nullptr) {
        if (bonus->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds()))
        {
            auto visitor = std::make_shared<CollisionWithTankVisitor>();
            bonus->handleCollision(visitor.get());
            //continue;
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

    //рендер бонуса
    if (bonus != nullptr) {
        if (!dynamic_cast<Bonus *>(bonus.get())->getIsPicked())
            bonus->render(window);
    }
    window.draw(hud);

    if (isPaused) // рендерим полупрозрачный спрайт с надписью PAUSE
    {}
}

void GameEngine::updateHUD() {

}

void GameEngine::restart() {
    isFlagFallen = false;
    levelNum = 1;
    timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;

    playerTank = std::make_shared<PlayerTank>(PLAYER_SPAWN_X * FACTOR, PLAYER_SPAWN_Y * FACTOR, allBullets);

    remainingEnemies = 18;
    enemiesOnMap = 0;
    points = 0;
    allBullets.clear();
    enemyTanks.clear();
    map.clear();
    Map::loadMap(map, levelNum);
}

void GameEngine::dropBonus() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 25);

    std::uniform_int_distribution<int> distributionType(0, 5);

    int x = distribution(generator);
    int y = distribution(generator);
    int type = distributionType(generator);

    bonus = std::make_shared<Bonus>(8 * FACTOR * x, 8 * FACTOR * y, type);
}

void GameEngine::bonusEffect() {
    if (bonus == nullptr)
        return;




    if ((dynamic_cast<Bonus*>(bonus.get())->getBonusType() == STAR) && dynamic_cast<Bonus*>(bonus.get())->getIsPicked())
    {
        dynamic_cast<PlayerTank *>(playerTank.get())->addStar();
        bonus.reset();
    }
}

void GameEngine ::dropEnemies()
{
    timeBetweenRenderEnemyTank--;
    if (remainingEnemies != 0 && enemiesOnMap < 4 && timeBetweenRenderEnemyTank == 0) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> distribution(0, 3);
        int type = distribution(generator);

        enemyWithBonusCounter++;

        std::shared_ptr<IGameObject> enemy;

        if ((enemyWithBonusCounter % 4) == 0)
            enemy = std::make_shared<EnemyTank>(6. * (remainingEnemies % 3) * 16. * FACTOR, 0., allBullets, type,
                                                true);
        else
            enemy = std::make_shared<EnemyTank>(6. * (remainingEnemies % 3) * 16. * FACTOR, 0., allBullets, type,
                                                false);

        enemyTanks.push_back(enemy);
        remainingEnemies--;

        enemiesOnMap++;

        timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;
    }

    if (remainingEnemies == 0 && enemiesOnMap == 0) {
        levelNum++;
        if (levelNum == MAX_LEVEL_NUM + 1)
            levelNum = 1;
        Map::loadMap(map, levelNum);
        remainingEnemies = 10;
        enemiesOnMap = 0; // тут окно с очками за уровень
        timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;
        return;
    }
}

void GameEngine::end() {
    // заставка мб
}
