#include "GameEngine.h"
#include "../Tank/Tank.h"
#include "../Bullet/Bullet.h"
#include "../Map/Map.h"
#include "../MapObject/MapObject.h"
#include "../Bonus/Bonus.h"

#include <random>
#include <chrono>

#define PLAYER_SPAWN_X 64
#define PLAYER_SPAWN_Y 192

#define ALLY_SPAWN_X 128
#define ALLY_SPAWN_Y 192


#define TIME_BETWEEN_RENDER_ENEMY_TANK 90
#define MAX_LEVEL_NUM 30
#define ENEMIES_ON_LEVEL 18

#define CLOCK_BONUS_TIME 15

#define BONUS_POINTS 500

void GameEngine::init(bool isTwoPlayers) {

    this->isTwoPlayers = isTwoPlayers;
    isPaused = false;
    isFlagFallen = false;
    isClockBonusActive = false;
    points = 0;
    mainTexture.loadFromFile(SPRITE_LIST_NAME);
    font.loadFromFile(FONT_NAME);

    IGameObject::texture = mainTexture;

    levelNum = 1;
    Map::loadMap(map, levelNum);

    timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;

    playerTank = std::make_shared<PlayerTank>(PLAYER_SPAWN_X * FACTOR, PLAYER_SPAWN_Y * FACTOR, allBullets, false);
    if (isTwoPlayers)
        allyTank = std::make_shared<PlayerTank>(ALLY_SPAWN_X * FACTOR, ALLY_SPAWN_Y * FACTOR, allBullets, true);

    remainingEnemies = ENEMIES_ON_LEVEL;
    enemiesOnScreen = 0;

    hud.setTexture(mainTexture);
    hud.setTextureRect(sf::IntRect(368, 0, 32, 224));
    hud.setPosition(13 * 16 * FACTOR, 0);
    hud.setScale(FACTOR, FACTOR);

}

void GameEngine::togglePause() {
    isPaused = !isPaused;
}

void GameEngine::update() {
    if (!isPaused) {
        if (isFlagFallen) {
            endInfo();
            restart();
        }

        if (remainingEnemies == 0 && enemiesOnScreen == 0) {
            while (1) {
                window.clear();

                std::string infoString = "Level complete!\n\n"
                                         "Your score is " + std::to_string(points)
                                         + "\n\nPress 'Enter' to continue";
                sf::Text text(infoString, font, 6 * FACTOR);
                text.setPosition(100, 100);

                window.draw(text);

                window.display();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                    break;
            }

            levelNum++;
            if (levelNum == MAX_LEVEL_NUM + 1)
                levelNum = 1;
            Map::loadMap(map, levelNum);
            remainingEnemies = ENEMIES_ON_LEVEL;
            enemiesOnScreen = 0; // тут окно с очками за уровень
            timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;
            bonus.reset(); // установить танки на начальные позиции + неуязвимость

            dynamic_cast<PlayerTank *>(playerTank.get())->setX(PLAYER_SPAWN_X * FACTOR);
            dynamic_cast<PlayerTank *>(playerTank.get())->setY(PLAYER_SPAWN_Y * FACTOR);
            dynamic_cast<PlayerTank *>(playerTank.get())->setIsInvulnerable();

            if (isTwoPlayers) {
                dynamic_cast<PlayerTank *>(allyTank.get())->setX(ALLY_SPAWN_X * FACTOR);
                dynamic_cast<PlayerTank *>(allyTank.get())->setY(ALLY_SPAWN_Y * FACTOR);
                dynamic_cast<PlayerTank *>(allyTank.get())->setIsInvulnerable();
            }
            return;
        }

        if (bonus != nullptr)
            bonusEffect();

        if (playerTank.use_count() != 0) {
            if (dynamic_cast<Tank *>(playerTank.get())->getHealths() == 0) {
                playerTank.reset();
            }
        }

        if (isTwoPlayers) {
            if (allyTank.use_count() != 0) {
                if (dynamic_cast<Tank *>(allyTank.get())->getHealths() == 0)
                    allyTank.reset();
            }
        }

        if (playerTank.use_count() == 0 && allyTank.use_count() == 0) {
            endInfo();
            restart();
            return;
        }

        dropEnemies();

        sf::Time time = timeBetweenFrames.restart();

        float seconds = time.asSeconds();

        Map::update(map);

        updateEnemies(seconds);

        if (clockBonusTimer.getElapsedTime().asSeconds() > CLOCK_BONUS_TIME && isClockBonusActive)
            isClockBonusActive = false;

        if (playerTank.use_count() != 0) {
            playerTank->update(seconds);
            if (isTwoPlayers && (allyTank.use_count() != 0))
                allyTank->update(seconds);
        }

        updateBullets(seconds);
    } else
        timeBetweenFrames.restart();
}

