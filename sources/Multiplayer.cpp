#include "../headers/Multiplayer.h"
#include "../headers/GameEngine.h"

bool Multiplayer::serverInit(int port) {
    this->port = port;
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1) {
        std::cout << "socket creation failed";
        close(server);
        exit(0);
    } else
        std::cout << "socket created" << std::endl;

    settings.sin_family = AF_INET;
    settings.sin_addr.s_addr = htonl(INADDR_ANY);
    settings.sin_port = htons(port);

    if (bind(server, (struct sockaddr *) &settings, sizeof(settings))) {
        std::cout << "socket bind failed";
        close(server);
        exit(0);
    } else
        std::cout << "socket binded" << std::endl;

    if (listen(server, 1) != 0) {
        std::cout << "listen error";
        close(server);
        exit(0);
    } else
        std::cout << "server listening..." << std::endl;

    struct sockaddr_in cli;
    int len = sizeof(cli);

    client = accept(server, (struct sockaddr *) &cli, reinterpret_cast<socklen_t *>(&len));
    if (client < 0) {
        std::cout << "server accepted failed";
        close(server);
        exit(0);
    } else
        std::cout << "server accepted successfully" << std::endl;

    return true;
}

bool Multiplayer::clientInit(std::string ip, int port) {
    this->port = port;
    this->ip = ip;
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) {
        std::cout << "socket creation failed";
        close(client);
        exit(0);
    } else
        std::cout << "socket created" << std::endl;

    bzero(&settings, sizeof(settings));

    settings.sin_family = AF_INET;
    settings.sin_addr.s_addr = inet_addr(ip.c_str());
    settings.sin_port = htons(this->port);
    if (connect(client, (struct sockaddr *) &settings, sizeof(settings)) != 0) {
        std::cout << "connection with server failed";
        close(client);
        return false;
    } else
        std::cout << "connected to server" << std::endl;
    return true;
}

std::string Multiplayer::getIpFromStruct() {
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp) {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *pAddr = (struct sockaddr_in *) tmp->ifa_addr;
            if (strcmp(tmp->ifa_name, "lo0") != 0 &&
                (pAddr->sin_addr.s_addr & htonl(0xff000000)) != htonl(0x7f000000)) {
                return inet_ntoa(pAddr->sin_addr);
            }
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
}

std::string Multiplayer::serializeGameState(const GameState &gameState) {
    SendReceiveMessage message;
    message.generateMessage(gameState);
    std::ostringstream oss;
    boost::archive::binary_oarchive archive(oss);
    archive << message;
    return oss.str();
}

void Multiplayer::deserializeGameState(GameState &gameState, std::string data, bool isServer) {

    std::istringstream iss(data);
    boost::archive::binary_iarchive ar(iss);
    SendReceiveMessage message;
    ar >> message;
    if (isServer)
        message.parsingMessageForServer(gameState);
    else
        message.parsingMessageForClient(gameState);
}

void SendReceiveMessage::generateMessage(const GameState &gameState) {
    if (gameState.playerTank1.use_count() != 0) {
        this->pt[0].dx = gameState.playerTank1->getX();
        this->pt[0].dy = gameState.playerTank1->getY();
        this->pt[0].time = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getTime();
        this->pt[0].isDestroyed = gameState.playerTank1->getIsDestroyed();
        this->pt[0].stars = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getStars();
        this->pt[0].health = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getHealth();
        this->pt[0].destination = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getDestination();
        this->pt[0].isInvulnerable = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getIsInvulnerable();
        this->pt[0].animation = dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->getAnimation();
    }

    if (gameState.playerTank2.use_count() != 0) {
        this->pt[1].dx = gameState.playerTank2->getX();
        this->pt[1].dy = gameState.playerTank2->getY();
        this->pt[1].time = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getTime();
        this->pt[1].isDestroyed = gameState.playerTank2->getIsDestroyed();
        this->pt[1].stars = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getStars();
        this->pt[1].health = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getHealth();
        this->pt[1].destination = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getDestination();
        this->pt[1].isInvulnerable = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getIsInvulnerable();
        this->pt[1].animation = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getAnimation();

        this->pt[1].canShoot = dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getCanShoot();
    }

    if (!gameState.enemyTanks.empty()) {
        for (int i = 0; i < gameState.enemyTanks.size(); i++) {
            this->et[i].dx = gameState.enemyTanks[i]->getX();
            this->et[i].dy = gameState.enemyTanks[i]->getY();
            this->et[i].isDestroyed = gameState.enemyTanks[i]->getIsDestroyed();
            this->et[i].destination = dynamic_cast<EnemyTank *>(gameState.enemyTanks[i].get())->getDestination();
            this->et[i].type = dynamic_cast<EnemyTank *>(gameState.enemyTanks[i].get())->getType();
            this->et[i].isBonus = dynamic_cast<EnemyTank *>(gameState.enemyTanks[i].get())->getIsBonusTank();
        }
    }

    if (!gameState.allBullets.empty()) {
        for (int i = 0; i < gameState.allBullets.size(); i++) {
            this->bul[i].dx = gameState.allBullets[i]->getX();
            this->bul[i].dy = gameState.allBullets[i]->getY();
            this->bul[i].isDestroyed = gameState.allBullets[i]->getIsDestroyed();
            this->bul[i].destination = dynamic_cast<Bullet *>(gameState.allBullets[i].get())->getDestination();
        }
    }

    for (int i = 0; i < gameState.map.size(); i++) {
        this->map[i].dx = gameState.map[i]->getX();
        this->map[i].dy = gameState.map[i]->getY();
        this->map[i].isDestroyed = gameState.map[i]->getIsDestroyed();
    }

    if (gameState.bonus.use_count() != 0) // добавить тип бонуса
    {
        this->bonus.dx = gameState.bonus->getX();
        this->bonus.dy = gameState.bonus->getY();
        this->bonus.isDestroyed = gameState.bonus->getIsDestroyed();
        this->bonus.isPicked = dynamic_cast<Bonus *>(gameState.bonus.get())->getIsPicked();
        this->bonus.type = dynamic_cast<Bonus *>(gameState.bonus.get())->getBonusType();
    }

    this->levelNum = gameState.levelNum;
    this->remainingEnemies = gameState.remainingEnemies;
    this->isPaused = gameState.isPaused;
    this->enemiesOnScreen = gameState.enemiesOnScreen;
    this->numOfBullets = gameState.allBullets.size();
}

