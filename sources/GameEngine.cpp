#include "../headers/GameEngine.h"

#include <fstream>

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

#define PORT 6005

void GameEngine::init(bool isTwoPlayers, bool isOnline) {
    this->isTwoPlayers = isTwoPlayers;
    this->isOnline = isOnline;
    gameState.isPaused = false;
    isFlagFallen = false;
    isClockBonusActive = false;
    gameState.points = 0;
    mainTexture.loadFromFile(SPRITE_LIST_NAME);
    font.loadFromFile(FONT_NAME);

    IGameObject::texture = mainTexture;

    levelNum = 1;
    Map::loadMap(gameState.map, levelNum);

    timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;

    gameState.playerTank = std::make_shared<PlayerTank>(PLAYER_SPAWN_X * FACTOR, PLAYER_SPAWN_Y * FACTOR,
                                                        gameState.allBullets, false);
    if (isTwoPlayers)
        gameState.allyTank = std::make_shared<PlayerTank>(ALLY_SPAWN_X * FACTOR, ALLY_SPAWN_Y * FACTOR,
                                                          gameState.allBullets, true);

    gameState.remainingEnemies = ENEMIES_ON_LEVEL;
    enemiesOnScreen = 0;

    hud.setTexture(mainTexture);
    hud.setTextureRect(sf::IntRect(368, 0, 32, 224));
    hud.setPosition(13 * 16 * FACTOR, 0);
    hud.setScale(FACTOR, FACTOR);

    if (isOnline)
        connect();

}

void GameEngine::connect()
{
    while (1)
    {
        window.clear();
        std::string resultString = "Press 'S' for waiting connection \nor 'C' for connect\n";
        sf::Text text(resultString, font, 6 * FACTOR);
        text.setPosition(20 * FACTOR, 20 * FACTOR);
        window.draw(text);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            // показывается ip и ждется подключение
            std::string ip = multiplayer.getIpFromStruct();

            while (1)
            {
                window.clear();
                sf::Text text1(ip + "\n\nEnter this IP at the client computer", font, 6 * FACTOR);
                text1.setPosition(20 * FACTOR, 20 * FACTOR);
                window.draw(text1);


                window.display();

                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                        exit(0);
                    }
                    if (multiplayer.serverInit(PORT))
                    {
                        sf::Text text1("Connection...", font, 6 * FACTOR);
                        text1.setPosition(400 * FACTOR, 40 * FACTOR);
                        window.draw(text1);
                        window.display();
                        sleep(5);
                        break;
                    }
                }
                break;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            // показывается поле ввода, где можно ввести IP сервера, к которому подключаться
            std::string ip;
            sf::Text text(ip, font, 6 * FACTOR);
            text.setPosition(20 * FACTOR, 20 * FACTOR);
            while (1)
            {
                window.clear();
                sf::Text text1("Enter server IP: \n\n", font, 6 * FACTOR);
                text1.setPosition(20 * FACTOR, 20 * FACTOR);
                window.draw(text1);

                text.setString(ip);
                window.draw(text);

                window.display();

                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                        exit(0);
                    }
                    if (event.type == sf::Event::TextEntered)
                    {
                        if (event.text.unicode == 8 && ip.size() > 0) // обработка нажатия backspace
                            ip.erase(ip.size() - 1, 1);
                        else if (event.text.unicode < 128 && event.text.unicode != 13) // только ASCII символы, кроме Enter
                            ip += static_cast<char>(event.text.unicode);
                        text.setString(ip);
                    }
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                    {
                        if (multiplayer.clientInit(ip, PORT))
                            break;
                        else {
                            sf::Text text2("Connection error. Wrong IP.", font, 6 * FACTOR);
                            text2.setPosition(20 * FACTOR, 80 * FACTOR);
                            window.draw(text2);
                            window.display();
                            sleep(3);
                            continue;
                        }
                    }
                }
            }
        }
        window.display();
    }

}

void GameEngine::togglePause() {
    gameState.isPaused = !gameState.isPaused;
}