void GameEngine::endInfo()
{
    sf::Sprite gameOverSprite(mainTexture);
    gameOverSprite.setTextureRect(sf::IntRect(288, 184, 32, 16));
    gameOverSprite.setPosition(85 * FACTOR, 100 * FACTOR);
    gameOverSprite.setScale(FACTOR, FACTOR);

    while (1) {
        window.clear();
        std::string resultString = "Your score is " + std::to_string(points) + "\n\nPress 'Enter' to restart";
        sf::Text text(resultString, font, 6 * FACTOR);
        text.setPosition(100, 100);


        window.draw(gameOverSprite);
        window.draw(text);

        window.display();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            timeBetweenFrames.restart();
            return;
        }
    }
}

void GameEngine::updateEnemies(float seconds)
{
    for (auto it = enemyTanks.begin(); it != enemyTanks.end();) {
        if ((*it)->getIsDestroyed()) {
            if (auto *tank = dynamic_cast<EnemyTank *>((*it).get())) {
                if (tank->getIsBonusTank())
                    dropBonus();
            }
            points += (dynamic_cast<EnemyTank *>((*it).get())->getType() + 1) * 100;
            std::cout << points << std::endl;
            it = enemyTanks.erase(it);
            enemiesOnScreen--;
        } else if (!isClockBonusActive) {
            (*it)->update(seconds);
            ++it;
        } else
            ++it;
    }
}

void GameEngine::updateBullets(float seconds)
{
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

}