void SendReceiveMessage::parsingMessageForClient(GameState &gameState) {
    gameState.enemiesOnScreen = this->enemiesOnScreen;
    gameState.remainingEnemies = this->remainingEnemies;
    gameState.isPaused = this->isPaused;

    gameState.playerTank1->setX(this->pt[0].dx);
    gameState.playerTank1->setY(this->pt[0].dy);
    dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->setTime(this->pt[0].time);
    gameState.playerTank1->setIsDestroyed(this->pt[0].isDestroyed);
    dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->setStars(this->pt[0].stars);
    dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->setHealth(this->pt[0].health);
    dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->setDestination(this->pt[0].destination);
    dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->setIsInvulnerable(this->pt[0].isInvulnerable);
    dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->setAnimation(this->pt[0].animation);
    dynamic_cast<PlayerTank *>(gameState.playerTank1.get())->setSprite();

    gameState.playerTank2->setX(this->pt[1].dx);
    gameState.playerTank2->setY(this->pt[1].dy);
    dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setTime(this->pt[1].time);
    gameState.playerTank2->setIsDestroyed(this->pt[1].isDestroyed);
    dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setStars(this->pt[1].stars);
    dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setHealth(this->pt[1].health);
    dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setDestination(this->pt[1].destination);
    dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setIsInvulnerable(this->pt[1].isInvulnerable);
    dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setAnimation(this->pt[1].animation);
    dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setCanShoot(this->pt[1].canShoot);
    // танку игрока 2 нет смысла устанавливать спрайт, так как его спрайт уже стоит как надо

    for (auto enemy: gameState.enemyTanks)
        enemy.reset();
    gameState.enemyTanks.clear();

    for (int i = 0; i < enemiesOnScreen; i++) {
        auto tank = std::make_shared<EnemyTank>(this->et[i].dx, this->et[i].dy, gameState.allBullets, this->et[i].type,
                                                this->et[i].isBonus);
        tank->setDestination(this->et[i].destination);
        tank->setSprite();
        gameState.enemyTanks.push_back(tank);
    }

    for (auto bullet: gameState.allBullets)
        bullet.reset();
    gameState.allBullets.clear();

    for (int i = 0; i < numOfBullets; i++) {
        switch (this->bul[i].destination) {
            case UP:
                this->bul[i].dx -= 5.f * FACTOR;
                this->bul[i].dy += 6.f * FACTOR;
                break;
            case DOWN:
                this->bul[i].dx -= 5.f * FACTOR;
                this->bul[i].dy -= 15.f * FACTOR;
                break;
            case LEFT:
                this->bul[i].dx += 6.f * FACTOR;
                this->bul[i].dy -= 5.f * FACTOR;
                break;
            case RIGHT:
                this->bul[i].dx -= 15.f * FACTOR;
                this->bul[i].dy -= 5.f * FACTOR;
                break;
        }
        auto bullet = std::make_shared<PlayerBullet>(this->bul[i].dx,
                                                     this->bul[i].dy, // сденлать чтобы не криво было как-то
                                                     static_cast<Destination>(this->bul[i].destination),
                                                     gameState.playerTank2);
        gameState.allBullets.push_back(bullet);
    }

    for (int i = 0; i <
                    gameState.map.size(); i++) // уничтоженные объекты карты не удаляются из вектора, поэтому его размер неизменен
    {
        gameState.map[i]->setX(this->map[i].dx);
        gameState.map[i]->setY(this->map[i].dy);
        gameState.map[i]->setIsDestroyed(this->map[i].isDestroyed);
    }

    if (!this->bonus.isDestroyed && gameState.bonus.use_count() == 0)
        gameState.bonus = std::make_shared<Bonus>(this->bonus.dx, this->bonus.dy, this->bonus.type);
    else if (this->bonus.isDestroyed)
        gameState.bonus.reset();
    if (gameState.levelNum != this->levelNum) {
        gameState.levelNum = this->levelNum;
        Map::loadMap(gameState.map, gameState.levelNum);
    }
}

void SendReceiveMessage::parsingMessageForServer(GameState &gameState) {
    if (gameState.playerTank2.use_count() != 0) {
        gameState.playerTank2->setX(this->pt[1].dx);
        gameState.playerTank2->setY(this->pt[1].dy);
        //dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setTime(this->pt[1].time);
        gameState.playerTank2->setIsDestroyed(this->pt[1].isDestroyed);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setStars(this->pt[1].stars);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setDestination(this->pt[1].destination);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setIsInvulnerable(this->pt[1].isInvulnerable);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setAnimation(this->pt[1].animation);
        dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setSprite();

        if (!this->pt[1].canShoot) // если клиент выстрелил
        {
            if (dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->getCanShoot()) // и этот выстрел не был еще обработан
            {
                dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->setCanShoot(false);
                dynamic_cast<PlayerTank *>(gameState.playerTank2.get())->shoot();
            }
        }
    }
}