void GameEngine::update() {
/*    std::ofstream ofile("game_state.txt", std::ios::binary);
//    boost::archive::text_oarchive oa(ofile);
//    oa << gameState;
//    ofile.close();
//
//     //Десериализуем объект
//    std::ifstream ifile("game_state.txt", std::ios::binary);
//    boost::archive::text_iarchive ia(ifile);
//
//    GameState newGameState;
//    ia >> newGameState;
//    ifile.close();*/

    // захватываем мьютекс
    if (!gameState.isPaused) {
        if (isFlagFallen) {
            endInfo();
            restart();
            // освобождаем мьютекс
            return;
        }

        if (gameState.remainingEnemies == 0 && enemiesOnScreen == 0) {
            while (1) {
                window.clear();

                std::string infoString = "Level complete!\n\n"
                                         "Your score is " + std::to_string(gameState.points)
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
            Map::loadMap(gameState.map, levelNum);
            gameState.remainingEnemies = ENEMIES_ON_LEVEL;
            enemiesOnScreen = 0; // тут окно с очками за уровень
            timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;
            gameState.bonus.reset(); // установить танки на начальные позиции + неуязвимость

            dynamic_cast<PlayerTank *>(gameState.playerTank.get())->setX(PLAYER_SPAWN_X * FACTOR);
            dynamic_cast<PlayerTank *>(gameState.playerTank.get())->setY(PLAYER_SPAWN_Y * FACTOR);
            dynamic_cast<PlayerTank *>(gameState.playerTank.get())->setIsInvulnerable();

            if (isTwoPlayers) {
                dynamic_cast<PlayerTank *>(gameState.allyTank.get())->setX(ALLY_SPAWN_X * FACTOR);
                dynamic_cast<PlayerTank *>(gameState.allyTank.get())->setY(ALLY_SPAWN_Y * FACTOR);
                dynamic_cast<PlayerTank *>(gameState.allyTank.get())->setIsInvulnerable();
            }
            // освобождаем мьютекс
            return;
        }

        if (gameState.bonus != nullptr)
            bonusEffect();

        if (gameState.playerTank.use_count() != 0) {
            if (dynamic_cast<Tank *>(gameState.playerTank.get())->getHealths() == 0) {
                gameState.playerTank.reset();
            }
        }

        if (isTwoPlayers && !isOnline) {
            if (gameState.allyTank.use_count() != 0) {
                if (dynamic_cast<Tank *>(gameState.allyTank.get())->getHealths() == 0)
                    gameState.allyTank.reset();
            }
        }

        if (gameState.playerTank.use_count() == 0 && gameState.allyTank.use_count() == 0) {
            endInfo();
            restart();
            // освобождаем мьютекс
            return;
        }

        dropEnemies();

        sf::Time time = timeBetweenFrames.restart();
        float seconds = time.asSeconds();

        Map::update(gameState.map);

        updateEnemies(seconds);

        if (clockBonusTimer.getElapsedTime().asSeconds() > CLOCK_BONUS_TIME && isClockBonusActive)
            isClockBonusActive = false;

        if (gameState.playerTank.use_count() != 0) {
            gameState.playerTank->update(seconds);
            if (isTwoPlayers && (gameState.allyTank.use_count() != 0) && !isOnline)
                gameState.allyTank->update(seconds);
        }

        updateBullets(seconds);
    } else
        timeBetweenFrames.restart();
    // освобождаем мьютекс
}

void GameEngine::endInfo() {
    sf::Sprite gameOverSprite(mainTexture);
    gameOverSprite.setTextureRect(sf::IntRect(288, 184, 32, 16));
    gameOverSprite.setPosition(85 * FACTOR, 100 * FACTOR);
    gameOverSprite.setScale(FACTOR, FACTOR);

    while (1) {
        window.clear();
        std::string resultString = "Your score is " + std::to_string(gameState.points) + "\n\nPress 'Enter' to restart";
        sf::Text text(resultString, font, 6 * FACTOR);
        text.setPosition(20 * FACTOR, 20 * FACTOR);


        window.draw(gameOverSprite);
        window.draw(text);

        window.display();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            timeBetweenFrames.restart();
            return;
        }
    }
}