void GameEngine::handleCollisions() {
    std::vector<std::shared_ptr<IGameObject>> allObjects;
    allObjects.insert(allObjects.end(), enemyTanks.begin(), enemyTanks.end());
    allObjects.insert(allObjects.end(), allBullets.begin(), allBullets.end());

    if (playerTank.use_count() != 0 && allyTank.use_count() != 0)
    {
        if (playerTank->getSprite().getGlobalBounds().intersects(allyTank->getSprite().getGlobalBounds())) {
            auto visitor = std::make_shared<CollisionWithTankVisitor>();
            playerTank->handleCollision(visitor.get());
            allyTank->handleCollision(visitor.get());
        }
    }

    for (const auto &obj: allObjects) // идем по объединенному вектору танков врагов и всех пуль
    {
        if (playerTank.use_count() != 0)
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
                    dynamic_cast<EnemyTank *>(obj.get())->setIsColliding();
                }
            }
        }

        if (allyTank.use_count() != 0)
        {
            if (obj->getSprite().getGlobalBounds().intersects(
                    allyTank->getSprite().getGlobalBounds())) // обработка коллизии танка врага или пули с танком игрока
            {

                auto visitor = std::make_shared<CollisionWithTankVisitor>();

                if ((dynamic_cast<EnemyBullet *>(obj.get()) || dynamic_cast<EnemyFastBullet *>(obj.get()) &&
                                                               !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed()))) // если вражеская пуля попала в игрока
                {
                    obj->handleCollision(visitor.get());
                    auto newVisitor = std::make_shared<CollisionWithBulletVisitor>();
                    allyTank->handleCollision(newVisitor.get());
                } else if ((dynamic_cast<PlayerBullet *>(obj.get()) ||
                            dynamic_cast<PlayerFastBullet *>(obj.get()) //  если пуля союзника попала в игрока
                            || dynamic_cast<PlayerPowerfulBullet *>(obj.get())) &&
                           !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed())) {
                    obj->handleCollision(visitor.get());
                }

                if (dynamic_cast<Tank *>(obj.get())) // столкновение танка врага с танком игрока
                {
                    obj->handleCollision(visitor.get());
                    allyTank->handleCollision(visitor.get());
                    dynamic_cast<EnemyTank *>(obj.get())->setIsColliding();
                }
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

                dynamic_cast<EnemyTank *>(obj.get())->setIsColliding();
                dynamic_cast<EnemyTank *>(enemy.get())->setIsColliding();
                continue;
            }
        }

    }

    for (const auto &mapObj: map) // обработка столкновений танка игрока с объектами на карте
    {
        if (playerTank.use_count() != 0) {
            if (mapObj->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds())) {
                if (dynamic_cast<Brick *>(mapObj.get()) ||
                    dynamic_cast<Metal *>(mapObj.get()) ||
                    dynamic_cast<Water *>(mapObj.get()) ||
                    dynamic_cast<Eagle *>(mapObj.get())) // через траву можно ездить
                {
                    auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                    playerTank->handleCollision(visitor.get());
                }
            }
        }

        if (allyTank.use_count() != 0) {
            if (mapObj->getSprite().getGlobalBounds().intersects(allyTank->getSprite().getGlobalBounds())) {
                if (dynamic_cast<Brick *>(mapObj.get()) ||
                    dynamic_cast<Metal *>(mapObj.get()) ||
                    dynamic_cast<Water *>(mapObj.get()) ||
                    dynamic_cast<Eagle *>(mapObj.get())) // через траву можно ездить
                {
                    auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                    allyTank->handleCollision(visitor.get());
                }
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
                    dynamic_cast<EnemyTank *>(enemy.get())->setIsColliding();
                    enemy->handleCollision(visitor.get());
                    continue;
                }
            }
        }
    }

    if (bonus.use_count() != 0)
    {
        auto visitor = std::make_shared<CollisionWithTankVisitor>();
        if (bonus->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds()))
            bonus->handleCollision(visitor.get());

        if(allyTank.use_count() != 0) {
            if (bonus->getSprite().getGlobalBounds().intersects(allyTank->getSprite().getGlobalBounds()))
                bonus->handleCollision(visitor.get());
        }
    }
}

void GameEngine::render() {
    for (auto &bullet: allBullets) {
        bullet->render(window);
    }

    if (playerTank.use_count() != 0)
        playerTank->render(window);

    if (isTwoPlayers && allyTank.use_count() != 0)
        allyTank->render(window);

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

    renderHUD();

    if (isPaused) {
        sf::Sprite pauseSprite(mainTexture);
        pauseSprite.setTextureRect(sf::IntRect(288, 176, 40, 8));
        pauseSprite.setPosition(88 * FACTOR, 96 * FACTOR);
        pauseSprite.setScale(FACTOR, FACTOR);
        window.draw(pauseSprite);
    }

}