void GameEngine::updateEnemies(float seconds) {
    for (auto it = gameState.enemyTanks.begin(); it != gameState.enemyTanks.end();) {
        if ((*it)->getIsDestroyed()) {
            if (auto *tank = dynamic_cast<EnemyTank *>((*it).get())) {
                if (tank->getIsBonusTank())
                    dropBonus();
            }
            gameState.points += (dynamic_cast<EnemyTank *>((*it).get())->getType() + 1) * 100;
            std::cout << gameState.points << std::endl;
            it = gameState.enemyTanks.erase(it);
            enemiesOnScreen--;
        } else if (!isClockBonusActive) {
            (*it)->update(seconds);
            ++it;
        } else
            ++it;
    }
}

void GameEngine::updateBullets(float seconds) {
    gameState.allBullets.erase(std::remove_if(gameState.allBullets.begin(), gameState.allBullets.end(),
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
                                              }), gameState.allBullets.end());

}

void GameEngine::handleCollisions() {
    std::vector<std::shared_ptr<IGameObject>> allObjects;
    allObjects.insert(allObjects.end(), gameState.enemyTanks.begin(), gameState.enemyTanks.end());
    allObjects.insert(allObjects.end(), gameState.allBullets.begin(), gameState.allBullets.end());

    if (gameState.playerTank.use_count() != 0 && gameState.allyTank.use_count() != 0) {
        if (gameState.playerTank->getSprite().getGlobalBounds().intersects(
                gameState.allyTank->getSprite().getGlobalBounds())) {
            auto visitor = std::make_shared<CollisionWithTankVisitor>();
            gameState.playerTank->handleCollision(visitor.get());
            gameState.allyTank->handleCollision(visitor.get());
        }
    }

    for (const auto &obj: allObjects) // идем по объединенному вектору танков врагов и всех пуль
    {
        if (gameState.playerTank.use_count() != 0) {
            if (obj->getSprite().getGlobalBounds().intersects(
                    gameState.playerTank->getSprite().getGlobalBounds())) // обработка коллизии танка врага или пули с танком игрока
            {

                auto visitor = std::make_shared<CollisionWithTankVisitor>();

                if ((dynamic_cast<EnemyBullet *>(obj.get()) || dynamic_cast<EnemyFastBullet *>(obj.get()) &&
                                                               !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed()))) // если вражеская пуля попала в игрока
                {
                    obj->handleCollision(visitor.get());
                    auto newVisitor = std::make_shared<CollisionWithBulletVisitor>();
                    gameState.playerTank->handleCollision(newVisitor.get());
                } else if ((dynamic_cast<PlayerBullet *>(obj.get()) ||
                            dynamic_cast<PlayerFastBullet *>(obj.get()) //  если пуля союзника попала в игрока
                            || dynamic_cast<PlayerPowerfulBullet *>(obj.get())) &&
                           !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed())) {
                    obj->handleCollision(visitor.get());
                }

                if (dynamic_cast<Tank *>(obj.get())) // столкновение танка врага с танком игрока
                {
                    obj->handleCollision(visitor.get());
                    gameState.playerTank->handleCollision(visitor.get());
                    dynamic_cast<EnemyTank *>(obj.get())->setIsColliding();
                }
            }
        }

        if (gameState.allyTank.use_count() != 0) {
            if (obj->getSprite().getGlobalBounds().intersects(
                    gameState.allyTank->getSprite().getGlobalBounds())) // обработка коллизии танка врага или пули с танком игрока
            {

                auto visitor = std::make_shared<CollisionWithTankVisitor>();

                if ((dynamic_cast<EnemyBullet *>(obj.get()) || dynamic_cast<EnemyFastBullet *>(obj.get()) &&
                                                               !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed()))) // если вражеская пуля попала в игрока
                {
                    obj->handleCollision(visitor.get());
                    auto newVisitor = std::make_shared<CollisionWithBulletVisitor>();
                    gameState.allyTank->handleCollision(newVisitor.get());
                } else if ((dynamic_cast<PlayerBullet *>(obj.get()) ||
                            dynamic_cast<PlayerFastBullet *>(obj.get()) //  если пуля союзника попала в игрока
                            || dynamic_cast<PlayerPowerfulBullet *>(obj.get())) &&
                           !(dynamic_cast<Bullet *>(obj.get())->getIsDestroyed())) {
                    obj->handleCollision(visitor.get());
                }

                if (dynamic_cast<Tank *>(obj.get())) // столкновение танка врага с танком игрока
                {
                    obj->handleCollision(visitor.get());
                    gameState.allyTank->handleCollision(visitor.get());
                    dynamic_cast<EnemyTank *>(obj.get())->setIsColliding();
                }
            }
        }

        for (const auto &bullet: gameState.allBullets) // обработка столкновений всех пуль
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

        for (const auto &enemy: gameState.enemyTanks) {
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

    for (const auto &mapObj: gameState.map) // обработка столкновений танка игрока с объектами на карте
    {
        if (gameState.playerTank.use_count() != 0) {
            if (mapObj->getSprite().getGlobalBounds().intersects(gameState.playerTank->getSprite().getGlobalBounds())) {
                if (dynamic_cast<Brick *>(mapObj.get()) ||
                    dynamic_cast<Metal *>(mapObj.get()) ||
                    dynamic_cast<Water *>(mapObj.get()) ||
                    dynamic_cast<Eagle *>(mapObj.get())) // через траву можно ездить
                {
                    auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                    gameState.playerTank->handleCollision(visitor.get());
                }
            }
        }

        if (gameState.allyTank.use_count() != 0) {
            if (mapObj->getSprite().getGlobalBounds().intersects(gameState.allyTank->getSprite().getGlobalBounds())) {
                if (dynamic_cast<Brick *>(mapObj.get()) ||
                    dynamic_cast<Metal *>(mapObj.get()) ||
                    dynamic_cast<Water *>(mapObj.get()) ||
                    dynamic_cast<Eagle *>(mapObj.get())) // через траву можно ездить
                {
                    auto visitor = std::make_shared<CollisionWithMapObjectVisitor>();
                    gameState.allyTank->handleCollision(visitor.get());
                }
            }
        }

        for (const auto &bullet: gameState.allBullets) {
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

        for (const auto &enemy: gameState.enemyTanks) {
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

    if (gameState.bonus.use_count() != 0) {
        auto visitor = std::make_shared<CollisionWithTankVisitor>();
        if (gameState.bonus->getSprite().getGlobalBounds().intersects(
                gameState.playerTank->getSprite().getGlobalBounds()))
            gameState.bonus->handleCollision(visitor.get());

        if (gameState.allyTank.use_count() != 0) {
            if (gameState.bonus->getSprite().getGlobalBounds().intersects(
                    gameState.allyTank->getSprite().getGlobalBounds()))
                gameState.bonus->handleCollision(visitor.get());
        }
    }
}

void GameEngine::render() {
    for (auto &bullet: gameState.allBullets) {
        bullet->render(window);
    }

    if (gameState.playerTank.use_count() != 0)
        gameState.playerTank->render(window);

    if (isTwoPlayers && gameState.allyTank.use_count() != 0)
        gameState.allyTank->render(window);

    for (auto &enemyTank: gameState.enemyTanks) {
        enemyTank->render(window);
    }

    Map::render(gameState.map, window);

    //рендер бонуса
    if (gameState.bonus != nullptr) {
        if (!dynamic_cast<Bonus *>(gameState.bonus.get())->getIsPicked())
            gameState.bonus->render(window);
    }
    window.draw(hud);

    renderHUD();

    if (gameState.isPaused) {
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
    for (int i = 0; i < gameState.remainingEnemies / 2; i++) {
        for (int j = 0; j < 2; j++) {
            littleTankSprite.setPosition((float) (216 + j * 8) * FACTOR,
                                         (float) (16 + i * 8) * FACTOR);  // задание координат клетки
            littleTankSprite.setScale(FACTOR, FACTOR);
            remEnem.push_back(littleTankSprite);
        }
    }
    if (gameState.remainingEnemies % 2 == 1) {
        littleTankSprite.setPosition(216 * FACTOR, (float) (16 + (gameState.remainingEnemies / 2) * 8) * FACTOR);
        littleTankSprite.setScale(FACTOR, FACTOR);
        remEnem.push_back(littleTankSprite);
    }
    for (const auto &sprite: remEnem) {
        window.draw(sprite);
    }

    if (gameState.playerTank.use_count() != 0) {
        int tmpHealths = dynamic_cast<PlayerTank *>(gameState.playerTank.get())->getHealths();
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

    gameState.playerTank = std::make_shared<PlayerTank>(PLAYER_SPAWN_X * FACTOR, PLAYER_SPAWN_Y * FACTOR,
                                                        gameState.allBullets, false);
    if (isTwoPlayers)
        gameState.allyTank = std::make_shared<PlayerTank>(ALLY_SPAWN_X * FACTOR, ALLY_SPAWN_Y * FACTOR,
                                                          gameState.allBullets, true);

    gameState.bonus.reset();
    gameState.remainingEnemies = 18;
    enemiesOnScreen = 0;
    gameState.points = 0;
    gameState.allBullets.clear();
    gameState.enemyTanks.clear();
    gameState.map.clear();

    Map::loadMap(gameState.map, levelNum);
}

void GameEngine::dropBonus() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 25);

    std::uniform_int_distribution<int> distributionType(0, 5);

    int x = distribution(generator);
    int y = distribution(generator);
    int type = distributionType(generator);

    gameState.bonus = std::make_shared<Bonus>(8 * FACTOR * x, 8 * FACTOR * y, type);
}

void GameEngine::bonusEffect() {

    auto *handlingBonus = dynamic_cast<Bonus *>(gameState.bonus.get());

    if (!(handlingBonus->getIsPicked()))
        return;

    gameState.points += BONUS_POINTS;

    if (handlingBonus->getBonusType() == HELMET) {
        if (gameState.bonus->getSprite().getGlobalBounds().intersects(
                gameState.playerTank->getSprite().getGlobalBounds()))
            dynamic_cast<PlayerTank *>(gameState.playerTank.get())->setIsInvulnerable();
        else if (isTwoPlayers)
            dynamic_cast<PlayerTank *>(gameState.allyTank.get())->setIsInvulnerable();
    } else if (handlingBonus->getBonusType() == CLOCK) {
        isClockBonusActive = true;
        clockBonusTimer.restart();
    } else if (handlingBonus->getBonusType() == SHOVEL) // устанавливает металл вокруг базы
    {

    } else if (handlingBonus->getBonusType() == STAR) {
        if (gameState.bonus->getSprite().getGlobalBounds().intersects(
                gameState.playerTank->getSprite().getGlobalBounds()))
            dynamic_cast<PlayerTank *>(gameState.playerTank.get())->addStar();
        else if (isTwoPlayers)
            dynamic_cast<PlayerTank *>(gameState.allyTank.get())->addStar();
    } else if (handlingBonus->getBonusType() == GRENADE) {
        enemiesOnScreen = 0;
        gameState.enemyTanks.clear();
    } else if (handlingBonus->getBonusType() == TANK) {
        if (gameState.bonus->getSprite().getGlobalBounds().intersects(
                gameState.playerTank->getSprite().getGlobalBounds()))
            dynamic_cast<PlayerTank *>(gameState.playerTank.get())->addHealth();
        else if (isTwoPlayers)
            dynamic_cast<PlayerTank *>(gameState.allyTank.get())->addHealth();
    }
    gameState.bonus.reset();
}

void GameEngine::dropEnemies() {
    if (enemiesOnScreen != 4)
        timeBetweenRenderEnemyTank--;

    if (gameState.remainingEnemies != 0 && enemiesOnScreen < 4 && timeBetweenRenderEnemyTank <= 0) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> distribution(0, 3);
        int type = distribution(generator);

        enemyWithBonusCounter++;

        std::shared_ptr<IGameObject> enemy;

        if ((enemyWithBonusCounter % 4) == 0) {
            enemy = std::make_shared<EnemyTank>(6. * (gameState.remainingEnemies % 3) * 16. * FACTOR, 0.,
                                                gameState.allBullets, type,
                                                true);
            enemyWithBonusCounter = 0;
        } else
            enemy = std::make_shared<EnemyTank>(6. * (gameState.remainingEnemies % 3) * 16. * FACTOR, 0.,
                                                gameState.allBullets, type,
                                                false);

        gameState.enemyTanks.push_back(enemy);
        gameState.remainingEnemies--;
        enemiesOnScreen++;

        timeBetweenRenderEnemyTank = TIME_BETWEEN_RENDER_ENEMY_TANK;
    }
}

void GameEngine::end() {
    // заставка мб
}