void GameEngine::renderHUD() {
    std::vector<sf::Sprite> remEnem;
    sf::Sprite littleTankSprite(mainTexture);
    littleTankSprite.setTextureRect(sf::IntRect(320, 192, 8, 8));
    for (int i = 0; i < remainingEnemies / 2; i++) {
        for (int j = 0; j < 2; j++) {
            littleTankSprite.setPosition((float) (216 + j * 8) * FACTOR,
                                         (float) (16 + i * 8) * FACTOR);  // задание координат клетки
            littleTankSprite.setScale(FACTOR, FACTOR);
            remEnem.push_back(littleTankSprite);
        }
    }
    if (remainingEnemies % 2 == 1) {
        littleTankSprite.setPosition(216 * FACTOR, (float) (16 + (remainingEnemies / 2) * 8) * FACTOR);
        littleTankSprite.setScale(FACTOR, FACTOR);
        remEnem.push_back(littleTankSprite);
    }
    for (const auto &sprite: remEnem) {
        window.draw(sprite);
    }

    if (playerTank.use_count() != 0) {
        int tmpHealths = dynamic_cast<PlayerTank *>(playerTank.get())->getHealths();
        sf::Sprite healthSprite(mainTexture);
        healthSprite.setTextureRect(sf::IntRect(288 + (8 * tmpHealths), 200, 8, 8));
        healthSprite.setPosition(224 * FACTOR, 136 * FACTOR);
        healthSprite.setScale(FACTOR, FACTOR);
        window.draw(healthSprite);
    }
}

void GameEngine::restart() {
    isFlagFallen = false;
    levelNum = 1;
    timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;

    playerTank = std::make_shared<PlayerTank>(PLAYER_SPAWN_X * FACTOR, PLAYER_SPAWN_Y * FACTOR, allBullets, false);
    if (isTwoPlayers)
        allyTank = std::make_shared<PlayerTank>(ALLY_SPAWN_X * FACTOR, ALLY_SPAWN_Y * FACTOR, allBullets, true);

    bonus.reset();
    remainingEnemies = 18;
    enemiesOnScreen = 0;
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

    auto *handlingBonus = dynamic_cast<Bonus *>(bonus.get());

    if (!(handlingBonus->getIsPicked()))
        return;

    points += BONUS_POINTS;

    if (handlingBonus->getBonusType() == HELMET) {
        if (bonus->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds()))
            dynamic_cast<PlayerTank *>(playerTank.get())->setIsInvulnerable();
        else if (isTwoPlayers)
            dynamic_cast<PlayerTank *>(allyTank.get())->setIsInvulnerable();
    } else if (handlingBonus->getBonusType() == CLOCK) {
        isClockBonusActive = true;
        clockBonusTimer.restart();
    } else if (handlingBonus->getBonusType() == SHOVEL) // устанавливает металл вокруг базы
    {

    } else if (handlingBonus->getBonusType() == STAR) {
        if (bonus->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds()))
            dynamic_cast<PlayerTank *>(playerTank.get())->addStar();
        else if (isTwoPlayers)
            dynamic_cast<PlayerTank *>(allyTank.get())->addStar();
    } else if (handlingBonus->getBonusType() == GRENADE) {
        enemiesOnScreen = 0;
        enemyTanks.clear();
    } else if (handlingBonus->getBonusType() == TANK) {
        if (bonus->getSprite().getGlobalBounds().intersects(playerTank->getSprite().getGlobalBounds()))
            dynamic_cast<PlayerTank *>(playerTank.get())->addHealth();
        else if (isTwoPlayers)
            dynamic_cast<PlayerTank *>(allyTank.get())->addHealth();
    }
    bonus.reset();
}

void GameEngine::dropEnemies() {
    if (enemiesOnScreen != 4)
        timeBetweenRenderEnemyTank--;

    if (remainingEnemies != 0 && enemiesOnScreen < 4 && timeBetweenRenderEnemyTank <= 0) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> distribution(0, 3);
        int type = distribution(generator);

        enemyWithBonusCounter++;

        std::shared_ptr<IGameObject> enemy;

        if ((enemyWithBonusCounter % 4) == 0) {
            enemy = std::make_shared<EnemyTank>(6. * (remainingEnemies % 3) * 16. * FACTOR, 0., allBullets, type,
                                                true);
            enemyWithBonusCounter = 0;
        } else
            enemy = std::make_shared<EnemyTank>(6. * (remainingEnemies % 3) * 16. * FACTOR, 0., allBullets, type,
                                                false);

        enemyTanks.push_back(enemy);
        remainingEnemies--;
        enemiesOnScreen++;

        timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;
    }
}

void GameEngine::end() {
    // заставка мб
}
